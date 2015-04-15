#include "PositionEventHandler.h"

#include <core/PositionComponent.h>

PositionEventHandler::PositionEventHandler(Game *game)
    : game(game) {
}

PositionEventHandler::~PositionEventHandler() {
}

void PositionEventHandler::handle(const PositionEvent &evt) const {
    Entity *entity = game->getEntityManager().getEntity(evt.getEntityId());

    PositionComponent *positionCom = entity->getComponent<PositionComponent>();

    if (positionCom != nullptr) {
        positionCom->setX(evt.getX());
        positionCom->setY(evt.getY());
        positionCom->setZ(evt.getZ());
    }
}
