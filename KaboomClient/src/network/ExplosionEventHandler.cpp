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
        case KABOOM_V2: 
            game->getSoundManager().playSound(SoundType::KABOOM_EXPLODE, bombPos);
            break;
		case TIME_BOMB:
			game->getSoundManager().playSound(SoundType::TIME_EXPLODE, bombPos);
			break;
		case REMOTE_DETONATOR:
			game->getSoundManager().playSound(SoundType::REMOTE_EXPLODE, bombPos);
			break;
    }

    // TODO: need to refactor the code above... probably abstract out a sound manager
    // trigger explosion particle effect
    ExplosionEffect *explosionEffect =
        static_cast<ExplosionEffect *>(game->getParticleEffectManager()->getParticleEffect(ParticleEffectManager::EXPLOSION));
    // TODO: can add some randomness by applying a random quat
    // explosionEffect->setExplosionRadius(2);
    osg::Vec3 bombPosVec = bombPos.getOsgVec3();
    explosionEffect->run(bombPos.getOsgVec3());
}
