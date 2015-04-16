#include "ClientNetwork.h"

#include <cstdio>

#include <WS2tcpip.h>

#include <network/NetworkData.h>

#include "NetworkServices.h"

ClientNetwork::ClientNetwork(ConfigSettings * _config) {
    config = _config;
    string serverAddress;
    int serverPort;
    config->getValue(ConfigSettings::str_server_address, serverAddress);
    config->getValue(ConfigSettings::str_server_port, serverPort);


    printf("<Client> port %d\n", serverPort);
    printf("<Client> addrees %s\n", serverAddress);


    //create WSADATA object
    WSADATA wsaData;

    // socket
    clientSocket = INVALID_SOCKET;

    // holds address info for socket to connect to
    struct addrinfo *result = NULL,
        *ptr = NULL,
        hints;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (iResult != 0) {
        printf("<Client> WSAStartup failed with error: %d\n", iResult);
        exit(1);
    }

    // set address info
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;  //TCP connection!!!

    //resolve server address and port 

    iResult = getaddrinfo(serverAddress.c_str(), "2350", &hints, &result);

    if (iResult != 0) {
        printf("<Client> getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        exit(1);
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        clientSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);

        if (clientSocket == INVALID_SOCKET) {
            printf("<Client> socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            exit(1);
        }

        // Connect to server.
        iResult = connect(clientSocket, ptr->ai_addr, (int)ptr->ai_addrlen);

        if (iResult == SOCKET_ERROR) {
            closesocket(clientSocket);
            clientSocket = INVALID_SOCKET;

            printf("<Client> The server is down... did not connect %x\n", ptr->ai_addr);
        }
    }



    // no longer need address info for server
    freeaddrinfo(result);

    // if connection failed
    if (clientSocket == INVALID_SOCKET) {
        printf("<Client> Unable to connect to server!\n");
        WSACleanup();
        exit(1);
    }


    // Set the mode of the socket to be nonblocking
    u_long iMode = 1;

    iResult = ioctlsocket(clientSocket, FIONBIO, &iMode);
    if (iResult == SOCKET_ERROR) {
        printf("<Client> ioctlsocket failed with error: %d\n", WSAGetLastError());
        closesocket(clientSocket);
        WSACleanup();
        exit(1);
    }

}

ClientNetwork::~ClientNetwork() {
	printf("<Client> disconnecting from server");
	closesocket(clientSocket);
	WSACleanup();
}

int ClientNetwork::receivePackets(char * recvbuf) {
    iResult = NetworkServices::receiveMessage(clientSocket, recvbuf, MAX_PACKET_SIZE);

    if (iResult == 0) {
        printf("<Client> Server is disconnected, terminating connecting\n");
        closesocket(clientSocket);
        WSACleanup();
        exit(1);
    }

    return iResult;
}
