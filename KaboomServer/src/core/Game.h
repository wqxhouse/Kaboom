#pragma once

#include <vector>

#include <btBulletDynamicsCommon.h>

#include <ConfigSettings.h>
#include <core/EntityManager.h>

#include "PlayerFactory.h"
#include "BombFactory.h"

class GameServer;
class ServerEventHandlerLookup;

class Game {
public:
    std::vector<Entity *> players;
	std::vector<Entity *> bombs;
    Game(ConfigSettings *config);
    ~Game();

    void loadMap();
    void update(float timestep);

    void addPhysicsEntity(Entity *entity);

    const EntityManager &getEntityManager() const;
    const PlayerFactory &getPlayerFactory() const;
    const BombFactory &getBombFactory() const;
    GameServer *getGameServer() const;

private:
    ConfigSettings *config;

    EntityManager entityManager;
    PlayerFactory playerFactory;
    BombFactory bombFactory;

    GameServer *server;
    ServerEventHandlerLookup *eventHandlerLookup;

    btBroadphaseInterface *broadphase;
    btDefaultCollisionConfiguration *collisionConfiguration;
    btCollisionDispatcher *dispatcher;
    btSequentialImpulseConstraintSolver *solver;
    btDiscreteDynamicsWorld *world;
};
