#include "GameServer.h"

#include <core/Entity.h>
#include <core/PositionComponent.h>
#include <core/RotationComponent.h>
#include <network/AssignEvent.h>
#include <network/DisconnectEvent.h>
#include <network/EmptyEvent.h>
#include <network/ExplosionEvent.h>
#include <network/PlayerInputEvent.h>
#include <network/PositionEvent.h>
#include <network/RotationEvent.h>
#include <network/SpawnEvent.h>

#include "../core/Game.h"

GameServer::GameServer(ConfigSettings * config, const ServerEventHandlerLookup &eventHandlerLookup)
        : eventHandlerLookup(eventHandlerLookup),
	      nextClientId(0),
	      currClientId(0) {
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
	if (network->acceptNewClient(nextClientId)) {
		printf("<Server> Client %d has connected to the server, with entitiy id %d\n", nextClientId, entity_id);
		clientIdToEntityId[nextClientId] = entity_id;
		currClientId = nextClientId++;
        return true;
    }

    return false;
}

void GameServer::receive(Game *game) {
    char network_data[MAX_PACKET_SIZE];

    for (auto it : network->sessions) {
        if (network->sessions.empty()) {
            break;
        }

        int client_id = it.first;
        int len = network->receive(it.first, network_data);

        if (len <= 0) {
            continue;
        }

        bool receivedPlayerInputEvent = false;

        unsigned int i = 0;
        while (i < (unsigned int)len) {
            EmptyEvent emptyEvent;
            emptyEvent.deserialize(&network_data[i]);

            switch (emptyEvent.getOpcode()) {
                case EventOpcode::DISCONNECT: {
                    DisconnectEvent disconnectEvent;
                    disconnectEvent.deserialize(&network_data[i]);
					disconnectEvent.setPlayerId(clientIdToEntityId[client_id]); // Prevent hacking from client impersonating as other clients

                    eventHandlerLookup.find(emptyEvent.getOpcode())->handle(disconnectEvent);
                }
                case EventOpcode::PLAYER_INPUT: {
                    PlayerInputEvent playerInputEvent;
                    playerInputEvent.deserialize(&network_data[i]);
                    playerInputEvent.setPlayerId(clientIdToEntityId[client_id]); // Prevent hacking from client impersonating as other clients

                    if (!receivedPlayerInputEvent) {
                        eventHandlerLookup.find(emptyEvent.getOpcode())->handle(playerInputEvent);
                        receivedPlayerInputEvent = true;
                    }

                    break;
                }
                default: {
                    printf("<Server> error in packet types\n");
                    break;
                }
            }

            i += emptyEvent.getByteSize();
        }
    }

	for (auto id : network->disconnectedClients){
		DisconnectEvent disconnectEvent(clientIdToEntityId[id]);
		eventHandlerLookup.find(disconnectEvent.getOpcode())->handle(disconnectEvent);
		clientIdToEntityId.erase(id);
	}
    network->removeDisconnectedClients();
}

void GameServer::sendEvent(const Event &evt) const {
    const int size = evt.getByteSize();
    char *data = new char[size];

    evt.serialize(data);
    network->send(data, size);

    delete[] data;
}

void GameServer::sendEvent(const Event &evt, const unsigned int &clientId) const {
    const int size = evt.getByteSize();
    char *data = new char[size];

    evt.serialize(data);
    network->send(data, size, clientId);

    delete[] data;
}

void GameServer::sendAssignEvent(const unsigned int &entityId) const {
    AssignEvent assignEvent(entityId);
	sendEvent(assignEvent, currClientId);
}

void GameServer::sendInitializeEvent(Entity *player, const std::vector<Entity *> &entities) const {
    for (auto entity : entities) {
        if (entity->getId() == player->getId()) {
            continue;
        }

        PositionComponent *posComp = entity->getComponent<PositionComponent>();

        if (posComp == nullptr) {
            return;
        }

        SpawnEvent spawnEvent(
            entity->getId(),
            posComp->getX(),
            posComp->getY(),
            posComp->getZ(),
            entity->getType(),
            0);
        sendEvent(spawnEvent, currClientId);
    }
}

void GameServer::sendDisconnectEvent(Entity *entity) const {
    DisconnectEvent disconnectEvent(entity->getId());
    sendEvent(disconnectEvent);
}

void GameServer::sendGameStatePackets(const std::vector<Entity *> &entities) const {
    for (Entity *entity : entities) {
        sendPositionEvent(entity);
        sendRotationEvent(entity);
    }
}

void GameServer::sendPositionEvent(Entity *entity) const {
    PositionComponent *posComp = entity->getComponent<PositionComponent>();

    if (posComp == nullptr) {
        return;
    }

    PositionEvent posEvent(entity->getId(), posComp->getX(), posComp->getY(), posComp->getZ());
    sendEvent(posEvent);
}

void GameServer::sendRotationEvent(Entity *entity) const {
    RotationComponent *rotComp = entity->getComponent<RotationComponent>();

    if (rotComp == nullptr) {
        return;
    }

    RotationEvent rotEvent(entity->getId(), rotComp->getYaw(), rotComp->getPitch());
    sendEvent(rotEvent);
}

void GameServer::sendSpawnEvent(Entity *entity) const {
    PositionComponent *posComp = entity->getComponent<PositionComponent>();

    if (posComp == nullptr) {
        return;
    }

    SpawnEvent spawnEvent(entity->getId(), posComp->getX(), posComp->getY(), posComp->getZ(), entity->getType(), 0);
    sendEvent(spawnEvent);
}

void GameServer::sendExplosionEvent(Entity *bomb) const {
    ExplosionEvent expEvent(bomb->getId());
    sendEvent(expEvent);
}
