#pragma once

#include <network/NetworkData.h>
#include <core/BombContainerComponent.h>
#include "ServerEventHandlerLookup.h"
#include "ServerNetwork.h"
#include "NetworkServices.h"
#include "../core/PhysicsComponent.h"

class Entity;
class Game;

class GameServer {
public:
    GameServer(ConfigSettings *, const ServerEventHandlerLookup &eventHandlerLookup);
    ~GameServer();

    bool acceptNewClient(unsigned int);

    void receive(Game *game);

    void sendEvent(const Event &evt) const;
    void sendEvent(const Event &evt, const unsigned int &clientId) const;

    void sendAssignEvent(const unsigned int &entityId) const;
    void sendInitializeEvent(Entity* player, const std::vector<Entity *> &entities) const;
    void sendDisconnectEvent(Entity *entity) const;

    void sendGameStatePackets(const std::vector<Entity *> &entities) const;
    void sendPositionEvent(Entity *entity) const;
    void sendRotationEvent(Entity *entity) const;
    void sendSpawnEvent(Entity *entity) const;
    void sendExplosionEvent(Entity *bomb) const;

private:
    const ServerEventHandlerLookup &eventHandlerLookup;

    ServerNetwork* network;
};