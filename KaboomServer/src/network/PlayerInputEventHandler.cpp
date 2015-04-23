#include "PlayerInputEventHandler.h"

#include <btBulletDynamicsCommon.h>

#include <core/Entity.h>
#include <core/PositionComponent.h>
#include <network/PlayerInputEvent.h>

#include "../core/Game.h"
#include "../core/PhysicsComponent.h"
#include "../network/GameServer.h"

PlayerInputEventHandler::PlayerInputEventHandler(Game *game)
    : game(game) {
}

PlayerInputEventHandler::~PlayerInputEventHandler() {
}

void PlayerInputEventHandler::handle(const Event &e) const {
    const PlayerInputEvent &evt = static_cast<const PlayerInputEvent &>(e);

    Entity *player = game->getEntityManager().getEntity(evt.getPlayerId());
    btRigidBody *rigidBody = player->getComponent<PhysicsComponent>()->getRigidBody();
    btVector3 velocity = rigidBody->getLinearVelocity();

    if (evt.isMovingForward()) {
        velocity.setY(1);
    } else if (evt.isMovingBackward()) {
        velocity.setY(-1);
    } else {
        velocity.setY(0);
    }

    if (evt.isMovingLeft()) {
        velocity.setX(-1);
    } else if (evt.isMovingRight()) {
        velocity.setX(1);
    } else {
        velocity.setX(0);
    }

    rigidBody->setLinearVelocity(velocity);


    if (evt.isFiring()) {
        PositionComponent *posComp = player->getComponent<PositionComponent>();

        Entity *bomb = game->getBombFactory().createBomb(posComp->getX(), posComp->getY(), posComp->getZ() + 1);
        game->addPhysicsEntity(bomb);
        game->getGameServer()->sendEntitySpawnEvent(bomb);
    }
}
