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

void Game::addEntity(Entity *entity) {
    PhysicsComponent *physicsCom = entity->getComponent<PhysicsComponent>();

    if (physicsCom != nullptr) {
        world->addRigidBody(physicsCom->getRigidBody());
    }
}

void Game::update(float timeStep) {
    if (server->acceptNewClient()) {
        ServerPlayer *player = playerFactory.createPlayer(0, 0, 5);
        players.push_back(player);  
        world->addRigidBody(player->getRigidBody());
    }

    server->receiveFromClients(this);

    if (players.size() > 0) {
        players[0]->getRigidBody()->activate(true);
    }

    if (players.size() > 1) {
        players[1]->getRigidBody()->activate(true);
    }

    world->stepSimulation(timeStep);
    
    for (auto it : players) {
        btRigidBody *rigidBody = it->getRigidBody();

        btTransform transform;
        rigidBody->getMotionState()->getWorldTransform(transform);

        const btVector3 &position = transform.getOrigin();
        
        it->setX(position.getX());
        it->setY(position.getY());
        it->setZ(position.getZ());
    }

    if (players.size() > 0) {
        printf("<Server> Player 1 velocity2: %.2f, %.2f, %.2f, damping: %.2f\n",
            players[0]->getVelocityX(),
            players[0]->getVelocityY(),
            players[0]->getVelocityZ(),
            players[0]->getRigidBody()->getLinearDamping());
    }

    server->sendGameStatePackets(this);
}

const EntityManager &Game::getEntityManager() const {
    return entityManager;
}
