#include "GameServer.h"

#include <core/PositionComponent.h>
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

bool GameServer::acceptNewClient(unsigned int entity_id) {
    if (network->acceptNewClient(entity_id)) {
        printf("<Server> Client %d has connected to the server\n", entity_id);

        //client_id++;

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

				playerInputEvent.setPlayerId(it.first); //Important prevent hacking from client impersonating as other clients

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

void GameServer::sendGameStatePackets(Game *game) {
    for (Entity *player : game->players) {
        sendPositionEvent(player);
    }
}

void GameServer::sendPlayerSpawnEvent(Entity* newEntity, std::vector<Entity *> existingEnities) {

	//send the spawn entity event to all the clients
	PositionComponent *positionCom = newEntity->getComponent<PositionComponent>();

    if (positionCom == nullptr) {
        return;
    }

	PlayerSpawnEvent playerSpawnEvent(newEntity->getId(), positionCom->getX(), positionCom->getY(), positionCom->getZ());

    const unsigned int packet_size = sizeof(PlayerSpawnEvent);
    char packet_data[packet_size];

    playerSpawnEvent.serialize(packet_data);
    network->sendToAll(packet_data, packet_size);

	//Send to the client who just joined with the existing entity we already have

	for (int i = 0; i < existingEnities.size(); i++){
		if (existingEnities[i]->getId() == newEntity->getId()){
			continue;
		}
		positionCom = existingEnities[i]->getComponent<PositionComponent>();

		PlayerSpawnEvent extraPlayers(existingEnities[i]->getId(), positionCom->getX(), positionCom->getY(), positionCom->getZ());
		extraPlayers.serialize(packet_data);
		network->sendToOneClient(packet_data, packet_size, newEntity->getId());
	}
}

void GameServer::sendPositionEvent(Entity* entity) {
    PositionComponent *positionCom = entity->getComponent<PositionComponent>();

    if (positionCom == nullptr) {
        return;
    }

    PositionEvent positionEvent(entity->getId(), positionCom->getX(), positionCom->getY(), positionCom->getZ());

    const unsigned int packet_size = sizeof(PositionEvent);
    char packet_data[packet_size];

    positionEvent.serialize(packet_data);
    network->sendToAll(packet_data, packet_size);
}
