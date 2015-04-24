#include "PositionEventHandler.h"

#include <osg/MatrixTransform>
#include <Core.h>
#include <GeometryObject.h>
#include <core/Entity.h>
#include <core/PositionComponent.h>
#include <network/PositionEvent.h>

#include "../core/Game.h"
#include "../core/SceneNodeComponent.h"

PositionEventHandler::PositionEventHandler(Game *game)
    : game(game) {
}

PositionEventHandler::~PositionEventHandler() {
}

void PositionEventHandler::handle(const Event &e) const {
    const PositionEvent &evt = static_cast<const PositionEvent &>(e);

    Entity *entity = game->getEntityManager().getEntity(evt.getEntityId());

	// TODO: a hack for not crashing currently . change this to intended behavior.
	if (!entity) return;

    PositionComponent *posCom = entity->getComponent<PositionComponent>();

    if (posCom != nullptr) {
        posCom->setPosition(evt.getX(), evt.getY(), evt.getZ());
    }

	game->getGeometryManager()->getGeometryObject(std::to_string(static_cast<int>(entity->getId())))->setTranslate(osg::Vec3(posCom->getX(), posCom->getY(), posCom->getZ()));

	//if the entity is the player entity the client is controlling, change the camera position everytime the player moves
	if (entity->getId() == game->getGameClient().getCurrentPlayerEntityId()){
		//printf("Changed player camera position : %d, x:%f, y:%f, z:%f\n", entity->getId(), evt.getX(), evt.getY(), evt.getZ());
		//game->getCamera().setEyePositionAndUpdate(osg::Vec3(evt.getX(), evt.getY(), evt.getZ()));
		osg::Vec3 eye = osg::Vec3(evt.getX(), evt.getY(), evt.getZ());
		game->getCamera().setFpsEyePositionAndUpdate(eye);
	}


	/*
    SceneNodeComponent *sceneNodeCom = entity->getComponent<SceneNodeComponent>();

    if (sceneNodeCom != nullptr) {
        osg::Group *rootNode = static_cast<osg::Group *>(sceneNodeCom->getNode());
        osg::MatrixTransform *transformNode = static_cast<osg::MatrixTransform *>(rootNode->getChild(0));

        osg::Matrix transformMat;
        transformMat.makeTranslate(evt.getX(), evt.getY(), evt.getZ());

        transformNode->setMatrix(transformMat);
    }
	*/
	
}
