#include "SpawnEventHandler.h"

#include <Core.h>
#include <ObjectGlowManager.h>
#include <core/EntityType.h>
#include <network/SpawnEvent.h>
#include <components/PositionComponent.h>
#include <ParticleEffectManager.h>
#include <TrailingEffect.h>
#include <components/RotationComponent.h>
#include "../core/Game.h"
#include "../components/SceneNodeComponent.h"
#include "../components/TrailingEffectComponent.h"
#include <GeometryObject.h>

SpawnEventHandler::SpawnEventHandler(Game *game)
        : game(game) {
}

void SpawnEventHandler::handle(const Event &e) const {
    const SpawnEvent &evt = static_cast<const SpawnEvent &>(e);
	EntityManager &entityManager = game->getEntityManager();
    EntityType type = evt.getType();
    Entity *entity = nullptr;
	Entity *player = game->getCurrentPlayer()->getEntity();
	
    if ((type & CAT_MASK) == CAT_CHARACTER) {
        entity = game->getCharacterFactory().createCharacter(
                evt.getEntityId(),
                evt.getType(),
                evt.getPosition(),
                evt.getRotation());
    } else if (evt.isPickup()) { //If it's a pickup we want to create a pickup instead
		entity = game->getPickupFactory().createPickup(
			evt.getEntityId(),
			evt.getType(),
			evt.getPosition(),
			evt.getRotation());
	} else if ((type & CAT_MASK) == CAT_BOMB) {
        entity = game->getBombFactory().createBomb(
                evt.getEntityId(),
                evt.getType(),
                evt.getPosition(),
                evt.getRotation());

        Entity *player = game->getCurrentPlayer()->getEntity();
		if (player != nullptr){


			PositionComponent *playerPosComp = player->getComponent<PositionComponent>();
			PositionComponent *entityPosComp = entity->getComponent<PositionComponent>();
            const Vec3 &playerPos = playerPosComp->getPosition();
            const Vec3 &entityPos = entityPosComp->getPosition();

            double x = (double)(-playerPos.x + entityPos.x);
            double y = (double)(-playerPos.y + entityPos.y);
            double z = (double)(-playerPos.z + entityPos.z);
			auto bombPos = entity->getComponent<PositionComponent>()->getPosition();
			auto playerRot = player->getComponent<RotationComponent>()->getRotation();
			auto playerPoss = player->getComponent<PositionComponent>()->getPosition();
			game->getSoundManager().setListenerPosition(playerPoss);
			game->getSoundManager().setListenerRotation(playerRot);
			switch (type) {
                case KABOOM_V2: {
                    game->getSoundManager().playSound(SoundType::KABOOM_FIRE,bombPos);
                    break;
				case TIME_BOMB:
					game->getSoundManager().playSound(SoundType::TIME_FIRE, bombPos);
					break;
				case REMOTE_DETONATOR:
					game->getSoundManager().playSound(SoundType::REMOTE_FIRE, bombPos);
					break;
                }
			}
		}
	} else if ((type & CAT_MASK) == CAT_JUMPPAD) {
		entity = game->getJumpPadFactory().createJumpPad(
			evt.getEntityId(),
			evt.getType(),
			evt.getPosition(),
			evt.getRotation());
	}

    if (entity != nullptr) {
        auto posComp = entity->getComponent<PositionComponent>();
        auto sceneNodeComp = entity->getComponent<SceneNodeComponent>();

        if (sceneNodeComp != nullptr) {
            const Vec3 &pos = posComp->getPosition();

            const auto name = std::to_string(entity->getId());
            const auto osgPos = osg::Vec3(pos.x, pos.y, pos.z);

            game->getGeometryManager()->addGeometry(name, sceneNodeComp->getNode(), osgPos);

            if (evt.isPickup()) {
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

                auto obj = Core::getWorldRef().getGeometryManager()->getGeometryObject(name);
                Core::getWorldRef().getObjectGlowManager()->addGlowGeometryObject(obj);
            }
        }

		if ((entity->getType() & CAT_MASK) == CAT_BOMB) {
			TrailingEffect *effect = static_cast<TrailingEffect *>(
				game->getParticleEffectManager()->getParticleEffect(ParticleEffectManager::TRAILING_PURPLE));

			GeometryObject *geomObj = game->getGeometryManager()->getGeometryObject(std::to_string(entity->getId()));
			effect->setTrailedObject(geomObj);
			effect->run(geomObj->getTranslate());

			entity->attachComponent(new TrailingEffectComponent(effect));
        }
    }
}
