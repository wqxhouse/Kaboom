#pragma once

#include <network/Event.h>
#include <network/PlayerSpawnEvent.h>
#include <network/PlayerInputEvent.h>
#include <network/PositionEvent.h>
#include <network/RotationEvent.h>

#include "ServerNetwork.h"
#include "NetworkData.h"
#include "NetworkServices.h"

#include "../core/Game.h"


class Game;

class GameServer {
public:

    GameServer(ConfigSettings *);
    ~GameServer();

    bool acceptNewClient();

    void receiveFromClients(Game *game);

	void sendActionPackets();

    void sendMovePackets();

    void sendGameStatePackets(Game *game);

	void sendAssignPackets(int);

	void sendPlayerSpawnEvent(ServerPlayer *);

	void sendPlayerInputEvent(ServerPlayer *, MoveEvent *);

	void sendPlayerPositionEvent(ServerPlayer *);

private:

	// IDs for the clients connecting for table in ServerNetwork 
	static unsigned int client_id;

	// The ServerNetwork object 
	ServerNetwork* network;

	// data buffer
	char network_data[MAX_PACKET_SIZE];

	// This is the game state data we send over the network
    GameStateUpdateEvent gameStateUpdateEvent;

	std::map<int, int> playerIndexNum;

};