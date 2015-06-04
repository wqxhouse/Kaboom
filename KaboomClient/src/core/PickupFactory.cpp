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

    //createBase(entity, position, rotation);

	entity->attachComponent(new PositionComponent(position));
	entity->attachComponent(new RotationComponent(rotation));

	GeometryCache *geoCache = Core::getWorldRef().getGeometryCache();

	//Currently using bomb models as pickup
	// TODO: Change bomb models to pickup box model
	osg::ref_ptr<osg::Node> box_node = geoCache->getNodeById(type);

	entity->attachComponent(new SceneNodeComponent(box_node));
    entity->attachComponent(new WeaponPickupComponent());

    return entity;
}