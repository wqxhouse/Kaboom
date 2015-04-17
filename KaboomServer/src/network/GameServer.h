#pragma once

#include <network/NetworkData.h>

#include "ServerEventHandlerLookup.h"
#include "ServerNetwork.h"
#include "NetworkServices.h"

#include "../core/Game.h"

class Game;
class ServerEventHandlerLookup;

class GameServer {
public:

    GameServer(ConfigSettings *, Game *game);
    ~GameServer();

    bool acceptNewClient();

    void receive();

    void sendGameStatePackets(Game *game);

	void sendPlayerSpawnEvent(Entity *entity);

    void sendPositionEvent(Entity *entity);

private:
    ServerEventHandlerLookup *eventHandlerLookup;

	// IDs for the clients connecting for table in ServerNetwork 
	static unsigned int client_id;

	// The ServerNetwork object 
	ServerNetwork* network;

	// data buffer
	char network_data[MAX_PACKET_SIZE];

	std::map<int, int> playerIndexNum;

};