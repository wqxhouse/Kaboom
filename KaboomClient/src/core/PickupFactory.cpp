#include "PickupFactory.h"

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

void PickupFactory::loadPickup(std::string mediaPath)
{
	_kaboom2_0_ammo = osgDB::readNodeFile( mediaPath + "DefaultAssets\\GeometryObject\\kaboom2_0.fbx");
	_timer_ammo = osgDB::readNodeFile(mediaPath + "DefaultAssets\\GeometryObject\\timeBomb.fbx");
	_remote_ammo = osgDB::readNodeFile(mediaPath + "DefaultAssets\\GeometryObject\\remoteBomb.fbx");
	_health_pack = osgDB::readNodeFile(mediaPath + "DefaultAssets\\GeometryObject\\healthPack.fbx"); 
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

	osg::ref_ptr<osg::MatrixTransform> transformation = new osg::MatrixTransform;
	transformation->addChild(_kaboom2_0_ammo);

	osg::ref_ptr<osg::Group> bombNode = new osg::Group;

	bombNode->addChild(transformation);

	entity->attachComponent(new SceneNodeComponent(bombNode));
}

void PickupFactory::createTimeBombPickup(Entity *entity, const Vec3 &position, Quat rotation) const {

	osg::ref_ptr<osg::MatrixTransform> transformation = new osg::MatrixTransform;
	transformation->addChild(_timer_ammo);

	osg::ref_ptr<osg::Group> bombNode = new osg::Group;

	bombNode->addChild(transformation);

	entity->attachComponent(new SceneNodeComponent(bombNode));
}

void PickupFactory::createRemoteDetonatorPickup(Entity *entity, const Vec3 &position, Quat rotation) const {

	osg::ref_ptr<osg::MatrixTransform> transformation = new osg::MatrixTransform;
	transformation->addChild(_remote_ammo);

	osg::ref_ptr<osg::Group> bombNode = new osg::Group;

	bombNode->addChild(transformation);

	entity->attachComponent(new SceneNodeComponent(bombNode));
}

void PickupFactory::createHealthPackPickup(Entity *entity, const Vec3 &position, Quat rotation) const {
	
	osg::ref_ptr<osg::MatrixTransform> transformation = new osg::MatrixTransform;
	transformation->addChild(_health_pack);

	osg::ref_ptr<osg::Group> bombNode = new osg::Group;

	bombNode->addChild(transformation);

	entity->attachComponent(new SceneNodeComponent(bombNode));
}
