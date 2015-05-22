#pragma once

#include <unordered_map>

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
    void sendDisconnectEvent(Player *player) const;
    void sendAssignEvent(Player *player) const;
    void sendBindEvent(Player *player) const;
    void sendMatchStateEvent(const GameMode &gameMode) const;

    // One-time events

    void sendScoreEvent(Player *player) const;
    void sendSpawnEvent(Entity *entity) const;
    void sendDestroyEvent(Entity *entity) const;
    void sendExplosionEvent(Entity *entity) const;

    // Game state events

    void sendPositionEvent(Entity *entity) const;
    void sendRotationEvent(Entity *entity) const;
    void sendHealthEvent(Player *player) const;
    void sendAmmoEvent(Player *player) const;
	void sendPlayerStatusEvent(Player *player) const;

    void sendInitializeEvent(Player *player, const std::vector<Entity *> &entities) const;
    void sendGameStatePackets(Player *player, const std::vector<Entity *> &entities) const;
    void sendGameStatePackets(const IdToPlayerMap &players, const std::vector<Entity *> &entities) const;

private:
    const ServerEventHandlerLookup &eventHandlerLookup;

    ServerNetwork *network;
};