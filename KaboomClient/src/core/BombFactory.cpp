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
        EntityType type,
        const Vec3 &position,
        Quat rotation) const {
    Entity *entity = entityManager.createEntity(id, type);

    createBase(entity, position, rotation);

    switch (type) {
        case KABOOM_V2: {
            createKaboomV2(entity);
            break;
        }
        case TIME_BOMB: {
            createTimeBomb(entity);
            break;
        }
        case REMOTE_DETONATOR: {
            createRemoteDetonator(entity);
            break;
        }
    }

    return entity;
}

void BombFactory::createBase(Entity *entity, const Vec3 &position, Quat rotation) const {
    entity->attachComponent(new PositionComponent(position));
    entity->attachComponent(new RotationComponent(rotation));

    auto &config = EntityConfigLookup::get(entity->getType());

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
}

void BombFactory::createKaboomV2(Entity *entity) const {
}

void BombFactory::createTimeBomb(Entity *entity) const {
}

void BombFactory::createRemoteDetonator(Entity *entity) const {
}
