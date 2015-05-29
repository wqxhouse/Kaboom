#include "ServerNetwork.h"

#include <ws2tcpip.h>

#include <network/NetworkData.h>
#include <util/ConfigSettings.h>

#include "NetworkServices.h"

ServerNetwork::ServerNetwork(ConfigSettings *config) {
    std::string serverPort;
    config->getValue(ConfigSettings::str_server_port, serverPort);

    // create WSADATA object
    WSADATA wsaData;

    // our sockets for the server
    listenSocket = INVALID_SOCKET;

    // address info for the server to listen to
    struct addrinfo *result = NULL;
    struct addrinfo hints;

    // Initialize Winsock
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("<Server> WSAStartup failed with error: %d\n", iResult);
        exit(1);
    }

    // set address information
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;    // TCP connection!!!
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, serverPort.c_str(), &hints, &result);

    if (iResult != 0) {
        printf("<Server> getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        exit(1);
    }

    // Create a SOCKET for connecting to server
    listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (listenSocket == INVALID_SOCKET) {
        printf("<Server> socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        exit(1);
    }

    // Set the mode of the socket to be nonblocking
    u_long iMode = 1;
    iResult = ioctlsocket(listenSocket, FIONBIO, &iMode);

    if (iResult == SOCKET_ERROR) {
        printf("<Server> ioctlsocket failed with error: %d\n", WSAGetLastError());
        closesocket(listenSocket);
        WSACleanup();
        exit(1);
    }

    // Setup the TCP listening socket
    iResult = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);

    if (iResult == SOCKET_ERROR) {
        printf("<Server> bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(listenSocket);
        WSACleanup();
        exit(1);
    }

    // no longer need address information
    freeaddrinfo(result);

    // start listening for new clients attempting to connect
    iResult = listen(listenSocket, SOMAXCONN);

    if (iResult == SOCKET_ERROR) {
        printf("<Server> listen failed with error: %d\n", WSAGetLastError());
        closesocket(listenSocket);
        WSACleanup();
        exit(1);
    }

    struct sockaddr_in sin;
    socklen_t len = sizeof(sin);
    if (getsockname(listenSocket, (struct sockaddr *)&sin, &len) == -1)
        perror("getsockname");
    else
        printf("<Server> Begin listening on port %d\n", ntohs(sin.sin_port));
}

bool ServerNetwork::acceptClient(unsigned int &playerId) {
    sockaddr_in addr;
    int len = sizeof(addr);
    memset(&addr, 0, len);
    SOCKET socket = accept(listenSocket, (sockaddr *)&addr, &len);

    // Don't buffer packet
    int flag = 1;
    setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(int));

    if (socket != INVALID_SOCKET) {
        playerId = playerIdPool.allocate();
        sessions[playerId] = socket;

        char clientIp[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &addr.sin_addr, clientIp, INET_ADDRSTRLEN);
        int clientPort = (int)ntohs(addr.sin_port);

        printf("<Server> Player %d (%s:%d) connected to server.\n", playerId, clientIp, clientPort);
        return true;
    }

    return false;
}

int ServerNetwork::receive(unsigned int playerId, char *recvbuf, int bufSize) {
    SOCKET socket = sessions[playerId];

    int iResult = NetworkServices::receiveMessage(socket, recvbuf, bufSize);

    if (iResult == 0) {
        printf("<Server> Player %d disconnected.\n", playerId);
        disconnectedPlayerIds.insert(playerId);
    }

    return iResult;
}

void ServerNetwork::send(char *packet, int size) {
    for (auto kv : sessions) {
        const auto playerId = kv.first;
        const auto socket = kv.second;
        send(packet, size, playerId, socket);
    }
}

void ServerNetwork::send(char *packet, int size, int playerId) {
    const auto socket = sessions[playerId];
    send(packet, size, playerId, socket);
}

void ServerNetwork::removeDisconnectedPlayers() {
    for (auto id : disconnectedPlayerIds) {
        const auto kv = sessions.find(id);
        const auto playerId = kv->first;
        const auto socket = kv->second;
        closesocket(socket);
        sessions.erase(playerId);
        playerIdPool.free(playerId);

        if (packetQueues.count(playerId) > 0) {
            auto &queue = packetQueues.at(playerId);

            while (!queue.empty()) {
                delete[] queue.front().data;
                queue.pop_front();
            }
        }
    }

    disconnectedPlayerIds.clear();
}

void ServerNetwork::send(char *data, int size, unsigned int playerId, SOCKET socket) {
    Packet packet;
    packet.data = data;
    packet.size = size;

    if (packetQueues.count(playerId) == 0) {
        packetQueues[playerId] = std::deque<Packet>();
    }

    auto &queue = packetQueues[playerId];

    queue.push_back(packet);

    while (!queue.empty()) {
        Packet packet = packetQueues[playerId].front();

        int iSendResult = NetworkServices::sendMessage(socket, packet.data, packet.size);

        if (iSendResult == SOCKET_ERROR) {
            if (WSAGetLastError() == WSAEWOULDBLOCK) {
                break;
            } else {
                printf("<Server> Unable to send to player %d (socket error: %d).\n", playerId, WSAGetLastError());
                disconnectedPlayerIds.insert(playerId);
            }
        } else {
            delete[] packet.data;
            queue.pop_front();
        }
    }
}
