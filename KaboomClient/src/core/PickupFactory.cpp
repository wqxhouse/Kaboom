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

	std::string box_name;

    switch (type) {
        case KABOOM_V2:
			box_name = "kaboom_box";
            break;
        case TIME_BOMB:
			box_name = "timer_box";
            break;
        case REMOTE_DETONATOR:
			box_name = "remote_box";
            break;
		case HEALTH_PACK:
			box_name = "health_pack";
			break;
		default:
			box_name = "kaboom_box";
			break;
    }


	GeometryCache *geoCache = Core::getWorldRef().getGeometryCache();

	//Currently using bomb models as pickup
	// TODO: Change bomb models to pickup box model
	Material * mat = geoCache->getMaterialById(type);
	osg::ref_ptr<osg::Node> box_node = geoCache->getNodeById(type);

	//this is a memory leak, hopefully it is not too bad.
	GeometryObject *box_geom = new GeometryObject(box_name, box_node);
	box_geom->setMaterial(mat);
	entity->attachComponent(new SceneNodeComponent(box_geom->getRoot()));

    return entity;
}