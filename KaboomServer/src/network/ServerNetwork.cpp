#include "ServerNetwork.h"

ServerNetwork::ServerNetwork(ConfigSettings * _config) {
    //Save the ConfigSetting
    config = _config;
    string serverPort;
    config->getValue(ConfigSettings::str_server_port, serverPort);


    // create WSADATA object
    WSADATA wsaData;

    // our sockets for the server
    ListenSocket = INVALID_SOCKET;

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
    iResult = getaddrinfo(NULL, "2350", &hints, &result);

    if (iResult != 0) {
        printf("<Server> getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        exit(1);
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (ListenSocket == INVALID_SOCKET) {
        printf("<Server> socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        exit(1);
    }

    // Set the mode of the socket to be nonblocking
    u_long iMode = 1;
    iResult = ioctlsocket(ListenSocket, FIONBIO, &iMode);

    if (iResult == SOCKET_ERROR) {
        printf("<Server> ioctlsocket failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        exit(1);
	}

    // Setup the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);

    if (iResult == SOCKET_ERROR) {
        printf("<Server> bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        exit(1);
    }

    // no longer need address information
    freeaddrinfo(result);

    // start listening for new clients attempting to connect
    iResult = listen(ListenSocket, SOMAXCONN);

    if (iResult == SOCKET_ERROR) {
        printf("<Server> listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        exit(1);
    }

    struct sockaddr_in sin;
    socklen_t len = sizeof(sin);
    if (getsockname(ListenSocket, (struct sockaddr *)&sin, &len) == -1)
        perror("getsockname");
    else
        printf("<Server> Begin listening on port %d\n", ntohs(sin.sin_port));
}

bool ServerNetwork::acceptNewClient(unsigned int id) {
    // if client waiting, accept the connection and save the socket
	SOCKET socket = accept(ListenSocket, NULL, NULL);

	// Don't buffer packet
	int flag = 1;
	setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(int));

    if (socket != INVALID_SOCKET) {
        // insert new client into session id table
        sessions.insert(pair<unsigned int, SOCKET>(id, socket));
        return true;
    }

    return false;

}

int ServerNetwork::receive(unsigned int clientId, char *recvbuf) {
    SOCKET socket = sessions[clientId];

    int iResult = NetworkServices::receiveMessage(socket, recvbuf, MAX_PACKET_SIZE);

    if (iResult == 0) {
        printf("<Server> Client Disconncect, closing connection\n");
        disconnectedClients.insert(clientId);
    }

    return iResult;
}

void ServerNetwork::send(char *packet, int size) {
    for (auto it : sessions) {
        int iSendResult = NetworkServices::sendMessage(it.second, packet, size);

        if (iSendResult == SOCKET_ERROR) {
            printf("send failed with error: %d\n", WSAGetLastError());
            disconnectedClients.insert(it.first);
        }
    }
}

void ServerNetwork::send(char *packet, int size, int clientId) {
    auto kv = sessions.find(clientId);

    int iSendResult = NetworkServices::sendMessage(kv->second, packet, size);

    if (iSendResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        disconnectedClients.insert(kv->first);
    }
}

void ServerNetwork::removeDisconnectedClients() {
    for (auto clientId : disconnectedClients) {
        auto kv = sessions.find(clientId);
        closesocket(kv->second);
        sessions.erase(kv->first);
    }

    disconnectedClients.clear();
}
