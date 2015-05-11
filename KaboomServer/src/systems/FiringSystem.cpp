#include "FiringSystem.h"

#include <btBulletDynamicsCommon.h>

#include <components/EquipmentComponent.h>
#include <components/PositionComponent.h>
#include <components/RotationComponent.h>
#include <core/FireMode.h>
#include <util/Timer.h>

#include "../components/DetonatorComponent.h"
#include "../components/ExplosionComponent.h"
#include "../components/InputComponent.h"
#include "../components/MessageHandlerComponent.h"
#include "../core/EntityConfigLookup.h"
#include "../core/Game.h"
#include "../math/util.h"
#include "../messaging/Attack1Message.h"
#include "../messaging/Attack2Message.h"
#include "../messaging/NoAttackMessage.h"

#define VELOCITYCAP 2
#define VELOCTIYACCELERATION .1

FiringSystem::FiringSystem(Game *game)
        : game(game) {
}

bool FiringSystem::checkEntity(Entity *entity) {
    return entity->hasComponent<InputComponent>() &&
            entity->hasComponent<MessageHandlerComponent>();
}

void FiringSystem::processEntity(Entity *entity) {
    InputComponent* inputComp = entity->getComponent<InputComponent>();
    MessageHandlerComponent *handlerComp = entity->getComponent<MessageHandlerComponent>();

    if (!inputComp->isAttacking1() && !inputComp->isAttacking2()) {
        NoAttackMessage message(game, entity);
        handlerComp->getHandler()->handle(message);
    } else {
        if (inputComp->isAttacking1()) {
            Attack1Message message(game, entity);
            handlerComp->getHandler()->handle(message);
        }

        if (inputComp->isAttacking2()) {
            Attack2Message message(game, entity);
            handlerComp->getHandler()->handle(message);
        }
    }
}
