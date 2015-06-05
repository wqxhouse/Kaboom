#include "ExplosionEventHandler.h"

#include <cmath>

#include <Core.h>
#include <components/PositionComponent.h>
#include <components/RotationComponent.h>
#include <network/ExplosionEvent.h>
#include <ParticleEffectManager.h>
#include <ExplosionEffect.h>

#include "../core/Game.h"

ExplosionEventHandler::ExplosionEventHandler(Game *game)
        : game(game) {
}

void ExplosionEventHandler::handle(const Event &e) const {
    const ExplosionEvent &evt = static_cast<const ExplosionEvent &>(e);

    EntityManager &entityManager = game->getEntityManager();
    Entity *bomb = entityManager.getEntity(evt.getBombId());
    Entity *player = game->getCurrentPlayer()->getEntity();

    auto bombPos = bomb->getComponent<PositionComponent>()->getPosition();
    auto playerRot = player->getComponent<RotationComponent>()->getRotation();
    auto playerPos = player->getComponent<PositionComponent>()->getPosition();

    // TODO: Move this to InputEventHandler.
    game->getSoundManager().setListenerPosition(playerPos);
    game->getSoundManager().setListenerRotation(playerRot);

    // TODO: Refactor this into component

    const std::string lightName = std::to_string(bomb->getId()) + std::to_string(rand());

    Game::ExplosionLight *expLight1 = new Game::ExplosionLight();
    expLight1->name = lightName;
    expLight1->radius = 3.0f;
    expLight1->step = 0.0f;

    Game::ExplosionLight *expLight2 = new Game::ExplosionLight();
    expLight2->name = lightName;
    expLight2->radius = 5.0f;
    expLight2->step = 0.0f;

    switch (bomb->getType()) {
        case TIME_BOMB: {
            game->getSoundManager().playSound(SoundType::TIME_EXPLODE, bombPos);
            ExplosionEffect *explosionEffect =
                    static_cast<ExplosionEffect *>(game->getParticleEffectManager()->getParticleEffect(ParticleEffectManager::EXPLOSION));
            osg::Vec3 bombPosVec = bombPos.getOsgVec3();
            explosionEffect->run(bombPos.getOsgVec3());

            game->explosionLightMap[lightName] = expLight1;

            Core::getWorldRef().getLightManager()->addPointLight(lightName, bombPosVec, osg::Vec3(1.0f, 0.5f, 0.2f), 0.0f, false, 0.0f);
            break;
        }
        case REMOTE_DETONATOR: {
            game->getSoundManager().playSound(SoundType::REMOTE_EXPLODE, bombPos);
            ExplosionEffect *explosionEffect =
                    static_cast<ExplosionEffect *>(game->getParticleEffectManager()->getParticleEffect(ParticleEffectManager::EXPLOSION2));
            osg::Vec3 bombPosVec = bombPos.getOsgVec3();
            explosionEffect->run(bombPos.getOsgVec3());

            game->explosionLightMap[lightName] = expLight2;

            Core::getWorldRef().getLightManager()->addPointLight(lightName, bombPosVec, osg::Vec3(0.1294f, 0.6510f, 0.5216f), 0.0f, false, 0.0f);
            break;
        }
		case SALTY_MARTY_BOMB: {
								   //no sound yet
			ExplosionEffect *explosionEffect =
                    static_cast<ExplosionEffect *>(game->getParticleEffectManager()->getParticleEffect(ParticleEffectManager::EXPLOSION2));
            osg::Vec3 bombPosVec = bombPos.getOsgVec3();
            explosionEffect->run(bombPos.getOsgVec3());
            game->setSMScreen();

            game->explosionLightMap[lightName] = expLight2;

            Core::getWorldRef().getLightManager()->addPointLight(lightName, bombPosVec, osg::Vec3(0.1294f, 0.6510f, 0.5216f), 0.0f, false, 0.0f);
			break;
		}
		case FAKE_BOMB: {
							//no sound yet
			ExplosionEffect *explosionEffect =
				static_cast<ExplosionEffect *>(game->getParticleEffectManager()->getParticleEffect(ParticleEffectManager::EXPLOSION2));
			osg::Vec3 bombPosVec = bombPos.getOsgVec3();
            explosionEffect->run(bombPos.getOsgVec3());

            game->explosionLightMap[lightName] = expLight2;

            Core::getWorldRef().getLightManager()->addPointLight(lightName, bombPosVec, osg::Vec3(1.0f, 0.5f, 0.2f), 0.0f, false, 0.0f);
			break;
		}
        default: {
            game->getSoundManager().playSound(SoundType::KABOOM_EXPLODE, bombPos);
            ExplosionEffect *explosionEffect =
                static_cast<ExplosionEffect *>(game->getParticleEffectManager()->getParticleEffect(ParticleEffectManager::EXPLOSION));
            osg::Vec3 bombPosVec = bombPos.getOsgVec3();
            explosionEffect->run(bombPos.getOsgVec3());

            game->explosionLightMap[lightName] = expLight1;

            Core::getWorldRef().getLightManager()->addPointLight(lightName, bombPosVec, osg::Vec3(1.0f, 0.5f, 0.2f), 0.0f, false, 0.0f);
            break;
        }
    }
}
