#pragma once

#include <network/Event.h>
#include <network/NetworkData.h>

#include "ClientNetwork.h"

class ClientEventHandlerLookup;

class GameClient {
public:
    GameClient(ConfigSettings *config, ClientEventHandlerLookup *eventHandlerLookup);
    ~GameClient();

	void receive();
	void connectToServer();
	void disconnectFromServer();

    void sendMessage(const Event &evt);

private:
    ClientEventHandlerLookup *eventHandlerLookup;

    ClientNetwork *network;

    char networkData[MAX_PACKET_SIZE];

	unsigned int currentPlayerEntityId;

	bool initialized = false;
};
