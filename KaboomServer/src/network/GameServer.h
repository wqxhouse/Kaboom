#pragma once

#include <unordered_map>
#include <core/DeathmatchMode.h>

class ConfigSettings;
class Entity;
class Event;
class Game;
class Player;
class ServerEventHandlerLookup;
class ServerNetwork;
class GameMode;

class GameServer {
public:
    typedef std::unordered_map<unsigned int, Player *> IdToPlayerMap;

    GameServer(ConfigSettings *config, const ServerEventHandlerLookup &eventHandlerLookup);

    bool acceptClient(unsigned int &playerId);

    void receive(const IdToPlayerMap &players);

    void sendEvent(const Event &evt) const;
    void sendEvent(const Event &evt, unsigned int playerId) const;

    // Control events

    void sendConnectEvent(Player *player) const;
    void sendConnectEvent(Player *player, unsigned int receiverId) const;
    void sendDisconnectEvent(Player *player) const;
	void sendAssignEvent(Player *player) const;
    void sendBindEvent(Player *player) const;
    void sendMatchStateEvent(const GameMode &gameMode) const;

    // One-time events

    void sendChatEvent(const std::string &message) const;
    void sendScoreEvent(Player *player) const;
    void sendPlayerRenameEvent(Player *player) const;
    void sendPlayerRespawnEvent(Player *player) const;
    void sendPlayerDeathEvent(Player *player) const;
    void sendSpawnEvent(Entity *entity) const;
    void sendSpawnEvent(Entity *entity, unsigned int receiverId) const;
    void sendDestroyEvent(Entity *entity) const;
    void sendExplosionEvent(Entity *entity) const;

    // Game state events

    void sendPositionEvent(Entity *entity) const;
    void sendRotationEvent(Entity *entity) const;
    void sendHealthEvent(Player *player) const;
    void sendAmmoEvent(Player *player) const;
    void sendPlayerStatusEvent(Entity *entity) const;

    void sendNewPlayerEvent(Player *newPlayer, const IdToPlayerMap &players, DeathmatchMode &gameMode) const;
    void sendNewPlayerEnterWorldEvent(
            Player *newPlayer,
            const IdToPlayerMap &players,
            const std::vector<Entity *> &entities) const;
    void sendGameStatePackets(Player *player, const std::vector<Entity *> &entities) const;
    void sendGameStatePackets(const IdToPlayerMap &players, const std::vector<Entity *> &entities) const;

private:
    const ServerEventHandlerLookup &eventHandlerLookup;

    ServerNetwork *network;
};