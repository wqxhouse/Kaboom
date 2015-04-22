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

    bool acceptNewClient(unsigned int);

    void receive();

    void sendGameStatePackets(Game *game);

	void sendPlayerSpawnEvent(Entity *entity, std::vector<Entity *> players);

    void sendPositionEvent(Entity *entity);
	void sendEntitySpawnEvent(Entity* newEntity);
	void sendAllEntitiesSpawnEvent(Entity* newEntity, std::vector<Entity *> players);

private:
    ServerEventHandlerLookup *eventHandlerLookup;

	// IDs for the clients connecting for table in ServerNetwork 
	static unsigned int client_id;

	// The ServerNetwork object 
	ServerNetwork* network;

	// data buffer
	char network_data[MAX_PACKET_SIZE];


};