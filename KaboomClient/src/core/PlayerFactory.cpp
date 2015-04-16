#include "PlayerFactory.h"

#include <Windows.h>

#include <osg/Geode>
#include <osg/MatrixTransform>
#include <osg/Shape>
#include <osg/ShapeDrawable>

#include <core/PositionComponent.h>

#include "SceneNodeComponent.h"

PlayerFactory::PlayerFactory(EntityManager *entityManager)
    : entityManager(entityManager) {
}

PlayerFactory::~PlayerFactory() {
}

ClientPlayer *PlayerFactory::createPlayer(unsigned int id, float x, float y, float z) const {
    ClientPlayer *player = new ClientPlayer(id);
    entityManager->addEntity(player);

    osg::Box *box = new osg::Box;
    osg::ShapeDrawable *drawable = new osg::ShapeDrawable(box);
    osg::Geode *model = new osg::Geode;
    model->addDrawable(drawable);

    osg::MatrixTransform *transformation = new osg::MatrixTransform;
    transformation->addChild(model);

    osg::Group *playerNode = new osg::Group;

    playerNode->addChild(transformation);

    player->attachComponent(new SceneNodeComponent(playerNode));
    player->attachComponent(new PositionComponent(x, y, z));

    return player;
}
