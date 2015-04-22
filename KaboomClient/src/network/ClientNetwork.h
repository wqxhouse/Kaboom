#pragma once

#include <WinSock2.h>
#include <ConfigSettings.h>

class ClientNetwork {
public:

    // for error checking function calls in Winsock library
    int iResult;

    // socket for client to connect to server
    SOCKET clientSocket;

    // ctor/dtor
    ClientNetwork(ConfigSettings *);
    ~ClientNetwork();

	void connectToServer();
	void disconnectFromServer();

    int receivePackets(char *);

private:
    ConfigSettings *_config;

	bool _connected;
};