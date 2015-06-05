#include "ExplosionEventHandler.h"

#include <cmath>

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

    switch (bomb->getType()) {
        case TIME_BOMB: {
            game->getSoundManager().playSound(SoundType::TIME_EXPLODE, bombPos);
            ExplosionEffect *explosionEffect =
                    static_cast<ExplosionEffect *>(game->getParticleEffectManager()->getParticleEffect(ParticleEffectManager::EXPLOSION));
            osg::Vec3 bombPosVec = bombPos.getOsgVec3();
            explosionEffect->run(bombPos.getOsgVec3());
            break;
        }
        case FAKE_BOMB:
        case REMOTE_DETONATOR: {
            game->getSoundManager().playSound(SoundType::REMOTE_EXPLODE, bombPos);
            ExplosionEffect *explosionEffect =
                    static_cast<ExplosionEffect *>(game->getParticleEffectManager()->getParticleEffect(ParticleEffectManager::EXPLOSION2));
            osg::Vec3 bombPosVec = bombPos.getOsgVec3();
            explosionEffect->run(bombPos.getOsgVec3());
            break;
        }
        default: {
            game->getSoundManager().playSound(SoundType::KABOOM_EXPLODE, bombPos);
            ExplosionEffect *explosionEffect =
                static_cast<ExplosionEffect *>(game->getParticleEffectManager()->getParticleEffect(ParticleEffectManager::EXPLOSION));
            osg::Vec3 bombPosVec = bombPos.getOsgVec3();
            explosionEffect->run(bombPos.getOsgVec3());
            break;
        }
    }
}
