#include "FiringSystem.h"

#include <btBulletDynamicsCommon.h>

#include <core/FireMode.h>
#include <core/PositionComponent.h>
#include <core/RotationComponent.h>
#include <util/Timer.h>

#include "BombDataLookup.h"
#include "InputComponent.h"
#include "Game.h"
#include "../math/Conversion.h"

//repetitive stuff, should move this to global.h
#define PI 3.14159265359
#define deg2rad(d) (PI / 180.0 * d)
#define VELOCITYCAP 2
#define VELOCTIYACCELERATION .1

FiringSystem::FiringSystem(Game *game)
        : EntitySystem(game) {
}

void FiringSystem::update(float timeStep) {
    //find player entity,
    //find input entity,
    //find bombContainer component
    //checks for firing, add cooldown

    auto entities = game->getEntityManager().getEntityList();

    for (Entity *entity : entities) {
        if ((entity->getType() & CAT_CHARACTER) != CAT_CHARACTER) {
            continue;
        }

        InputComponent* inputComp = entity->getComponent<InputComponent>();

        if (!inputComp->isFiring() || inputComp->getFireMode() == NOT_FIRING) {
            continue;
        }

        BombContainerComponent* invComp = entity->getComponent<BombContainerComponent>();
        PositionComponent* posComp = entity->getComponent<PositionComponent>();
        RotationComponent* rotComp = entity->getComponent<RotationComponent>();

        const EntityType &bombType = inputComp->getSelectedBombType();

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
            //todo
        }
    }
}
