#include "PickupFactory.h"

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

PickupFactory::PickupFactory(EntityManager &entityManager)
        : entityManager(entityManager) {

}

Entity *PickupFactory::createPickup(
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
			createKaboomV2Pickup(entity, position, rotation);
            break;
        }
        case TIME_BOMB: {
			createTimeBombPickup(entity, position, rotation);
            break;
        }
        case REMOTE_DETONATOR: {
			createRemoteDetonatorPickup(entity, position, rotation);
            break;
        }
		case HEALTH_PACK:{
			createHealthPackPickup(entity, position, rotation);
			break;
		}
		default:{
			createBase(entity, position, rotation);
			break;
		}
    }

    return entity;
}

void PickupFactory::createBase(Entity *entity, const Vec3 &position, Quat rotation) const {

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

void PickupFactory::createKaboomV2Pickup(Entity *entity, const Vec3 &position, Quat rotation) const {

	GeometryCache *geoCache = Core::getWorldRef().getGeometryCache();

	//Currently using Kaboom object as a place holder
	//Kaboom has an id of 1.
	Material * mat = geoCache->getMaterialById(1);
	osg::ref_ptr<osg::Node> kaboom_box = geoCache->getNodeById(1);

	//this is a memory leak, hopefully it is not too bad.
	GeometryObject *kaboom_pickup = new GeometryObject("kaboom_box", kaboom_box);
	kaboom_pickup->setMaterial(mat);
	entity->attachComponent(new SceneNodeComponent(kaboom_pickup->getRoot()));
}

void PickupFactory::createTimeBombPickup(Entity *entity, const Vec3 &position, Quat rotation) const {

	GeometryCache *geoCache = Core::getWorldRef().getGeometryCache();

	//Currently using the Timer object as a place holder.
	//Timer has an id of 2.
	Material * mat = geoCache->getMaterialById(2);
	osg::ref_ptr<osg::Node> timer_box = geoCache->getNodeById(2);

	//this is a memory leak, hopefully it is not too bad.
	GeometryObject *timer_pickup = new GeometryObject("timer_box", timer_box);
	timer_pickup->setMaterial(mat);
	entity->attachComponent(new SceneNodeComponent(timer_pickup->getRoot()));
}

void PickupFactory::createRemoteDetonatorPickup(Entity *entity, const Vec3 &position, Quat rotation) const {

	GeometryCache *geoCache = Core::getWorldRef().getGeometryCache();

	//Currently using the remote bomb object as placeholder
	//Remote has an id of 3.
	Material * mat = geoCache->getMaterialById(3);
	osg::ref_ptr<osg::Node> remote_box = geoCache->getNodeById(3);

	//this is a memory leak, hopefully it is not too bad.
	GeometryObject *remote_pickup = new GeometryObject("remote_box", remote_box);
	remote_pickup->setMaterial(mat);
	entity->attachComponent(new SceneNodeComponent(remote_pickup->getRoot()));
}

void PickupFactory::createHealthPackPickup(Entity *entity, const Vec3 &position, Quat rotation) const {
	
	GeometryCache *geoCache = Core::getWorldRef().getGeometryCache();

	//Health pack has an id of 201.
	Material * mat = geoCache->getMaterialById(201);
	osg::ref_ptr<osg::Node> health_pack = geoCache->getNodeById(201);

	//this is a memory leak, hopefully it is not too bad.
	GeometryObject *health_pack_pickup = new GeometryObject("health_pack", health_pack);
	health_pack_pickup->setMaterial(mat);
	entity->attachComponent(new SceneNodeComponent(health_pack_pickup->getRoot()));
}
