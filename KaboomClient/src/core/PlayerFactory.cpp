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

ClientPlayer *PlayerFactory::createPlayer() const {
    createPlayer(0.0f, 0.0f, 0.0f);
}

ClientPlayer *PlayerFactory::createPlayer(float x, float y, float z) const {
    Entity *entity = entityManager->createEntity();

    return createPlayer(entity, x, y, z);
}

ClientPlayer *PlayerFactory::createPlayer(unsigned int id, float x, float y, float z) const {
    Entity *entity = entityManager->createEntity(id);

    return createPlayer(entity, x, y, z);
}

ClientPlayer *PlayerFactory::createPlayer(Entity *entity, float x, float y, float z) const {
    osg::Box *box = new osg::Box;
    osg::ShapeDrawable *drawable = new osg::ShapeDrawable(box);
    osg::Geode *model = new osg::Geode;
    model->addDrawable(drawable);

    osg::MatrixTransform *transformation = new osg::MatrixTransform;
    transformation->addChild(model);

    osg::Group *playerNode = new osg::Group;

    playerNode->addChild(transformation);

    entity->attachComponent(new SceneNodeComponent(playerNode));
    entity->attachComponent(new PositionComponent(x, y, z));

    return static_cast<ClientPlayer *>(entity);
}
