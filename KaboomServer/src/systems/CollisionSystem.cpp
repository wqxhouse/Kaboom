#include "CollisionSystem.h"

#include <core/Entity.h>

#include "../components/CollisionComponent.h"
#include "../components/MessageHandlerComponent.h"
#include "../messaging/CollisionMessage.h"

CollisionSystem::CollisionSystem(Game *game)
        : game(game) {
}

bool CollisionSystem::checkEntity(Entity *entity) {
    return entity->hasComponent<CollisionComponent>() &&
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
