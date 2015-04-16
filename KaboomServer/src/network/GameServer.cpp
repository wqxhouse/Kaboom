#include "GameServer.h"

#include <network/EmptyEvent.h>
#include <network/PlayerSpawnEvent.h>
#include <network/PlayerInputEvent.h>
#include <network/PositionEvent.h>
#include <network/RotationEvent.h>

unsigned int GameServer::client_id;

GameServer::GameServer(ConfigSettings * config, Game *game)
    : eventHandlerLookup(new ServerEventHandlerLookup(game)) {
    // id's to assign clients for our table
    client_id = 0;

	printf("<Server> Creating a Network Server\n");
    // set up the server network to listen 
    network = new ServerNetwork(config);


	printf("<Sevrer> Initializing a Game\n");
	int maxPlayers;
	config->getValue(ConfigSettings::str_max_client, maxPlayers);
}

GameServer::~GameServer() {
}

bool GameServer::acceptNewClient() {
    if (network->acceptNewClient(client_id)) {
        printf("<Server> Client %d has connected to the server\n", client_id);

        client_id++;

        return true;
    }

    return false;
}

void GameServer::receive() {
    for (auto it : network->sessions) {
        int len = network->receiveData(it.first, network_data);

        if (len <= 0) {
            continue;
        }

        EmptyEvent emptyEvent;
        PlayerInputEvent playerInputEvent;

        unsigned int i = 0;
        while (i < (unsigned int)len) {
            emptyEvent.deserialize(&network_data[i]);

            switch (emptyEvent.getOpcode()) {
            case EventOpcode::PLAYER_INPUT:
                playerInputEvent.deserialize(&network_data[i]);
                eventHandlerLookup->find(emptyEvent.getOpcode())->handle(playerInputEvent);
                break;
            default:
                printf("<Server> error in packet types\n");
                break;
            }

            i += emptyEvent.getByteSize();
        }
    }
}

void GameServer::sendActionPackets() {
    // send action packet
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
    packet.packet_type = ASSIGN_EVENT;

    packet.serialize(packet_data);

    network->sendToAll(packet_data, packet_size);
}

void GameServer::sendGameStatePackets(Game *game) {

	for (ServerPlayer* player : game->players) {
		sendPlayerPositionEvent(player);
	}
	//send position, send input
}

void GameServer::sendAssignPackets(int client_id) {
	const unsigned int packet_size = sizeof(AssignEvent);
	char packet_data[packet_size];

	AssignEvent packet;
	packet.packet_type = ASSIGN_EVENT;
	packet.client_id = client_id;
	packet.serialize(packet_data);

	network->sendToOneClient(packet_data,packet_size,client_id);
}


void GameServer::sendPlayerSpawnEvent(ServerPlayer* serverPlayer) {

	PlayerSpawnEvent playerSpawnEvent(serverPlayer->getId(), serverPlayer->getX(), serverPlayer->getY(), serverPlayer->getZ());

	const unsigned int packet_size = sizeof(PlayerSpawnEvent);
	char packet_data[packet_size];

	playerSpawnEvent.serialize(packet_data);
	network->sendToAll(packet_data, packet_size);
}

void GameServer::sendPlayerPositionEvent(ServerPlayer* serverPlayer) {
	PositionEvent positionEvent(serverPlayer->getId(), serverPlayer->getX(), serverPlayer->getY(), serverPlayer->getZ());

	const unsigned int packet_size = sizeof(PositionEvent);
	char packet_data[packet_size];

	positionEvent.serialize(packet_data);
	network->sendToAll(packet_data, packet_size);

}
/*
void GameServer::sendPlayerInputEvent(ServerPlayer* serverPlayer, MoveEvent * moveEvent) {

	PlayerInputEvent

}*/