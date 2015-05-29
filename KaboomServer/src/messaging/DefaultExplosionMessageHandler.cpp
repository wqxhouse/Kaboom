#include "DefaultExplosionMessageHandler.h"

#include <btBulletDynamicsCommon.h>

#include <components/PlayerComponent.h>
#include <components/HealthComponent.h>
#include <components/PlayerStatusComponent.h>
#include <core/Entity.h>
#include <core/Player.h>

#include "ExplosionMessage.h"
#include "Message.h"
#include "MessageType.h"
#include "../components/DestroyComponent.h"
#include "../components/DetonatorComponent.h"
#include "../components/ExplosionComponent.h"
#include "../components/PhysicsComponent.h"
#include "../components/OwnerComponent.h"
#include "../components/TriggerComponent.h"
#include "../core/EntityConfigLookup.h"
#include "../core/Game.h"

bool DefaultExplosionMessageHandler::handle(const Message &message) const {
    switch (message.getType()) {
        case MessageType::EXPLOSION: {
            return handle(static_cast<const ExplosionMessage &>(message));
        }
    }

    return false;
}

bool DefaultExplosionMessageHandler::handle(const ExplosionMessage &message) const {
    Game *game = message.getGame();
    Entity *entity = message.getEntity();

    auto &nearbyEntities = message.getNearbyEntities();

    auto bombTriggerComp = entity->getComponent<TriggerComponent>();

    for (auto nearbyEntity : nearbyEntities) {
        if (nearbyEntity->getType() == REMOTE_DETONATOR) {
            nearbyEntity->attachComponent(new ExplosionComponent());
            continue;
        }

        auto charStatusComp = nearbyEntity->getComponent<PlayerStatusComponent>();
        auto charPhysicsComp = nearbyEntity->getComponent<PhysicsComponent>();
        auto charHealthComp = nearbyEntity->getComponent<HealthComponent>();

        if (charStatusComp == nullptr || charPhysicsComp == nullptr || charHealthComp == nullptr) {
            continue;
        }

        // Begin timer on knockback, during this time the player cannot move
        btVector3 explosionPos = bombTriggerComp->getGhostObject()->getWorldTransform().getOrigin();
        btVector3 playerPos = charPhysicsComp->getRigidBody()->getCenterOfMassPosition();
        printf("explosionPos : <%f,%f,%f> \n", explosionPos.getX(), explosionPos.getY(), explosionPos.getZ());
        printf("playerPos : <%f,%f,%f> \n", playerPos.getX(), playerPos.getY(), playerPos.getZ());

        btVector3 dirVec = btVector3(playerPos - explosionPos);
        dirVec.normalize();
	
        btScalar playerDistanceFromExplosion = playerPos.distance(explosionPos);

        auto &bombConfig = EntityConfigLookup::get(entity->getType());
        auto &charConfig = EntityConfigLookup::get(nearbyEntity->getType());

        float explosionRadius = bombConfig.getFloat("explosion-radius");
        float knockbackAmount = bombConfig.getFloat("knockback-amount");
        int knockbackDuration = bombConfig.getInt("knockback-duration");
        int maxDamage = bombConfig.getInt("max-damage");
        int minDamage = bombConfig.getInt("min-damage");
        float collisionRadius = charConfig.getFloat("collision-radius");

        float maxDistancePossible = explosionRadius + (collisionRadius + 0.3f); //0.3f is hard code fix
        float distancePercentAway = (maxDistancePossible - playerDistanceFromExplosion) / maxDistancePossible;
        btVector3 impulseVec = (knockbackAmount * distancePercentAway) * dirVec; // linear equation
        printf("playerDistanceFromExplosion : %f\n", playerDistanceFromExplosion);
        printf("maxDistancePossible : %f\n", maxDistancePossible);
        printf("distancePercentAway : %f\n", distancePercentAway);
		printf("impulseVec-> x : %f, y :%f, z :%f\n", impulseVec.getX(), impulseVec.getY(), impulseVec.getZ());

        charPhysicsComp->getRigidBody()->applyCentralImpulse(impulseVec);

        charStatusComp->getKnockBackTimer().setDuration(knockbackDuration);
        charStatusComp->getKnockBackTimer().start();
        charStatusComp->setIsKnockBacked(true); // we could do some camera shake with this

        // apply damage
        int damage = int(distancePercentAway * (maxDamage - minDamage) + minDamage);
        printf("damage taken: %d \n", damage);

        charHealthComp->subtractAmount(damage);
        charStatusComp->setDamaged(true);
        printf("new Player Health: %d \n", charHealthComp->getAmount());

        if (charHealthComp->getAmount() == 0) {
            const auto &entityManager = game->getEntityManager();

            auto ownerComp = entity->getComponent<OwnerComponent>();
            auto killerEntity = entityManager.getEntity(ownerComp->getEntity()->getId());
            auto victimEntity = entityManager.getEntity(nearbyEntity->getId());

            Player *killer = killerEntity->getComponent<PlayerComponent>()->getPlayer();
            Player *victim = victimEntity->getComponent<PlayerComponent>()->getPlayer();

            if (killer->getId() != victim->getId()) {
                killer->setKills(killer->getKills() + 1);
                game->getGameServer().sendChatEvent(victim->getName() + " was killed by " + killer->getName() + ".");
            } else {
                game->getGameServer().sendChatEvent(victim->getName() + " committed suicide.");
            }

            victim->setDeaths(victim->getDeaths() + 1);

            game->getGameServer().sendScoreEvent(killer);
            game->getGameServer().sendScoreEvent(victim);
        }

		//message.getGame()->getGameServer().sendPlayerStatusEvent(nearbyEntity);
    }
	
    game->getGameServer().sendExplosionEvent(entity);
    entity->attachComponent(new DestroyComponent());

    return true;
}
