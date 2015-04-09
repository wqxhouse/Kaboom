#pragma once

#include <winsock2.h>
#include <Windows.h>

#include "ClientNetwork.h"
#include "NetworkData.h"
#include "NetworkServices.h"

class Client {

public:

    Client(ConfigSettings *);
    ~Client(void);

    ClientNetwork* network;

    char network_data[MAX_PACKET_SIZE];

    void receive();
    void sendMoveEvent(bool movingForward, bool movingBackward, bool movingLeft, bool movingRight);

};