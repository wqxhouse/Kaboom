#include "FiringSystem.h"

#include <btBulletDynamicsCommon.h>

#include <components/EquipmentComponent.h>
#include <components/PositionComponent.h>
#include <components/RotationComponent.h>
#include <core/FireMode.h>
#include <util/Timer.h>

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

    if (!inputComp->isFiring() || inputComp->getFireMode() == NOT_FIRING) {
        return;
    }

    const EntityType &bombType = equipComp->getEquipmentType();

    if (inputComp->getFireMode() == FireMode::LEFT_CLICK) {
        const Configuration &bombConfig = EntityConfigLookup::instance()[bombType];

        if (invComp->hasBomb(bombType)) {
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

                float launchSpeed = bombConfig.getInt("launch-speed");

                Entity* bombEntity = game->getBombFactory().createBomb(
                        bombType,
                        posComp->getX() + viewDir.getX(),
                        posComp->getY() + viewDir.getY(),
                        posComp->getZ() + viewDir.getZ(),
                        viewDir.getX() * launchSpeed,
                        viewDir.getY() * launchSpeed,
                        viewDir.getZ() * launchSpeed);

                invComp->addToActiveBomb(bombEntity);

                game->addEntity(bombEntity);
                game->getGameServer().sendSpawnEvent(bombEntity);
            }
        }
    } else if (inputComp->getFireMode() == FireMode::RIGHT_CLICK) {
        // TODO
    }
}
