#include "Game.h"

#include "PhysicsComponent.h"

Game::Game(ConfigSettings *config)
    : config(config),
    playerFactory(&entityManager) {
    server = new GameServer(config);

    broadphase = new btDbvtBroadphase();
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    solver = new btSequentialImpulseConstraintSolver;

    world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

    world->setGravity(btVector3(0, 0, -1)); // TODO: Extract gravity constant
}

Game::~Game() {
    delete server;

    delete broadphase;
    delete collisionConfiguration;
    delete dispatcher;
    delete solver;
}

void Game::loadMap() {
    // TODO: Handle memory leak
    btCollisionShape *groundShape = new btStaticPlaneShape(btVector3(0, 0, 1), 1);
    btDefaultMotionState *groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, -1)));

    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
    btRigidBody *groundRigidBody = new btRigidBody(groundRigidBodyCI);

    world->addRigidBody(groundRigidBody);
}

void Game::addPhysicsEntity(Entity *entity) {
    PhysicsComponent *physicsCom = entity->getComponent<PhysicsComponent>();

    if (physicsCom != nullptr) {
        world->addRigidBody(physicsCom->getRigidBody());
    }
}

void Game::update(float timeStep) {
    if (server->acceptNewClient()) {
        ServerPlayer *player = playerFactory.createPlayer(0, 0, 5);
        players.push_back(player);
        addPhysicsEntity(player);
    }

    server->receiveFromClients(this);

    for (ServerPlayer *player : players) {
        player->getRigidBody()->activate(true);
    }

    world->stepSimulation(timeStep);
    
    for (ServerPlayer *player : players) {
        const btVector3 &position = player->getRigidBody()->getWorldTransform().getOrigin();
        player->setPosition(position.getX(), position.getY(), position.getZ());
    }

    server->sendGameStatePackets(this);
}

const EntityManager &Game::getEntityManager() const {
    return entityManager;
}
