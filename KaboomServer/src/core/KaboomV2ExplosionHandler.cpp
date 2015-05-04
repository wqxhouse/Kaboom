#include "KaboomV2ExplosionHandler.h"

#include <btBulletDynamicsCommon.h>

#include <core/Entity.h>
#include <core/PlayerStatusComponent.h>

#include "BombData.h"
#include "BombDataLookup.h"
#include "ExplosionComponent.h"
#include "Game.h"
#include "PhysicsComponent.h"
#include "TriggerComponent.h"

void KaboomV2ExplosionHandler::handle(
        Game *game,
        Entity *entity,
        const std::unordered_set<Entity *> &nearbyEntities) {
    TriggerComponent* bombTriggerComp = entity->getComponent<TriggerComponent>();

    for (auto nearbyEntity : nearbyEntities) {
        PlayerStatusComponent* playerStatusComp = nearbyEntity->getComponent<PlayerStatusComponent>();
        PhysicsComponent* playerPhysicsComp = nearbyEntity->getComponent<PhysicsComponent>();

        if (playerStatusComp == nullptr || playerPhysicsComp == nullptr) {
            continue;
        }

        //begin timer on knockback, during this time the player cannot move

        btVector3 explosionPos = bombTriggerComp->getGhostObject()->getWorldTransform().getOrigin();
        btVector3 playerPos = playerPhysicsComp->getRigidBody()->getCenterOfMassPosition();

        btVector3 dirVec = btVector3(playerPos - explosionPos);
        dirVec.normalize();

        btScalar distFromExplosion = playerPos.distance(explosionPos);


        const BombData &bombData = BombDataLookup::instance[entity->getType()];

        btVector3 impulseVec = (bombData.knockbackAmount / distFromExplosion) * dirVec; //inverse function

        playerPhysicsComp->getRigidBody()->applyCentralImpulse(impulseVec);

        playerStatusComp->getKnockBackTimer().setDuration(bombData.knockbackDuration);
        playerStatusComp->getKnockBackTimer().start();
        playerStatusComp->setIsKnockBacked(true);

        //TODO apply damage base on dist
    }

    game->getGameServer().sendExplosionEvent(entity);
    game->removeEntity(entity);
}
