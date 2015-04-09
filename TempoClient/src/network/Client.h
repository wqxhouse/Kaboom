#pragma once

#include "ClientNetwork.h"
#include "NetworkData.h"

class Client {
public:
    Client(ConfigSettings *);
    ~Client();

    ClientNetwork *network;

    char network_data[MAX_PACKET_SIZE];

    void receive();
    void sendMoveEvent(bool movingForward, bool movingBackward, bool movingLeft, bool movingRight);
};
