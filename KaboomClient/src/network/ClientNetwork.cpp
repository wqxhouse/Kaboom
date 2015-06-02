#include "ClientNetwork.h"

#include <cstdio>

#include <WS2tcpip.h>

#include <network/NetworkData.h>

#include "NetworkServices.h"

ClientNetwork::ClientNetwork() {
}

ClientNetwork::~ClientNetwork() {
	printf("<Client> disconnecting from server");
	closesocket(clientSocket);
	WSACleanup();
}

void ClientNetwork::connectToServer(const std::string &serverAddress, const int &serverPort) {
	if (_connected) return;

	bool hasError = false;

    printf("<Client> Connecting to %s:%d\n", serverAddress.c_str(), serverPort);

	//create WSADATA object
	WSADATA wsaData;

	// socket
	clientSocket = INVALID_SOCKET;

	// holds address info for socket to connect to
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;

	// Initialize Winsock
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult != 0) {
		fprintf(stderr, "<Client> WSAStartup failed with error: %d\n", iResult);
		// exit(1);
		hasError = true;
	}

	// set address info
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;  //TCP connection!!!

	//resolve server address and port 

	iResult = getaddrinfo(serverAddress.c_str(), "2350", &hints, &result);

	if (iResult != 0) {
		fprintf(stderr, "<Client> getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		// exit(1);
		hasError = true;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		clientSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);

		if (clientSocket == INVALID_SOCKET) {
			fprintf(stderr, "<Client> socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			// exit(1);
			hasError = true;
		}

		// Don't buffer packet
		int flag = 1;
		setsockopt(clientSocket, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(int));

		// Connect to server.
		iResult = connect(clientSocket, ptr->ai_addr, (int)ptr->ai_addrlen);

		if (iResult == SOCKET_ERROR) {
			closesocket(clientSocket);
			clientSocket = INVALID_SOCKET;

			printf("<Client> The server is down... did not connect %p\n", ptr->ai_addr);
		}
	}

	// no longer need address info for server
	freeaddrinfo(result);

	// if connection failed
	if (clientSocket == INVALID_SOCKET) {
		fprintf(stderr, "<Client> Unable to connect to server!\n");
		WSACleanup();
		// exit(1);
		hasError = true;
	}


	// Set the mode of the socket to be nonblocking
	u_long iMode = 1;

	iResult = ioctlsocket(clientSocket, FIONBIO, &iMode);
	if (iResult == SOCKET_ERROR) {
		fprintf(stderr, "<Client> ioctlsocket failed with error: %d\n", WSAGetLastError());
		closesocket(clientSocket);
		WSACleanup();
		// exit(1);
		hasError = true;
	}
	if (hasError)
	{
		_connected = false;
		fprintf(stderr, "<Client> Connection failed...\n");
	}
	else
	{
		_connected = true;
		printf("<Client> Connected to server...\n");
	}
}

void ClientNetwork::disconnectFromServer()
{
	if (!_connected) return;
	
	printf("<Client> Manually Disconnecting from server\n");

	// shutdown the send half of the connection since no more data will be sent
	int iResult = shutdown(clientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed: %d\n", WSAGetLastError());
	}
	closesocket(clientSocket);
	WSACleanup();

	_connected = false;
}

int ClientNetwork::receivePackets(char * recvbuf, int bufSize) {
    int iResult = NetworkServices::receiveMessage(clientSocket, recvbuf, bufSize);

    if (iResult == 0) {
        fprintf(stderr, "<Client> Server is disconnected, terminating connecting\n");
        closesocket(clientSocket);
        WSACleanup();
        // exit(1);
    }

    return iResult;
}

void ClientNetwork::sendMessage(char *message, int size) const {
    NetworkServices::sendMessage(clientSocket, message, size);
}
