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
    rotComp->setRotation(evt.getRotation());

    Quat rot = rotComp->getRotation();
    osg::Quat quat(rot.x, rot.y, rot.z, rot.w);

    GeometryObject *geometryObj = game->getGeometryManager()->getGeometryObject(std::to_string(entity->getId()));
    geometryObj->setRotation(quat);
}
