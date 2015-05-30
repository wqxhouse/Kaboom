#include "InputSystem.h"

#include <btBulletDynamicsCommon.h>

#include <components/PositionComponent.h>
#include <components/PlayerStatusComponent.h>
#include <components/RotationComponent.h>
#include <core/EntityManager.h>

#include "../components/DestroyComponent.h"
#include "../components/PlayerRespawnComponent.h"
#include "../components/InputComponent.h"
#include "../components/PhysicsComponent.h"
#include "../components/JetpackComponent.h"
#include "../components/JumpComponent.h"
#include "../core/EntityConfigLookup.h"
#include "../core/Game.h"
#include "../math/util.h"

#define VELOCITYCAP 2
#define VELOCTIYACCELERATION .1

InputSystem::InputSystem(Game *game)
	: EntityProcessingSystem(game) {
}

bool InputSystem::checkEntity(Entity *entity) {
    return game->getGameMode().getMatchState() == GameMode::MatchState::IN_PROGRESS &&
            !entity->hasComponent<DestroyComponent>() &&
			!entity->hasComponent<PlayerRespawnComponent>() &&
		    entity->hasComponent<PositionComponent>() &&
		    entity->hasComponent<RotationComponent>() &&
		    entity->hasComponent<PhysicsComponent>() &&
		    entity->hasComponent<InputComponent>() &&
		    entity->hasComponent<PlayerStatusComponent>();
}

void InputSystem::processEntity(Entity *entity) {
	auto inputComp = entity->getComponent<InputComponent>();
	auto posComp = entity->getComponent<PositionComponent>();
	auto rotComp = entity->getComponent<RotationComponent>();
	auto physComp = entity->getComponent<PhysicsComponent>();
	auto playerStatusComp = entity->getComponent<PlayerStatusComponent>();

	btRigidBody *rigidBody = physComp->getRigidBody();
	btVector3 velocity = rigidBody->getLinearVelocity();

	// TODO: currently, forward & backward event will override 
	// the front and right velocity generated by the physics.
	// Only velocity on the z axis is taken from the physics calc

    Vec3 walkDir = getViewDirection(rotComp->getRotation());

	// construct local coordinate system
    btVector3 front = btVector3(walkDir.x, walkDir.y, walkDir.z);
	btVector3 right = front.cross(btVector3(0, 0, 1));
	right.normalize();
	// do not calculate head vector, since gravity is always neg z

	btVector3 localVelocity(0.0, 0.0, 0.0);

	JumpComponent *jumpCom = entity->getComponent<JumpComponent>();
	bool jumping = false;
	//if (jumpCom != NULL) {
		if (!jumpCom->isJumping() && inputComp->isJumping() && !jumpCom->checkIsLaunchByJumpPad()) {
			const float jumpingSpeed = EntityConfigLookup::get(entity->getType()).getFloat("jumping-speed");

			velocity.setZ(velocity.getZ() + jumpingSpeed);
			jumpCom->setJumping(true);
			jumping = true;
		}
	//}

	// Define y to be front speed, x to be right speed
	if (!jumping) {
		const float runningSpeed = EntityConfigLookup::get(entity->getType()).getFloat("running-speed");

		if (inputComp->isMovingForward()) {
			localVelocity.setY(runningSpeed);
		} else if (inputComp->isMovingBackward()) {
			localVelocity.setY(-runningSpeed);
		} else {
			localVelocity.setY(0);
		}

		if (inputComp->isMovingLeft()) {
			localVelocity.setX(-runningSpeed);
		} else if (inputComp->isMovingRight()) {
			localVelocity.setX(runningSpeed);
		} else {
			localVelocity.setX(0);
		}
	}

	/*    JetpackComponent *jetComp = entity->getComponent<JetpackComponent>();

	if (jetComp != nullptr) {
	if (inputComp->isJumping()) {

	if (!jetComp->activateJetpack()) {
	jetComp->refillJetpack();
	} else {
	if (velocity.getZ() > VELOCITYCAP) {
	velocity.setZ(VELOCITYCAP);
	} else {
	velocity.setZ(velocity.getZ() + VELOCTIYACCELERATION);
	}
	}
	} else {
	jetComp->refillJetpack();
	}
	}*/




	btVector3 worldVelocity(right * localVelocity.getX() + front * localVelocity.getY());
	worldVelocity.setZ(velocity.getZ());

	if (!playerStatusComp->checkIsKnockBacked() && /*jumpCom != NULL &&*/ !jumpCom->checkIsLaunchByJumpPad()) { //don't move if we are knockbacked, or got launched by jumpPad
		rigidBody->setLinearVelocity(worldVelocity);
	}
}