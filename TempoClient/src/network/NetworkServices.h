#pragma once

#include <WinSock2.h>

class NetworkServices {
public:
    static int sendMessage(SOCKET curSocket, char * message, int messageSize);
    static int receiveMessage(SOCKET curSocket, char * buffer, int bufSize);
};