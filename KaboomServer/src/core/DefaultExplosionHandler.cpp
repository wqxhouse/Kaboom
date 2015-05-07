#include "DefaultExplosionHandler.h"

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

void DefaultExplosionHandler::handle(
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


        const Configuration &bombConfig = BombDataLookup::instance()[entity->getType()];
		const CharacterData &characterData = CharacterDataLookup::instance[nearbyEntity->getType()];


        float maxDistancePossible = bombConfig.getFloat("explosion-radius") + (characterData.collisionRadius * 2);
		float distancePercentAway = (maxDistancePossible - playerDistanceFromExplosion) / maxDistancePossible;
        btVector3 impulseVec = (bombConfig.getFloat("knockback-amount") * distancePercentAway) * dirVec; //linear equation

		printf("playerDistanceFromExplosion : %f\n", playerDistanceFromExplosion);
        printf("maxDistancePossible : %f\n", bombConfig.getFloat("explosion-radius") + (characterData.collisionRadius * 2));
		printf("distancePercentAway : %f\n", (maxDistancePossible - playerDistanceFromExplosion) / maxDistancePossible);

        playerPhysicsComp->getRigidBody()->applyCentralImpulse(impulseVec);

        playerStatusComp->getKnockBackTimer().setDuration(bombConfig.getFloat("knockback-duration"));
        playerStatusComp->getKnockBackTimer().start();
        playerStatusComp->setIsKnockBacked(true); //we could do some camera shake with this

        //apply damage
        printf("damage taken: %d \n", int((distancePercentAway * (bombConfig.getFloat("max-damage") - bombConfig.getFloat("min-damage")))) + bombConfig.getFloat("min-damage"));

        playerHealthComp->subtractFromHealthAmount(int((distancePercentAway * (bombConfig.getFloat("max-damage") - bombConfig.getFloat("min-damage"))) + bombConfig.getFloat("min-damage")));
		playerStatusComp->getDamageTimer().setDuration(250); // this should be global
		playerStatusComp->getDamageTimer().start();
		playerStatusComp->setIsDamaged(true);
		printf("new Player Health: %d \n", playerHealthComp->getHealthAmount());

    }

    game->getGameServer().sendExplosionEvent(entity);
    game->removeEntity(entity);//maybe we shouldn't destroy the entity right away, or wait til the next tick destory it
}
