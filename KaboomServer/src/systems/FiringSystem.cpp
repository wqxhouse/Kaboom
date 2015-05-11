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
#include "../core/EntityConfigLookup.h"
#include "../core/Game.h"
#include "../math/util.h"

#define VELOCITYCAP 2
#define VELOCTIYACCELERATION .1

FiringSystem::FiringSystem(Game *game)
        : game(game) {
}

bool FiringSystem::checkEntity(Entity *entity) {
    return entity->hasComponent<PositionComponent>() &&
            entity->hasComponent<RotationComponent>() &&
            entity->hasComponent<BombContainerComponent>() &&
            entity->hasComponent<EquipmentComponent>() &&
            entity->hasComponent<InputComponent>();
}

void FiringSystem::processEntity(Entity *entity) {
    PositionComponent* posComp = entity->getComponent<PositionComponent>();
    RotationComponent* rotComp = entity->getComponent<RotationComponent>();
    BombContainerComponent* invComp = entity->getComponent<BombContainerComponent>();
    EquipmentComponent *equipComp = entity->getComponent<EquipmentComponent>();
    InputComponent* inputComp = entity->getComponent<InputComponent>();
    DetonatorComponent *detonatorComp = entity->getComponent<DetonatorComponent>();

    if (!inputComp->isAttacking1() && inputComp->isAttacking2()) {
        if (detonatorComp != nullptr) {
            if (detonatorComp->isDetonated()) {
                entity->detachComponent<DetonatorComponent>();
                delete detonatorComp;
            } else {
                detonatorComp->setReady(true);
            }
        }

        return;
    }

    const EntityType &bombType = equipComp->getEquipmentType();

    if (inputComp->isAttacking1()) {
        const Configuration &bombConfig = EntityConfigLookup::instance()[bombType];

        if (invComp->hasBomb(bombType)) {
            if (detonatorComp != nullptr) {
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
                            posComp->getZ() + viewDir.getZ(),
                            viewDir.getX() * launchSpeed,
                            viewDir.getY() * launchSpeed,
                            viewDir.getZ() * launchSpeed);

                    if (bombType == REMOTE_DETONATOR) {
                        entity->attachComponent(new DetonatorComponent(bombEntity));
                    }

                    invComp->addToActiveBomb(bombEntity);

                    game->addEntity(bombEntity);
                    game->getGameServer().sendSpawnEvent(bombEntity);
                }
            }

        }
    } else if (inputComp->isAttacking2()) {
        // TODO
    }
}
