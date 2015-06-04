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

	GeometryCache *geoCache = Core::getWorldRef().getGeometryCache();

	//Material * mat = geoCache->getMaterialById(type);
	osg::ref_ptr<osg::Node> node = geoCache->getNodeById(type);

	//this is a memory leak, hopefully it is not too bad.
	//GeometryObject *bomb_geom = new GeometryObject(bomb_name, node);
	//bomb_geom->setMaterial(mat);
	entity->attachComponent(new SceneNodeComponent(node));// bomb_geom->getRoot()));

    return entity;
}