#include "BombFactory.h"

#include <osg/Geode>
#include <osg/MatrixTransform>
#include <osg/Shape>
#include <osg/ShapeDrawable>

#include <components/PositionComponent.h>
#include <components/RotationComponent.h>
#include <core/EntityManager.h>

#include "EntityConfigLookup.h"
#include "../components/SceneNodeComponent.h"

BombFactory::BombFactory(EntityManager &entityManager)
        : entityManager(entityManager) {
}

Entity *BombFactory::createBomb(
        unsigned int id,
        EntityType bombType,
        float x,
        float y,
        float z,
        Quat rotation) const {
    auto &config= EntityConfigLookup::get(bombType);

    Entity *entity = entityManager.createEntity(id, bombType);

    osg::ref_ptr<osg::Sphere> sphere = new osg::Sphere();
    sphere->setRadius(config.getFloat("size"));
    osg::ref_ptr<osg::ShapeDrawable> drawable = new osg::ShapeDrawable(sphere);
    osg::ref_ptr<osg::Geode> model = new osg::Geode;
    model->addDrawable(drawable);

    osg::ref_ptr<osg::MatrixTransform> transformation = new osg::MatrixTransform;
    transformation->addChild(model);

    osg::ref_ptr<osg::Group> bombNode = new osg::Group;

	bombNode->addChild(transformation);

	entity->attachComponent(new SceneNodeComponent(bombNode));
    entity->attachComponent(new PositionComponent(x, y, z));
    entity->attachComponent(new RotationComponent(rotation));

    return entity;
}
