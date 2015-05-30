#include "BombFactory.h"

#include <Core.h>
#include <GeometryCache.h>
#include <GeometryObject.h>
#include <osg/Geode>
#include <osg/MatrixTransform>
#include <osg/Shape>
#include <osg/ShapeDrawable>

#include <components/PositionComponent.h>
#include <components/RotationComponent.h>
#include <core/EntityManager.h>
#include <osgDB/ReadFile>

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

    //createBase(entity, position, rotation);

	entity->attachComponent(new PositionComponent(position));
	entity->attachComponent(new RotationComponent(rotation));

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
        case SALTY_MARTY_BOMB: {
            createSaltyMartyBomb(entity);
            break;
        }
		default:{
			createBase(entity, position, rotation);
			break;
		}
    }

    return entity;
}

void BombFactory::createBase(Entity *entity, const Vec3 &position, Quat rotation) const {

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
	
	GeometryCache *geoCache = Core::getWorldRef().getGeometryCache();

	//Kaboom has an id of 1.
	Material * mat = geoCache->getMaterialById(1);
	osg::ref_ptr<osg::Node> kaboom = geoCache->getNodeById(1);

	//this is a memory leak, hopefully it is not too bad.
	GeometryObject *kaboom2_0 = new GeometryObject("kaboom", kaboom);
	kaboom2_0->setMaterial(mat);
	entity->attachComponent(new SceneNodeComponent(kaboom2_0->getRoot()));
}

void BombFactory::createTimeBomb(Entity *entity) const {

	GeometryCache *geoCache = Core::getWorldRef().getGeometryCache();

	//Timer has an id of 2.
	Material * mat = geoCache->getMaterialById(2);
	osg::ref_ptr<osg::Node> timer = geoCache->getNodeById(2);

	//this is a memory leak, hopefully it is not too bad.
	GeometryObject *timer_bomb = new GeometryObject("timer", timer);
	timer_bomb->setMaterial(mat);
	entity->attachComponent(new SceneNodeComponent(timer_bomb->getRoot()));
}

void BombFactory::createRemoteDetonator(Entity *entity) const {

	GeometryCache *geoCache = Core::getWorldRef().getGeometryCache();

	//Remote has an id of 3.
	Material * mat = geoCache->getMaterialById(3);
	osg::ref_ptr<osg::Node> remote = geoCache->getNodeById(3);

	//this is a memory leak, hopefully it is not too bad.
	GeometryObject *remote_bomb = new GeometryObject("remote", remote);
	remote_bomb->setMaterial(mat);
	entity->attachComponent(new SceneNodeComponent(remote_bomb->getRoot()));
}

void BombFactory::createSaltyMartyBomb(Entity *entity) const {
    // TODO: Change geometry to salty marty bomb

    GeometryCache *geoCache = Core::getWorldRef().getGeometryCache();

    //Kaboom has an id of 1.
    Material * mat = geoCache->getMaterialById(1);
    osg::ref_ptr<osg::Node> kaboom = geoCache->getNodeById(1);

    //this is a memory leak, hopefully it is not too bad.
    GeometryObject *kaboom2_0 = new GeometryObject("kaboom", kaboom);
    kaboom2_0->setMaterial(mat);
    entity->attachComponent(new SceneNodeComponent(kaboom2_0->getRoot()));
}
