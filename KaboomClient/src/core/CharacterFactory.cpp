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
#include "../components/SoundComponent.h"
#include "../sound/SoundManager.h"
#include "Model.h"

CharacterFactory::CharacterFactory(EntityManager &entityManager)
        : entityManager(entityManager) {
}

Entity *CharacterFactory::createCharacter(
        unsigned int id,
        EntityType characterType,
        const Vec3 &position,
        Quat rotation) const {
    Entity *entity = entityManager.createEntity(id, characterType);

	entity->attachComponent(new ModelComponent());
	auto modelComp = entity->getComponent<ModelComponent>();

	entity->attachComponent(new SceneNodeComponent(modelComp->getModel()->getRootNode()));
    entity->attachComponent(new PositionComponent(position));
    entity->attachComponent(new RotationComponent(rotation));
	entity->attachComponent(new HealthComponent(100,100));
	entity->attachComponent(new PlayerStatusComponent());
	std::string name = std::to_string(static_cast<unsigned int>(SoundType::WALKING));
	entity->attachComponent(new SoundComponent(name));

    return entity;
}
