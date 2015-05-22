#include "GameServer.h"

#include <components/HealthComponent.h>
#include <components/InventoryComponent.h>
#include <components/PositionComponent.h>
#include <components/RotationComponent.h>
#include <components/PlayerStatusComponent.h>
#include <core/Entity.h>
#include <core/GameMode.h>
#include <core/Player.h>
#include <network/AmmoAmountEvent.h>
#include <network/AssignEvent.h>
#include <network/BindEvent.h>
#include <network/ConnectEvent.h>
#include <network/DestroyEvent.h>
#include <network/DisconnectEvent.h>
#include <network/EmptyEvent.h>
#include <network/EquipEvent.h>
#include <network/ExplosionEvent.h>
#include <network/HealthEvent.h>
#include <network/MatchStateEvent.h>
#include <network/NetworkData.h>
#include <network/PlayerInputEvent.h>
#include <network/PlayerStatusEvent.h>
#include <network/PositionEvent.h>
#include <network/ReloadRequestEvent.h>
#include <network/RotationEvent.h>
#include <network/ScoreEvent.h>
#include <network/SpawnEvent.h>
#include <util/ConfigSettings.h>

#include "ServerEventHandlerLookup.h"
#include "ServerNetwork.h"

GameServer::GameServer(ConfigSettings *config, const ServerEventHandlerLookup &eventHandlerLookup)
        : eventHandlerLookup(eventHandlerLookup) {
    printf("<Server> Creating a Network Server\n");
    network = new ServerNetwork(config);

    printf("<Sevrer> Initializing a Game\n");
}

bool GameServer::acceptClient(unsigned int &playerId) {
    return network->acceptClient(playerId);
}

void GameServer::receive(const IdToPlayerMap &players) {
    static char networkData[MAX_PACKET_SIZE];

    for (auto kv : players) {
        const auto player = kv.second;

        int len = network->receive(player->getId(), networkData);

        if (len <= 0) {
            continue;
        }

        EmptyEvent emptyEvent;
        PlayerInputEvent playerInputEvent;
        ReloadRequestEvent reloadRequestEvent;
        EquipEvent equipEvent;

        std::unordered_set<unsigned int> receivedOpcodes;

        unsigned int i = 0;
        while (i < (unsigned int)len) {
            emptyEvent.deserialize(&networkData[i]);
            receivedOpcodes.insert(emptyEvent.getOpcode());

            switch (emptyEvent.getOpcode()) {
                case EVENT_PLAYER_INPUT: {
                    playerInputEvent.deserialize(&networkData[i]);
                    playerInputEvent.setPlayerId(player->getId());
                    break;
                }
                case EVENT_EQUIP: {
                    equipEvent.deserialize(&networkData[i]);
                    if (player->getEntity() != nullptr) {
                        equipEvent.setEntityId(player->getEntity()->getId());
                    }
                    break;
                }
                case EVENT_RELOAD_REQUEST: {
                    reloadRequestEvent.deserialize(&networkData[i]);
                    break;
                }
                default: {
                    printf("<Server> error in packet types\n");
                    break;
                }
            }

            i += emptyEvent.getByteSize();
		}

        if (receivedOpcodes.count(EVENT_PLAYER_INPUT)) {
            eventHandlerLookup.find(EVENT_PLAYER_INPUT)->handle(playerInputEvent);
        }

        if (receivedOpcodes.count(EVENT_EQUIP)) {
            eventHandlerLookup.find(EVENT_EQUIP)->handle(equipEvent);
        }

        if (receivedOpcodes.count(EVENT_RELOAD_REQUEST)) {
            eventHandlerLookup.find(EVENT_RELOAD_REQUEST)->handle(reloadRequestEvent);
        }
    }

    auto disconnectedPlayerIds = network->getDisconnectedPlayerIds();

    for (auto id : disconnectedPlayerIds) {
		DisconnectEvent disconnectEvent(id);
		eventHandlerLookup.find(disconnectEvent.getOpcode())->handle(disconnectEvent);
	}

    network->removeDisconnectedPlayers();
}

void GameServer::sendEvent(const Event &evt) const {
    const int size = evt.getByteSize();
    char *data = new char[size];

    evt.serialize(data);
    network->send(data, size);

    delete[] data;
}

void GameServer::sendEvent(const Event &evt, unsigned int playerId) const {
    const int size = evt.getByteSize();
    char *data = new char[size];

    evt.serialize(data);
    network->send(data, size, playerId);

    delete[] data;
}

void GameServer::sendConnectEvent(Player *player) const {
    ConnectEvent evt(player->getId());
    sendEvent(evt);
}

void GameServer::sendDisconnectEvent(Player *player) const {
    DisconnectEvent evt(player->getId());
    sendEvent(evt);
}

void GameServer::sendAssignEvent(Player *player) const {
    AssignEvent evt(player->getId());
    sendEvent(evt, player->getId());
}

void GameServer::sendBindEvent(Player *player) const {
    BindEvent evt(player->getId(), player->getEntity()->getId());
    sendEvent(evt);
}

void GameServer::sendMatchStateEvent(const GameMode &gameMode) const {
    MatchStateEvent evt(gameMode.getMatchState(), gameMode.getTimer());
    sendEvent(evt);
}

void GameServer::sendScoreEvent(Player *player) const {
    ScoreEvent evt(player->getId(), player->getKills(), player->getDeaths());
    sendEvent(evt);
}

void GameServer::sendSpawnEvent(Entity *entity) const {
    auto posComp = entity->getComponent<PositionComponent>();
    auto rotComp = entity->getComponent<RotationComponent>();

    SpawnEvent evt(
            entity->getId(),
            entity->getType(),
            posComp->getPosition(),
            rotComp->getRotation());
    sendEvent(evt);
}

void GameServer::sendDestroyEvent(Entity *entity) const {
    DestroyEvent evt(entity->getId());
    sendEvent(evt);
}

void GameServer::sendExplosionEvent(Entity *entity) const {
    ExplosionEvent evt(entity->getId());
    sendEvent(evt);
}

void GameServer::sendPositionEvent(Entity *entity) const {
    auto posComp = entity->getComponent<PositionComponent>();

    if (posComp == nullptr) {
        return;
    }

    PositionEvent evt(entity->getId(), posComp->getPosition());
    sendEvent(evt);
}

void GameServer::sendRotationEvent(Entity *entity) const {
    auto rotComp = entity->getComponent<RotationComponent>();

    if (rotComp == nullptr) {
        return;
    }

    RotationEvent evt(entity->getId(), rotComp->getRotation());
    sendEvent(evt);
}

void GameServer::sendHealthEvent(Player *player) const {
    auto healthComp = player->getEntity()->getComponent<HealthComponent>();

    if (healthComp == nullptr) {
        return;
    }

    HealthEvent evt(healthComp->getAmount());
    sendEvent(evt, player->getId());
}

void GameServer::sendAmmoEvent(Player *player) const {
    auto invComp = player->getEntity()->getComponent<InventoryComponent>();

    if (invComp == nullptr) {
		return;
	}

	//Hardcode the amount of bomb
	//when we add more bomb type, we need to modify this
    int kaboom_ammo = invComp->getAmount(KABOOM_V2);
    int time_ammo = invComp->getAmount(TIME_BOMB);
    int remote_ammo = invComp->getAmount(REMOTE_DETONATOR);

	//make sure its 0 or greater
	kaboom_ammo = (kaboom_ammo > 0) ? kaboom_ammo : 0;
	time_ammo = (time_ammo > 0) ? time_ammo : 0;
	remote_ammo = (remote_ammo > 0) ? remote_ammo : 0;

    AmmoAmountEvent evt(kaboom_ammo, time_ammo, remote_ammo);
    sendEvent(evt, player->getId());
}

void GameServer::sendPlayerStatusEvent(Player *player) const {
    PlayerStatusComponent *playerStatusComp = player->getEntity()->getComponent<PlayerStatusComponent>();

    if (playerStatusComp == nullptr) {
        return;
    }

    PlayerStatusEvent evt(
            playerStatusComp->checkIsKnockBacked(),
            playerStatusComp->checkIsStaggered(),
            playerStatusComp->checkIsDamaged(),
            playerStatusComp->getIsAlive());
    sendEvent(evt, player->getId());
}

void GameServer::sendInitializeEvent(Player *newPlayer, const IdToPlayerMap &players, const std::vector<Entity *> &entities) const {
    for (auto entity : entities) {
        if (entity->getId() == newPlayer->getEntity()->getId()) {
            continue;
        }

        PositionComponent *posComp = entity->getComponent<PositionComponent>();
        RotationComponent *rotComp = entity->getComponent<RotationComponent>();

        SpawnEvent evt(
                entity->getId(),
                entity->getType(),
                posComp->getPosition(),
                rotComp->getRotation());
        sendEvent(evt, newPlayer->getId());
    }

    for (auto kv : players) {
        const auto player = kv.second;
        ScoreEvent evt(player->getId(), player->getKills(), player->getDeaths());
        sendEvent(evt, newPlayer->getId());
    }
}

void GameServer::sendGameStatePackets(Player *player, const std::vector<Entity *> &entities) const {
    for (auto entity : entities) {
        sendPositionEvent(entity);
        sendRotationEvent(entity);
    }

    sendHealthEvent(player);
    sendAmmoEvent(player);
    sendPlayerStatusEvent(player);
}

void GameServer::sendGameStatePackets(const IdToPlayerMap &players, const std::vector<Entity *> &entities) const {
    for (auto entity : entities) {
        sendPositionEvent(entity);
        sendRotationEvent(entity);
    }

    for (auto kv : players) {
        const auto player = kv.second;
        sendHealthEvent(player);
        sendAmmoEvent(player);
        sendPlayerStatusEvent(player);
    }
}
