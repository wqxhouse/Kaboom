#include "FiringSystem.h"

#include <btBulletDynamicsCommon.h>
#include <core/FireMode.h>
#include <core/PositionComponent.h>
#include <core/RotationComponent.h>

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

FiringSystem::~FiringSystem() {
}

void FiringSystem::update(float timeStep) {
	//find player entity,
	//find input entity,
	//find bombContainer component
	//checks for firing, add cooldown

    auto entities = game->getEntityManager().getEntityList();

    for (Entity *entity : entities) {
		if (entity->getComponent<InputComponent>() == nullptr||
			entity->getComponent<BombContainerComponent>() == nullptr ||
			entity->getComponent<PositionComponent>() == nullptr ||
			entity->getComponent<RotationComponent>() == nullptr) {
            continue;
        }

		InputComponent* inputComponent = entity->getComponent<InputComponent>();
		BombContainerComponent* bombComponent = entity->getComponent<BombContainerComponent>();
		PositionComponent* positionComponent = entity->getComponent<PositionComponent>();
		RotationComponent* rotationComponent = entity->getComponent<RotationComponent>();

		if (inputComponent->isFiring() && inputComponent->getFireMode() != FireMode::NOT_FIRING ){

            EntityType selectedBombType = inputComponent->getSelectedBombType();

			if (inputComponent->getFireMode() == FireMode::LEFT_CLICK) {

				if (bombComponent->getSize(selectedBombType) > 0 && 
					rand() % 10 == 0) {//bombComponent->getBombCoolDown(selectedBombType)->isReady()) { // TODO: Fix cooldown

					bombComponent->removeFromInventory(selectedBombType);

					// forward & backward & left & right w.r.t orientation
					btQuaternion rot0;
					btQuaternion rot1;
					btScalar yaw = rotationComponent->getYaw();
					btScalar pitch = rotationComponent->getPitch();
					rot0.setRotation(btVector3(0, 0, 1), deg2rad(-yaw));
					rot1.setRotation(btVector3(1, 0, 0), deg2rad(pitch));
					btQuaternion rot = rot0 * rot1; // order different from osg::Quat
					btVector3 viewDir = quatRotate(rot, btVector3(0, 1, 0));


					Entity* bombEntity = game->getBombFactory().createBomb(selectedBombType,
																		   positionComponent->getX() + viewDir.getX(),
																		   positionComponent->getY() + viewDir.getY(),
																		   positionComponent->getZ() + viewDir.getZ(),
																		   viewDir.getX() * 5, // TODO: Change launch speed
																		   viewDir.getY() * 5,
																		   viewDir.getZ() * 5);

					bombComponent->addToActiveBomb(bombEntity);

					game->addEntity(bombEntity);
					game->getGameServer().sendSpawnEvent(bombEntity);
				}
			}
			else if (inputComponent->getFireMode() == FireMode::RIGHT_CLICK) {
				//todo
			}




		}



    }
}
