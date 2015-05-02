#include "FiringSystem.h"

#include <btBulletDynamicsCommon.h>
#include <core/FireMode.h>
#include <core/PositionComponent.h>
#include <core/RotationComponent.h>

#include "BombDataLookup.h"
#include "InputComponent.h"
#include "Game.h"

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
        if ((entity->getType() & CAT_PLAYER) != CAT_PLAYER) {
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
            Stopwatch &stopwatch = invComp->getStopwatch(bombType);
            const int cooldown = BombDataLookup::instance[bombType].cooldown;
            auto timeelapsed = stopwatch.getTimeElapsed();
            if (invComp->getSize(bombType) > 0 && stopwatch.getTimeElapsed() > cooldown) {
                invComp->removeFromInventory(bombType);
                stopwatch.start();

                // forward & backward & left & right w.r.t orientation
                btQuaternion rot0;
                btQuaternion rot1;
                btScalar yaw = rotComp->getYaw();
                btScalar pitch = rotComp->getPitch();
                rot0.setRotation(btVector3(0, 0, 1), deg2rad(-yaw));
                rot1.setRotation(btVector3(1, 0, 0), deg2rad(pitch));
                btQuaternion rot = rot0 * rot1; // order different from osg::Quat
                btVector3 viewDir = quatRotate(rot, btVector3(0, 1, 0));


                Entity* bombEntity = game->getBombFactory().createBomb(
                    bombType,
                    posComp->getX() + viewDir.getX(),
                    posComp->getY() + viewDir.getY(),
                    posComp->getZ() + viewDir.getZ(),
                    viewDir.getX() * 5, // TODO: Change launch speed
                    viewDir.getY() * 5,
                    viewDir.getZ() * 5);

                invComp->addToActiveBomb(bombEntity);

                game->addEntity(bombEntity);
                game->getGameServer().sendSpawnEvent(bombEntity);
            }
        } else if (inputComp->getFireMode() == FireMode::RIGHT_CLICK) {
            //todo
        }
    }
}
