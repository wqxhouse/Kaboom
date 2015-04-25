#include "GameServer.h"

#include <core/Entity.h>
#include <core/CharacteristicComponent.h>
#include <core/PositionComponent.h>
#include <core/RotationComponent.h>
#include <network/EmptyEvent.h>
#include <network/PlayerInputEvent.h>
#include <network/PositionEvent.h>
#include <network/RotationEvent.h>
#include <network/SpawnEvent.h>
#include <network/AssignEvent.h>

#include "../core/Game.h"

GameServer::GameServer(ConfigSettings * config, const ServerEventHandlerLookup &eventHandlerLookup)
        : eventHandlerLookup(eventHandlerLookup) {
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

		int id = it.first;
        int len = network->receiveData(it.first, network_data);

		if (len == 0){
			
			//handle player disconnect by sending delete messages to all clients about the deleted users entities
			Entity *entity = game->getEntityManager().getEntity(id);

            // HACK
            if (entity == nullptr) {
                continue;
            }

			if (entity->getComponent<BombContainerComponent>() == nullptr) {
				printf("\nsomething wrong with look up method\n");
				continue;
			}
			DeleteEvent deleteEvent(id);
			char del[sizeof(DeleteEvent)];
			deleteEvent.serialize(del);
			network->sendToAll(del,sizeof(DeleteEvent));

			
			//TODO: Need to delete our rigid body and it's collision shape
			PhysicsComponent *phys=game->getEntityManager().getEntity(id)->getComponent<PhysicsComponent>();
			game->getWorld().removeRigidBody(phys->getRigidBody());
			delete phys->getRigidBody()->getMotionState();
			delete phys->getRigidBody();

			game->getEntityManager().destroyEntity(id);
		}

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
            case EventOpcode::PLAYER_INPUT:
                playerInputEvent.deserialize(&network_data[i]);

				playerInputEvent.setPlayerId(it.first); //Important prevent hacking from client impersonating as other clients
				receivedPlayerInputEvent = true;

                break;
            default:
                printf("<Server> error in packet types\n");
                break;
            }

            i += emptyEvent.getByteSize();
        }

		if (receivedPlayerInputEvent) { 
			eventHandlerLookup.find(emptyEvent.getOpcode())->handle(playerInputEvent);
		}
    }
}

void GameServer::sendEvent(const Event &evt) const {
    const int size = evt.getByteSize();
    char *data = new char[size];

    evt.serialize(data);
    network->sendToAll(data, size);

    delete[] data;
}

void GameServer::sendEvent(const Event &evt, const unsigned int &clientId) const {
    const int size = evt.getByteSize();
    char *data = new char[size];

    evt.serialize(data);
    network->sendToOneClient(data, size, clientId);

    delete[] data;
}

void GameServer::sendAssignEvent(const unsigned int &entityId) const {
    AssignEvent assignEvent(entityId);
    sendEvent(assignEvent, entityId);
}

void GameServer::sendInitializeEvent(Entity *player, const std::vector<Entity *> &entities) const {
    for (auto entity : entities) {
        if (entity->getId() == player->getId()) {
            continue;
        }

        PositionComponent *posComp = entity->getComponent<PositionComponent>();
        CharacteristicComponent *charComp = entity->getComponent<CharacteristicComponent>();

        if (posComp == nullptr || charComp == nullptr) {
            return;
        }

        SpawnEvent spawnEvent(
            entity->getId(),
            posComp->getX(),
            posComp->getY(),
            posComp->getZ(),
            charComp->getType(),
            0);
        sendEvent(spawnEvent, player->getId());
    }
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
    CharacteristicComponent *charComp = entity->getComponent<CharacteristicComponent>();

    if (posComp == nullptr || charComp == nullptr) {
        return;
    }

    SpawnEvent spawnEvent(entity->getId(), posComp->getX(), posComp->getY(), posComp->getZ(), charComp->getType(), 0);
    sendEvent(spawnEvent);
}
