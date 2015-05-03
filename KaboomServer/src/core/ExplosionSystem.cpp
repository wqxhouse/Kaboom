#include "ExplosionSystem.h"

#include <core/PlayerStatusComponent.h>

#include "BombDataLookup.h"
#include "ExplosionComponent.h"
#include "Game.h"
#include "TriggerComponent.h"
#include "../math/Conversion.h"

ExplosionSystem::ExplosionSystem(Game *game)
        : EntitySystem(game) {
}

void ExplosionSystem::update(float timeStep) {
    auto entities = game->getEntityManager().getEntityList();

    for (Entity *bombEntity : entities) {
		if (!bombEntity->hasComponent<ExplosionComponent>() || !bombEntity->hasComponent<TriggerComponent>()) {
            continue;
        }


		TriggerComponent* bombTriggerComp = bombEntity->getComponent<TriggerComponent>();


		for (auto collidedEntity : bombTriggerComp->getTriggerEntities()) {

			if (collidedEntity->getType() & CAT_CHARACTER) {
				if (collidedEntity->hasComponent<PlayerStatusComponent>() && collidedEntity->hasComponent<PhysicsComponent>()) {

					PlayerStatusComponent* playerStatusComp = collidedEntity->getComponent<PlayerStatusComponent>();
					PhysicsComponent* playerPhysicComp = collidedEntity->getComponent<PhysicsComponent>();
					
					//begin timer on knockback, during this time the player cannot move

					btVector3 explosionPos = bombTriggerComp->getGhostObject()->getWorldTransform().getOrigin();
					btVector3 playerPos = playerPhysicComp->getRigidBody()->getCenterOfMassPosition();

					btVector3 dirVec = btVector3(playerPos - explosionPos);
					dirVec.normalize();

					btScalar distFromExplosion = playerPos.distance(explosionPos);


					const BombData &bombData = BombDataLookup::instance[bombEntity->getType()];

					btVector3 impulseVec = (bombData.knockbackRatio / distFromExplosion) * dirVec; //inverse function

					playerPhysicComp->getRigidBody()->applyCentralImpulse(impulseVec);

					playerStatusComp->getKnockBackTimer().setDuration(bombData.knockbackDuration); 
					playerStatusComp->getKnockBackTimer().start();
					playerStatusComp->setIsKnockBacked(true);

					//TODO apply damage base on dist

				}
			}
		}

		game->getGameServer().sendExplosionEvent(bombEntity);
		game->removeEntity(bombEntity);
    }
}
