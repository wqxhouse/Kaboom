#include "CollisionSystem.h"

#include <core/Entity.h>

#include "../components/CollisionComponent.h"
#include "../components/DestroyComponent.h"
#include "../components/MessageHandlerComponent.h"
#include "../messaging/CollisionMessage.h"
#include "../messaging/MessageHandler.h"

CollisionSystem::CollisionSystem(Game *game)
        : EntityProcessingSystem(game) {
}

bool CollisionSystem::checkEntity(Entity *entity) {
    return !entity->hasComponent<DestroyComponent>() &&
            entity->hasComponent<CollisionComponent>() &&
            entity->hasComponent<MessageHandlerComponent>();
}

void CollisionSystem::processEntity(Entity *entity) {
    auto colComp = entity->getComponent<CollisionComponent>();
    auto handlerComp = entity->getComponent<MessageHandlerComponent>();

    if (colComp->isCollided()) {
        CollisionMessage message(game, entity, colComp->getContactEntities());
        handlerComp->getHandler()->handle(message);
    }
}
