#include "JumpPadSystem.h"

#include <core/Entity.h>

#include "../components/DestroyComponent.h"
#include "../components/MessageHandlerComponent.h"
#include "../components/TriggerComponent.h"
#include "../messaging/MessageHandler.h"
#include "../messaging/JumpPadMessage.h"

class JumpPadComponent;

JumpPadSystem::JumpPadSystem(Game *game)
	: EntityProcessingSystem(game) {
}

bool JumpPadSystem::checkEntity(Entity *entity) {
    return !entity->hasComponent<DestroyComponent>() && 
            entity->hasComponent<JumpPadComponent>() &&
            entity->hasComponent<TriggerComponent>() &&
            entity->hasComponent<MessageHandlerComponent>();
}

void JumpPadSystem::processEntity(Entity *entity) {
    auto triggerComp = entity->getComponent<TriggerComponent>();
    auto handlerComop = entity->getComponent<MessageHandlerComponent>();

    auto triggerEntities = triggerComp->getTriggerEntities();

    JumpPadMessage message(game, entity, triggerEntities);
    handlerComop->getHandler()->handle(message);
}
