#include "PositionEventHandler.h"

#include <osg/MatrixTransform>

#include <Core.h>
#include <GeometryObject.h>
#include <components/PositionComponent.h>
#include <core/Entity.h>
#include <network/PositionEvent.h>

#include "../components/SceneNodeComponent.h"
#include "../core/Game.h"

PositionEventHandler::PositionEventHandler(Game *game)
        : game(game) {
}

void PositionEventHandler::handle(const Event &e) const {
    const PositionEvent &evt = static_cast<const PositionEvent &>(e);

    Entity *entity = game->getEntityManager().getEntity(evt.getEntityId());

    // TODO: a hack for not crashing currently . change this to intended behavior.
    if (!entity) return;

    PositionComponent *posCom = entity->getComponent<PositionComponent>();

    if (posCom == nullptr) {
        return;
    }

    posCom->setPosition(evt.getX(), evt.getY(), evt.getZ());

    const auto name = std::to_string(entity->getId());
    const auto pos = osg::Vec3(posCom->getX(), posCom->getY(), posCom->getZ());

    game->getGeometryManager()->getGeometryObject(name)->setTranslate(pos);

    //if the entity is the player entity the client is controlling, change the camera position everytime the player moves
    if (entity->getId() == game->getGameClient().getCurrentPlayerEntityId()) {
		// TODO: make it in the xml file
		osg::Vec3 characterHeadPos = pos + osg::Vec3(0, 0, 1);
        game->getCamera().setFpsEyePositionAndUpdate(characterHeadPos);
    }
}
