#include "DeathSystem.h"

#include <components/HealthComponent.h>
#include <components/PositionComponent.h>
#include <components/PlayerStatusComponent.h>

#include "../components/PhysicsComponent.h"
#include <core/Entity.h>

DeathSystem::DeathSystem(Game * game) 
	: game(game){
}

bool DeathSystem::checkEntity(Entity *entity) {
    return entity->hasComponent<HealthComponent>()  &&
		   entity->hasComponent<PlayerStatusComponent>() &&
		   entity->hasComponent<PhysicsComponent>(); //this is going to be removed, the first time the player dies.
}

void DeathSystem::processEntity(Entity *entity) {
    auto healthComp = entity->getComponent<HealthComponent>();
	auto playerStatusComp = entity->getComponent<PlayerStatusComponent>();
	auto physicComp = entity->getComponent<PhysicsComponent>();

	if (healthComp->getAmount() == 0){
		playerStatusComp->setIsAlive(false);
		//game->getGameServer().sendPlayerStatusEvent(entity);
		//game->getWorld().removeRigidBody(physicComp->getRigidBody());
		//entity->detachComponent<PhysicsComponent>(); //just remove the physicsComponent for now, we might want to attach a spectator component, or local camera on the client
	}
}
