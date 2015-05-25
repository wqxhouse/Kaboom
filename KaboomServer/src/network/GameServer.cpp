#include "GameServer.h"

#include <components/HealthComponent.h>
#include <components/InventoryComponent.h>
#include <components/PlayerComponent.h>
#include <components/PositionComponent.h>
#include <components/RotationComponent.h>
#include <components/PlayerStatusComponent.h>
#include <components/WeaponPickupComponent.h>
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
#include <network/PlayerDeathEvent.h>
#include <network/PlayerInputEvent.h>
#include <network/PlayerRenameEvent.h>
#include <network/PlayerRespawnEvent.h>
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
        PlayerRenameEvent playerRenameEvent;
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
                case EVENT_PLAYER_RENAME: {
                    playerRenameEvent.deserialize(&networkData[i]);
                    playerRenameEvent.setPlayerId(player->getId());
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

        if (receivedOpcodes.count(EVENT_PLAYER_RENAME)) {
            eventHandlerLookup.find(EVENT_PLAYER_RENAME)->handle(playerRenameEvent);
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

void GameServer::sendConnectEvent(Player *player, unsigned int receiverId) const {
    ConnectEvent evt(player->getId());
    sendEvent(evt, receiverId);
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

void GameServer::sendPlayerRenameEvent(Player *player) const {
    PlayerRenameEvent evt(player->getId(), player->getName());
    sendEvent(evt);
}

void GameServer::sendPlayerRespawnEvent(Player *player) const {
    PlayerRespawnEvent evt;
    sendEvent(evt);
}

void GameServer::sendPlayerDeathEvent(Player *player) const {
    PlayerDeathEvent evt;
    sendEvent(evt);
}

void GameServer::sendSpawnEvent(Entity *entity) const {
    auto posComp = entity->getComponent<PositionComponent>();
    auto rotComp = entity->getComponent<RotationComponent>();
    auto weaponPickupComp = entity->getComponent<WeaponPickupComponent>();

    const bool pickup = weaponPickupComp != nullptr;

    SpawnEvent evt(
            entity->getId(),
            entity->getType(),
            pickup,
            posComp->getPosition(),
            rotComp->getRotation());
    sendEvent(evt);
}

void GameServer::sendSpawnEvent(Entity *entity, unsigned int receiverId) const {
    auto posComp = entity->getComponent<PositionComponent>();
    auto rotComp = entity->getComponent<RotationComponent>();
    auto weaponPickupComp = entity->getComponent<WeaponPickupComponent>();

    const bool pickup = weaponPickupComp != nullptr;

    SpawnEvent evt(
            entity->getId(),
            entity->getType(),
            pickup,
            posComp->getPosition(),
            rotComp->getRotation());
    sendEvent(evt, receiverId);
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

void GameServer::sendNewPlayerEvent(Player *newPlayer, const IdToPlayerMap &players) const {
    sendConnectEvent(newPlayer); // Tells everyone about the new player's player ID
    sendAssignEvent(newPlayer); // Tells this player its player ID

    // Tells the new player about everyone else's player ID
    for (auto kv : players) {
        const auto playerId = kv.first;
        const auto player = kv.second;

        if (playerId == newPlayer->getId()) {
            continue;
        }

        sendConnectEvent(player, newPlayer->getId());
    }
}

void GameServer::sendNewPlayerEnterWorldEvent(
        Player *newPlayer,
        const IdToPlayerMap &players,
        const std::vector<Entity *> &entities) const {
    sendBindEvent(newPlayer); // Tells everyone about the new player's entity ID
    sendScoreEvent(newPlayer); // Tells everyone about the new player's score

    // Tells the new player about everyone else's entity ID and score
    for (auto kv : players) {
        const auto player = kv.second;

        if (player->getId() == newPlayer->getId()) {
            continue;
        }

        BindEvent bindEvent(player->getId(), player->getEntity()->getId());
        sendEvent(bindEvent, newPlayer->getId());

        ScoreEvent scoreEvent(player->getId(), player->getKills(), player->getDeaths());
        sendEvent(scoreEvent, newPlayer->getId());
    }

    // Tells the new player about every entity's entity ID (except itself)
    for (auto entity : entities) {
        if (entity->getId() != newPlayer->getEntity()->getId()) {
            sendSpawnEvent(entity, newPlayer->getId());
        }
    }

    // Tells the new player about the current game state
    sendGameStatePackets(newPlayer, entities);
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
