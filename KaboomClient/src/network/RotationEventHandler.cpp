#include "RotationEventHandler.h"

#include <GeometryObject.h>
#include <components/RotationComponent.h>
#include <core/EntityManager.h>
#include <network/RotationEvent.h>

#include "../core/Game.h"

RotationEventHandler::RotationEventHandler(Game *game)
        : game(game) {
}

void RotationEventHandler::handle(const Event &e) const {
    const RotationEvent &evt = static_cast<const RotationEvent &>(e);

    Entity *entity = game->getEntityManager().getEntity(evt.getEntityId());

    RotationComponent *rotComp = entity->getComponent<RotationComponent>();
    rotComp->setYaw(evt.getYaw());
    rotComp->setPitch(evt.getPitch());

    osg::Quat quat = game->getCamera().eulerToQuat(evt.getYaw(), 0.0f);

    GeometryObject *geometryObj = game->getGeometryManager()->getGeometryObject(std::to_string(entity->getId()));
    geometryObj->setRotation(quat);
}
