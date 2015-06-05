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
#include <components/WeaponPickupComponent.h>
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

	entity->attachComponent(new PositionComponent(position));
	entity->attachComponent(new RotationComponent(rotation));

    //createBase(entity, position, rotation);
	if (type == SALTY_MARTY_BOMB)
	{
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
		entity->attachComponent(new WeaponPickupComponent());
	}
	else{
		GeometryCache *geoCache = Core::getWorldRef().getGeometryCache();

		//Currently using bomb models as pickup
		// TODO: Change bomb models to pickup box model
		osg::ref_ptr<osg::Node> box_node = geoCache->getNodeById(type);

		entity->attachComponent(new SceneNodeComponent(box_node));
		entity->attachComponent(new WeaponPickupComponent());
	}
    return entity;
}