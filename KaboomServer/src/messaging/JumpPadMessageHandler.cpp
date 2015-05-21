#include "JumpPadMessageHandler.h"

#include <limits>

#include <components/BombContainerComponent.h>
#include <components/PositionComponent.h>
#include <components/WeaponPickupComponent.h>
#include <components/PlayerStatusComponent.h>
#include <core/Entity.h>

#include "Message.h"
#include "MessageType.h"
#include "JumpPadMessage.h"
#include "../components/DestroyComponent.h"
#include "../components/RespawnComponent.h"
#include "../components/PhysicsComponent.h"
#include "../components/JumpPadComponent.h"
#include "../components/InputComponent.h"
#include "../components/JumpComponent.h"

#include "../core/EntityConfigLookup.h"
#include "../core/Game.h"
#include "../math/util.h"

bool JumpPadMessageHandler::handle(const Message &message) const {
    if (message.getType() == MessageType::JUMP_PAD) {
		return handle(static_cast<const JumpPadMessage &>(message));
    }

    return false;
}

bool JumpPadMessageHandler::handle(const JumpPadMessage &message) const {
    Entity *jumpPad = message.getEntity();
	auto posComp = jumpPad->getComponent<PositionComponent>();

    Entity *closestEntity = nullptr;
    float minDistance = FLT_MAX;

    auto &nearbyEntities = message.getNearbyEntities();

    for (Entity *character : nearbyEntities) {
        auto charPosComp = character->getComponent<PositionComponent>();

		//make sure the entity is a player, and that the player is not dead or disconnected
		if (charPosComp == nullptr || 
			!character->hasComponent<InputComponent>()  || 
			character->hasComponent<RespawnComponent>() || 
			character->hasComponent<DestroyComponent>()) {
            continue;
        }

        //apply the jumping force here
		auto charPhysicComp = character->getComponent<PhysicsComponent>();
		auto charStatusComp = character->getComponent<PlayerStatusComponent>();
		auto jumpPadComp = jumpPad->getComponent<JumpPadComponent>();
		auto charJumpComp = character->getComponent<JumpComponent>();
		
		if (charJumpComp != nullptr && charJumpComp->checkIsOkToLaunch()) {
			//this is need to make sure we don't get launched twice if we touch the same jump pad for more than a split second
			//charJumpComp->setLaunched(true); 
			charJumpComp->getIsLaunchingTimer().setDuration(jumpPadComp->getLaunchDuration());
			charJumpComp->getIsLaunchingTimer().start();

			charPhysicComp->getRigidBody()->applyCentralImpulse(jumpPadComp->getLaunchSpeedVec()); //apply the jumpPad force

			//charStatusComp->getKnockBackTimer().setDuration(jumpPadComp->getLaunchDuration());
			//charStatusComp->getKnockBackTimer().start();
			//charStatusComp->setIsKnockBacked(true);
		}
    }
    return true;
}
