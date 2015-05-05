#include "FiringSystem.h"

#include <btBulletDynamicsCommon.h>

#include <core/EquipmentComponent.h>
#include <core/FireMode.h>
#include <core/PositionComponent.h>
#include <core/RotationComponent.h>
#include <util/Timer.h>

#include "BombDataLookup.h"
#include "Game.h"
#include "../components/InputComponent.h"
#include "../math/Conversion.h"

#define VELOCITYCAP 2
#define VELOCTIYACCELERATION .1

FiringSystem::FiringSystem(Game *game)
        : EntitySystem(game) {
}

void FiringSystem::update(float timeStep) {
    auto entities = game->getEntityManager().getEntityList();

    for (Entity *entity : entities) {
        PositionComponent* posComp = entity->getComponent<PositionComponent>();
        RotationComponent* rotComp = entity->getComponent<RotationComponent>();
        BombContainerComponent* invComp = entity->getComponent<BombContainerComponent>();
        EquipmentComponent *equipComp = entity->getComponent<EquipmentComponent>();
        InputComponent* inputComp = entity->getComponent<InputComponent>();

        if (posComp == nullptr ||
                rotComp == nullptr ||
                invComp == nullptr ||
                equipComp == nullptr ||
                inputComp == nullptr) {
            continue;
        }

        if (!inputComp->isFiring() || inputComp->getFireMode() == NOT_FIRING) {
            continue;
        }

        const EntityType &bombType = equipComp->getEquipmentType();

        if (inputComp->getFireMode() == FireMode::LEFT_CLICK) {
            const BombData &bombData = BombDataLookup::instance[bombType];

            if (!invComp->hasBomb(bombType)) {
                continue;
            }

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

                Entity* bombEntity = game->getBombFactory().createBomb(
                        bombType,
                        posComp->getX() + viewDir.getX(),
                        posComp->getY() + viewDir.getY(),
                        posComp->getZ() + viewDir.getZ(),
                        viewDir.getX() * bombData.launchSpeed,
                        viewDir.getY() * bombData.launchSpeed,
                        viewDir.getZ() * bombData.launchSpeed);

                invComp->addToActiveBomb(bombEntity);

                game->addEntity(bombEntity);
                game->getGameServer().sendSpawnEvent(bombEntity);
            }
        } else if (inputComp->getFireMode() == FireMode::RIGHT_CLICK) {
            // TODO
        }
    }
}
