#pragma once

#include "ServerEventHandlerLookup.h"
#include "ServerNetwork.h"
#include "NetworkData.h"
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

	void sendActionPackets();

    void sendMovePackets();

    void sendGameStatePackets(Game *game);

	void sendAssignPackets(int);

	void sendPlayerSpawnEvent(ServerPlayer *);

	void sendPlayerInputEvent(ServerPlayer *, MoveEvent *);

	void sendPlayerPositionEvent(ServerPlayer *);

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