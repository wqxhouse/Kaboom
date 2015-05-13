#include "DefaultCharacterMessageHandler.h"

#include <btBulletDynamicsCommon.h>

#include <components/BombContainerComponent.h>
#include <components/EquipmentComponent.h>
#include <components/PositionComponent.h>
#include <components/RotationComponent.h>
#include <core/Entity.h>
#include <util/Configuration.h>

#include "Attack1Message.h"
#include "Message.h"
#include "MessageType.h"
#include "NoAttackMessage.h"
#include "../components/DetonatorComponent.h"
#include "../components/ExplosionComponent.h"
#include "../components/OwnerComponent.h"
#include "../core/EntityConfigLookup.h"
#include "../core/Game.h"
#include "../math/util.h"

bool DefaultCharacterMessageHandler::handle(const Message &message) const {
    switch (message.getType()) {
        case MessageType::ATTACK1: {
            return handle(static_cast<const Attack1Message &>(message));
        }
        case MessageType::NO_ATTACK: {
            return handle(static_cast<const NoAttackMessage &>(message));
        }
    }

    return false;
}

bool DefaultCharacterMessageHandler::handle(const Attack1Message &message) const {
    Entity *entity = message.getEntity();
    Game *game = message.getGame();

    auto equipComp = entity->getComponent<EquipmentComponent>();
    auto detonatorComp = entity->getComponent<DetonatorComponent>();
    auto invComp = entity->getComponent<BombContainerComponent>();
    auto posComp = entity->getComponent<PositionComponent>();
    auto rotComp = entity->getComponent<RotationComponent>();

    const EntityType &bombType = equipComp->getEquipmentType();

    if (!invComp->hasBomb(bombType)) {
        return true;
    }

    const Configuration &bombConfig = EntityConfigLookup::instance()[bombType];

    if (bombType == REMOTE_DETONATOR && detonatorComp != nullptr) {
        if (detonatorComp->isReady() && !detonatorComp->isDetonated()) {
            detonatorComp->getBomb()->attachComponent(new ExplosionComponent());
            detonatorComp->setDetonated(true);
        }
    } else {
        Timer &timer = invComp->getTimer(bombType);

        if (invComp->getAmount(bombType) > 0 && timer.isExpired()) {
            invComp->removeFromInventory(bombType);
            timer.start();

            btVector3 viewDir = getViewDirection(
                    posComp->getX(),
                    posComp->getY(),
                    posComp->getZ(),
                    rotComp->getYaw(),
                    rotComp->getPitch());

            float launchSpeed = bombConfig.getFloat("launch-speed");

            Entity* bombEntity = game->getBombFactory().createBomb(
                    bombType,
                    posComp->getX() + viewDir.getX(),
                    posComp->getY() + viewDir.getY(),
                    posComp->getZ() + 1 + viewDir.getZ(),
                    viewDir.getX() * launchSpeed,
                    viewDir.getY() * launchSpeed,
                    viewDir.getZ() * launchSpeed);
            bombEntity->attachComponent(new OwnerComponent(entity));

            if (bombType == REMOTE_DETONATOR) {
                entity->attachComponent(new DetonatorComponent(bombEntity));
            }

            invComp->addToActiveBomb(bombEntity);

            game->addEntity(bombEntity);
            game->getGameServer().sendSpawnEvent(bombEntity);
        }
    }

    return true;
}

bool DefaultCharacterMessageHandler::handle(const NoAttackMessage &message) const {
    Entity *entity = message.getEntity();

    auto detonatorComp = entity->getComponent<DetonatorComponent>();

    if (detonatorComp != nullptr) {
        if (detonatorComp->isDetonated()) {
            entity->detachComponent<DetonatorComponent>();
            delete detonatorComp;
        } else {
            detonatorComp->setReady(true);
        }
    }

    return true;
}
