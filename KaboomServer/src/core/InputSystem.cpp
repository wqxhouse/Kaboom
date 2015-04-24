#include "InputSystem.h"

#include <btBulletDynamicsCommon.h>

#include <core/EntityManager.h>
#include <core/PositionComponent.h>

#include "Game.h"
#include "InputComponent.h"
#include "PhysicsComponent.h"

InputSystem::InputSystem(Game *game)
        : EntitySystem(game) {
}

InputSystem::~InputSystem() {
}

void InputSystem::update(float timeStep) {
    for (auto entity : game->getEntityManager().getEntityList()) {
        InputComponent *inputCom = entity->getComponent<InputComponent>();
        PositionComponent *posCom = entity->getComponent<PositionComponent>();
        PhysicsComponent *physCom = entity->getComponent<PhysicsComponent>();

        if (inputCom == nullptr || physCom == nullptr || physCom == nullptr) {
            continue;
        }

        btRigidBody *rigidBody = physCom->getRigidBody();
        btVector3 velocity = rigidBody->getLinearVelocity();

        if (inputCom->isMovingForward()) {
            velocity.setY(1);
        } else if (inputCom->isMovingBackward()) {
            velocity.setY(-1);
        } else {
            velocity.setY(0);
        }

        if (inputCom->isMovingLeft()) {
            velocity.setX(-1);
        } else if (inputCom->isMovingRight()) {
            velocity.setX(1);
        } else {
            velocity.setX(0);
        }

        rigidBody->setLinearVelocity(velocity);

        if (inputCom->isFiring()) {
            Entity *bomb = game->getBombFactory().createBomb(posCom->getX(), posCom->getY(), posCom->getZ() + 5);
            game->addPhysicsEntity(bomb);
            game->getGameServer().sendEntitySpawnEvent(bomb);
        }
    }
}
