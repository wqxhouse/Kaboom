#pragma once

#include <WinSock2.h>

#include "../util/ConfigSettings.h"

class ClientNetwork {
public:
    ConfigSettings *config;

    // for error checking function calls in Winsock library
    int iResult;

    // socket for client to connect to server
    SOCKET clientSocket;

    // ctor/dtor
    ClientNetwork(ConfigSettings *);
    ~ClientNetwork();

    int receivePackets(char *);
};