#include "PlayerInputEventHandler.h"

#include <network/PlayerInputEvent.h>

#include "../core/PhysicsComponent.h"

PlayerInputEventHandler::PlayerInputEventHandler(Game *game)
    : game(game) {
}

PlayerInputEventHandler::~PlayerInputEventHandler() {
}

void PlayerInputEventHandler::handle(const Event &e) const {
    const PlayerInputEvent &evt = static_cast<const PlayerInputEvent &>(e);

    Entity *entity = game->getEntityManager().getEntity(evt.getPlayerId());
    btRigidBody *rigidBody = entity->getComponent<PhysicsComponent>()->getRigidBody();
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
}
