#include "JumpPadFactory.h"

#include <osg/Geode>
#include <osg/MatrixTransform>
#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <components/HealthComponent.h>
#include <components/PositionComponent.h>
#include <components/RotationComponent.h>
#include <components/PlayerStatusComponent.h>
#include <core/EntityManager.h>
#include "../components/SceneNodeComponent.h"
#include "Model.h"
#include "util/ConfigSettings.h"

JumpPadFactory::JumpPadFactory(EntityManager &entityManager)
        : entityManager(entityManager) {
}

Entity *JumpPadFactory::createJumpPad(
        unsigned int id,
        EntityType jumpPadType,
        const Vec3 &position,
        Quat rotation) const {
	Entity *entity = entityManager.createEntity(id, jumpPadType);

	osg::ref_ptr<osg::Box> box = new osg::Box(osg::Vec3(), 1.0f, 1.0f, 0.5f);
	osg::ref_ptr<osg::ShapeDrawable> drawable = new osg::ShapeDrawable(box);
    osg::ref_ptr<osg::Geode> model = new osg::Geode;
    model->addDrawable(drawable);

    osg::ref_ptr<osg::MatrixTransform> transformation = new osg::MatrixTransform;
    transformation->addChild(model);

    osg::ref_ptr<osg::Group> jumpPadNode = new osg::Group;

	jumpPadNode->addChild(transformation);

	entity->attachComponent(new PositionComponent(position));
	entity->attachComponent(new RotationComponent(rotation));
	entity->attachComponent(new SceneNodeComponent(jumpPadNode));

    return entity;
}
