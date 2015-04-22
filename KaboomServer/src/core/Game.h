#pragma once

#include <vector>

#include <btBulletDynamicsCommon.h>

#include <ConfigSettings.h>
#include <core/EntityManager.h>

#include "PlayerFactory.h"
#include "BombFactory.h"

#include "../network/GameServer.h"

class GameServer;

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

	PlayerFactory playerFactory;
	BombFactory bombFactory;
	GameServer *server;

private:
    ConfigSettings *config;
    

    EntityManager entityManager;
    

    btBroadphaseInterface *broadphase;
    btDefaultCollisionConfiguration *collisionConfiguration;
    btCollisionDispatcher *dispatcher;
    btSequentialImpulseConstraintSolver *solver;
    btDiscreteDynamicsWorld *world;
};
