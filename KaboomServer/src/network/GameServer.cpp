#include "GameServer.h"

#include <components/BombContainerComponent.h>
#include <components/HealthComponent.h>
#include <components/PositionComponent.h>
#include <components/RotationComponent.h>
#include <components/PlayerStatusComponent.h>

#include <core/Entity.h>
#include <core/Player.h>
#include <network/AssignEvent.h>
#include <network/DestroyEvent.h>
#include <network/DisconnectEvent.h>
#include <network/EmptyEvent.h>
#include <network/EquipEvent.h>
#include <network/ExplosionEvent.h>
#include <network/HealthEvent.h>
#include <network/NetworkData.h>
#include <network/PlayerInputEvent.h>
#include <network/PlayerStatusEvent.h>
#include <network/PositionEvent.h>
#include <network/RotationEvent.h>
#include <network/ScoreEvent.h>
#include <network/SpawnEvent.h>
#include <network/AmmoAmountEvent.h>

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
    static char networkData[MAX_PACKET_SIZE];

    for (auto it : network->sessions) {
        if (network->sessions.empty()) {
            break;
        }

        int client_id = it.first;
        int len = network->receive(it.first, networkData);

        if (len <= 0) {
            continue;
        }

        EmptyEvent emptyEvent;
        DisconnectEvent disconnectEvent;
        PlayerInputEvent playerInputEvent;
        EquipEvent equipEvent;

        std::unordered_set<unsigned int> receivedOpcodes;

        unsigned int i = 0;
        while (i < (unsigned int)len) {
            emptyEvent.deserialize(&networkData[i]);
            receivedOpcodes.insert(emptyEvent.getOpcode());

            switch (emptyEvent.getOpcode()) {
                case EVENT_DISCONNECT: {
                    disconnectEvent.deserialize(&networkData[i]);
					disconnectEvent.setPlayerId(clientIdToEntityId[client_id]);
                    break;
                }
                case EVENT_PLAYER_INPUT: {
                    playerInputEvent.deserialize(&networkData[i]);
                    playerInputEvent.setPlayerId(clientIdToEntityId[client_id]);
                    break;
                }
                case EVENT_EQUIP: {
                    equipEvent.deserialize(&networkData[i]);
                    equipEvent.setEntityId(clientIdToEntityId[client_id]);
                    break;
                }
                default: {
                    printf("<Server> error in packet types\n");
                    break;
                }
            }

            i += emptyEvent.getByteSize();
		}

        if (receivedOpcodes.count(EVENT_DISCONNECT)) {
            eventHandlerLookup.find(EVENT_DISCONNECT)->handle(disconnectEvent);
        }

        if (receivedOpcodes.count(EVENT_PLAYER_INPUT)) {
            eventHandlerLookup.find(EVENT_PLAYER_INPUT)->handle(playerInputEvent);
        }

        if (receivedOpcodes.count(EVENT_EQUIP)) {
            eventHandlerLookup.find(EVENT_EQUIP)->handle(equipEvent);
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
	BombContainerComponent *bombConCom = entity->getComponent<BombContainerComponent>();

	if (bombConCom == nullptr){
		return;
	}

	//Hardcode the amount of bomb
	//when we add more bomb type, we need to modify this
	int kaboom_ammo = bombConCom->getAmount(KABOOM_V2);
	int time_ammo = bombConCom->getAmount(TIME_BOMB);
	int remote_ammo = bombConCom->getAmount(REMOTE_DETONATOR);

	//make sure its 0 or greater
	kaboom_ammo = (kaboom_ammo > 0) ? kaboom_ammo : 0;
	time_ammo = (time_ammo > 0) ? time_ammo : 0;
	remote_ammo = (remote_ammo > 0) ? remote_ammo : 0;

	AmmoAmountEvent ammoAmountEvent(kaboom_ammo, time_ammo, remote_ammo);
	sendEvent(ammoAmountEvent, entityIdToClientId.at(entity->getId()));
}

void GameServer::sendScoreEvent(Player *player) const {
    ScoreEvent scoreEvent(player->getId(), player->getKills(), player->getDeaths());
	sendEvent(scoreEvent);
}

void GameServer::sendPlayerStatusEvent(Entity *entity) const{
	PlayerStatusComponent *playerStatusComp = entity->getComponent<PlayerStatusComponent>();

	if (playerStatusComp == nullptr){
		return;
	}

	PlayerStatusEvent playerStatusEvent(playerStatusComp->checkIsKnockBacked(), playerStatusComp->checkIsStaggered(), playerStatusComp->checkIsDamaged(), playerStatusComp->getIsAlive());
	sendEvent(playerStatusEvent, entityIdToClientId.at(entity->getId()));

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
		sendPlayerStatusEvent(entity);
		sendAmmoEvent(entity);
    }
}
