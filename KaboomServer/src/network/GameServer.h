#pragma once

#include <network/NetworkData.h>
#include <network/DeleteEvent.h>
#include <core/BombContainerComponent.h>
#include "ServerEventHandlerLookup.h"
#include "ServerNetwork.h"
#include "NetworkServices.h"

class Entity;
class Game;

class GameServer {
public:

    GameServer(ConfigSettings *, ServerEventHandlerLookup *eventHandlerLookup);
    ~GameServer();

    bool acceptNewClient(unsigned int);

    void receive(Game *game);

    void sendGameStatePackets(Game *game);

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