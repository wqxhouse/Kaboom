#include "KaboomV2ExplosionHandler.h"

#include <btBulletDynamicsCommon.h>

#include <components/PlayerStatusComponent.h>
#include <components/HealthComponent.h>
#include <core/Entity.h>

#include "BombData.h"
#include "BombDataLookup.h"
#include "CharacterData.h"
#include "CharacterDataLookup.h"
#include "Game.h"
#include "../components/ExplosionComponent.h"
#include "../components/PhysicsComponent.h"
#include "../components/TriggerComponent.h"

void KaboomV2ExplosionHandler::handle(
        Game *game,
        Entity *entity,
        const std::unordered_set<Entity *> &nearbyEntities) {
    TriggerComponent* bombTriggerComp = entity->getComponent<TriggerComponent>();

    for (auto nearbyEntity : nearbyEntities) {
        PlayerStatusComponent* playerStatusComp = nearbyEntity->getComponent<PlayerStatusComponent>();
        PhysicsComponent* playerPhysicsComp = nearbyEntity->getComponent<PhysicsComponent>();
		HealthComponent* playerHealthComp = nearbyEntity->getComponent<HealthComponent>();

		if (playerStatusComp == nullptr || playerPhysicsComp == nullptr || playerHealthComp == nullptr) {
            continue;
        }

        //begin timer on knockback, during this time the player cannot move

        btVector3 explosionPos = bombTriggerComp->getGhostObject()->getWorldTransform().getOrigin();
        btVector3 playerPos = playerPhysicsComp->getRigidBody()->getCenterOfMassPosition();
		printf("explosionPos : <%f,%f,%f> \n", explosionPos.getX(), explosionPos.getY(), explosionPos.getZ());
		printf("playerPos : <%f,%f,%f> \n", playerPos.getX(), playerPos.getY(), playerPos.getZ());

        btVector3 dirVec = btVector3(playerPos - explosionPos);
        dirVec.normalize();

        btScalar playerDistanceFromExplosion = playerPos.distance(explosionPos);


        const BombData &bombData = BombDataLookup::instance[entity->getType()];
		const CharacterData &characterData = CharacterDataLookup::instance[nearbyEntity->getType()];


		float maxDistancePossible = bombData.explosionRadius + (characterData.collisionRadius * 2); 
		float distancePercentAway = (maxDistancePossible - playerDistanceFromExplosion) / maxDistancePossible;
		btVector3 impulseVec = (bombData.knockbackAmount * distancePercentAway) * dirVec; //linear equation

		printf("playerDistanceFromExplosion : %f\n", playerDistanceFromExplosion);
		printf("maxDistancePossible : %f\n", bombData.explosionRadius + (characterData.collisionRadius * 2));
		printf("distancePercentAway : %f\n", (maxDistancePossible - playerDistanceFromExplosion) / maxDistancePossible);

        playerPhysicsComp->getRigidBody()->applyCentralImpulse(impulseVec);

        playerStatusComp->getKnockBackTimer().setDuration(bombData.knockbackDuration);
        playerStatusComp->getKnockBackTimer().start();
        playerStatusComp->setIsKnockBacked(true); //we could do some camera shake with this

        //apply damage
		printf("damage taken: %d \n", int((distancePercentAway * (bombData.maxDamage - bombData.minDamage))) + bombData.minDamage);

		playerHealthComp->subtractFromHealthAmount(int((distancePercentAway * (bombData.maxDamage - bombData.minDamage))) + bombData.minDamage);
		playerStatusComp->getDamageTimer().setDuration(250); // this should be global
		playerStatusComp->getDamageTimer().start();
		playerStatusComp->setIsDamaged(true);
		printf("new Player Health: %d \n", playerHealthComp->getHealthAmount());

    }

    game->getGameServer().sendExplosionEvent(entity);
    game->removeEntity(entity);//maybe we shouldn't destroy the entity right away, or wait til the next tick destory it
}
