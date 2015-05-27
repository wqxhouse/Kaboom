#include "CharacterFactory.h"

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
#include "../components/ModelComponent.h"
#include "Model.h"
#include "Core.h"
#include "GeometryCache.h"
#include "util/ConfigSettings.h"

CharacterFactory::CharacterFactory(EntityManager &entityManager)
        : entityManager(entityManager) {
}

Entity *CharacterFactory::createCharacter(
        unsigned int id,
        EntityType characterType,
        const Vec3 &position,
        Quat rotation) const {
    Entity *entity = entityManager.createEntity(id, characterType);

	Model *model;
	model = new Model(IDLE, true);
	model->addAnimationById(RUNNING);
	model->playAnimation(IDLE);

	entity->attachComponent(new ModelComponent(model));
	entity->attachComponent(new SceneNodeComponent(model->getRootNode()));
    entity->attachComponent(new PositionComponent(position));
    entity->attachComponent(new RotationComponent(rotation));
	entity->attachComponent(new HealthComponent(100,100));
	entity->attachComponent(new PlayerStatusComponent());

    return entity;
}
