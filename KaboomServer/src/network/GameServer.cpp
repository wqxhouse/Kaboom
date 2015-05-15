#include "GameServer.h"

#include <components/BombContainerComponent.h>
#include <components/HealthComponent.h>
#include <components/PositionComponent.h>
#include <components/RotationComponent.h>
#include <core/Entity.h>
#include <network/AssignEvent.h>
#include <network/DestroyEvent.h>
#include <network/DisconnectEvent.h>
#include <network/EmptyEvent.h>
#include <network/EquipEvent.h>
#include <network/ExplosionEvent.h>
#include <network/HealthEvent.h>
#include <network/NetworkData.h>
#include <network/PlayerInputEvent.h>
#include <network/PositionEvent.h>
#include <network/RotationEvent.h>
#include <network/ScoreEvent.h>
#include <network/SpawnEvent.h>

#include "NetworkServices.h"
#include "ServerEventHandlerLookup.h"
#include "ServerNetwork.h"
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

bool GameServer::acceptNewClient(unsigned int entity_id) {
	if (network->acceptNewClient(nextClientId)) {
		printf("<Server> Client %d has connected to the server, with entitiy id %d\n", nextClientId, entity_id);
		clientIdToEntityId[nextClientId] = entity_id;
		entityIdToClientId[entity_id] = nextClientId;
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

		EmptyEvent emptyEvent;
		PlayerInputEvent playerInputEvent;
        bool receivedPlayerInputEvent = false;

        unsigned int i = 0;
        while (i < (unsigned int)len) {
            emptyEvent.deserialize(&network_data[i]);

            switch (emptyEvent.getOpcode()) {
                case EVENT_DISCONNECT: {
                    DisconnectEvent disconnectEvent;
                    disconnectEvent.deserialize(&network_data[i]);
					disconnectEvent.setPlayerId(clientIdToEntityId[client_id]); // Prevent hacking from client impersonating as other clients

                    eventHandlerLookup.find(emptyEvent.getOpcode())->handle(disconnectEvent);
                }
                case EVENT_PLAYER_INPUT: {
                    playerInputEvent.deserialize(&network_data[i]);
                    playerInputEvent.setPlayerId(clientIdToEntityId[client_id]); // Prevent hacking from client impersonating as other clients

                    receivedPlayerInputEvent = true;

                    break;
                }
                case EVENT_EQUIP: {
                    EquipEvent equipEvent;
                    equipEvent.deserialize(&network_data[i]);
                    equipEvent.setEntityId(clientIdToEntityId[client_id]); // Prevent hacking from client impersonating as other clients
                    eventHandlerLookup.find(emptyEvent.getOpcode())->handle(equipEvent);
                    break;
                }
                default: {
                    printf("<Server> error in packet types\n");
                    break;
                }
            }

            i += emptyEvent.getByteSize();
		}

		if (receivedPlayerInputEvent) {
			eventHandlerLookup.find(emptyEvent.getOpcode())->handle(playerInputEvent);
		}
    }

	for (auto id : network->disconnectedClients) {
		DisconnectEvent disconnectEvent(clientIdToEntityId[id]);
		eventHandlerLookup.find(disconnectEvent.getOpcode())->handle(disconnectEvent);
		entityIdToClientId.erase(clientIdToEntityId.at(id));
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

void GameServer::sendEvent(const Event &evt, unsigned int clientId) const {
    const int size = evt.getByteSize();
    char *data = new char[size];

    evt.serialize(data);
    network->send(data, size, clientId);

    delete[] data;
}

void GameServer::sendAssignEvent(unsigned int entityId) const {
    AssignEvent assignEvent(entityId);
	sendEvent(assignEvent, currClientId);
}

void GameServer::sendDisconnectEvent(Entity *entity) const {
    DisconnectEvent disconnectEvent(entity->getId());
    sendEvent(disconnectEvent);
}

void GameServer::sendSpawnEvent(Entity *entity) const {
    PositionComponent *posComp = entity->getComponent<PositionComponent>();
    RotationComponent *rotComp = entity->getComponent<RotationComponent>();

    SpawnEvent spawnEvent(
            entity->getId(),
            entity->getType(),
            posComp->getPosition(),
            rotComp->getRotation());
    sendEvent(spawnEvent);
}

void GameServer::sendDestroyEvent(Entity *entity) const {
    DestroyEvent evt(entity->getId());
    sendEvent(evt);
}

void GameServer::sendPositionEvent(Entity *entity) const {
    PositionComponent *posComp = entity->getComponent<PositionComponent>();

    if (posComp == nullptr) {
        return;
    }

    PositionEvent posEvent(entity->getId(), posComp->getPosition());
    sendEvent(posEvent);
}

void GameServer::sendRotationEvent(Entity *entity) const {
    RotationComponent *rotComp = entity->getComponent<RotationComponent>();

    if (rotComp == nullptr) {
        return;
    }

    RotationEvent rotEvent(entity->getId(), rotComp->getRotation());
    sendEvent(rotEvent);
}

void GameServer::sendExplosionEvent(Entity *bomb) const {
    ExplosionEvent expEvent(bomb->getId());
    sendEvent(expEvent);
}

void GameServer::sendHealthEvent(Entity *entity) const {
    HealthComponent *healthComp = entity->getComponent<HealthComponent>();

    if (healthComp == nullptr) {
        return;
    }

    HealthEvent healthEvent(healthComp->getAmount());
    sendEvent(healthEvent, entityIdToClientId.at(entity->getId()));
}

void GameServer::sendAmmoEvent(Entity *entity) const{

}

void GameServer::sendScoreEvent(int kills, int deaths) const {
    ScoreEvent scoreEvent(kills, deaths);
    sendEvent(scoreEvent);
}

void GameServer::sendInitializeEvent(Entity *player, const std::vector<Entity *> &entities) const {
    for (auto entity : entities) {
        if (entity->getId() == player->getId()) {
            continue;
        }

        PositionComponent *posComp = entity->getComponent<PositionComponent>();
        RotationComponent *rotComp = entity->getComponent<RotationComponent>();

        SpawnEvent spawnEvent(
                entity->getId(),
                entity->getType(),
                posComp->getPosition(),
                rotComp->getRotation());
        sendEvent(spawnEvent, currClientId);
    }
}

void GameServer::sendGameStatePackets(const std::vector<Entity *> &entities) const {
    for (Entity *entity : entities) {
        sendPositionEvent(entity);
        sendRotationEvent(entity);
        sendHealthEvent(entity);
    }
}
