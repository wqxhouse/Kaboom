#include "BombFactory.h"


#include <osg/Geode>
#include <osg/MatrixTransform>
#include <osg/Shape>
#include <osg/ShapeDrawable>

#include <core/PositionComponent.h>

#include "SceneNodeComponent.h"

BombFactory::BombFactory(EntityManager *entityManager)
    : entityManager(entityManager) {
}

BombFactory::~BombFactory() {
}

Entity *BombFactory::createBomb(unsigned int id, float x, float y, float z) const {
    Entity *entity = entityManager->createEntity(id);

    osg::Sphere *sphere = new osg::Sphere();
	sphere->setRadius(0.5); //make this dynamic
    osg::ShapeDrawable *drawable = new osg::ShapeDrawable(sphere);
    osg::Geode *model = new osg::Geode;
    model->addDrawable(drawable);

    osg::MatrixTransform *transformation = new osg::MatrixTransform;
    transformation->addChild(model);

    osg::Group *bombNode = new osg::Group;

	bombNode->addChild(transformation);

	entity->attachComponent(new SceneNodeComponent(bombNode));
    entity->attachComponent(new PositionComponent(x, y, z));

    return entity;
}
