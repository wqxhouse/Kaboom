#include "RotationEventHandler.h"

#include <GeometryObject.h>
#include <core/EntityManager.h>
#include <core/RotationComponent.h>
#include <network/RotationEvent.h>

#include "../core/Game.h"

RotationEventHandler::RotationEventHandler(Game *game)
        : game(game) {
}

RotationEventHandler::~RotationEventHandler() {
}

void RotationEventHandler::handle(const Event &e) const {
    const RotationEvent &evt = static_cast<const RotationEvent &>(e);

    Entity *entity = game->getEntityManager().getEntity(evt.getEntityId());

    RotationComponent *rotComp = entity->getComponent<RotationComponent>();
    rotComp->setYaw(evt.getYaw());
    rotComp->setPitch(evt.getPitch());

    GeometryObject *geometryObj = game->getGeometryManager()->getGeometryObject(std::to_string(entity->getId()));

    // TODO: Finish handling rotation
}