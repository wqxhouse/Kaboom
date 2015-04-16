#pragma once

#include <network/NetworkData.h>

#include "ClientEventHandlerLookup.h"
#include "ClientNetwork.h"

class GameClient {
public:
    GameClient(ConfigSettings *config, ClientEventHandlerLookup *eventHandlerLookup);
    ~GameClient();

	void receive();

    void GameClient::sendMessage(const Event &evt);

private:
    ClientEventHandlerLookup *eventHandlerLookup;

    ClientNetwork *network;

    char networkData[MAX_PACKET_SIZE];
};
