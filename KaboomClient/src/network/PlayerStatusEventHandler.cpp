#include "PlayerStatusEventHandler.h"

#include <components/PlayerStatusComponent.h>
#include "../components/ModelComponent.h"
#include "../components/SoundComponent.h"
#include <components/PositionComponent.h>
#include <components/RotationComponent.h>
#include <core/Entity.h>
#include <network/PlayerStatusEvent.h>

#include "../core/Game.h"
#include <Core.h>			// For animation type-id constants

PlayerStatusEventHandler::PlayerStatusEventHandler(Game *game)
        : game(game) {
}

void PlayerStatusEventHandler::handle(const Event &e) const {
    const PlayerStatusEvent &evt = static_cast<const PlayerStatusEvent &>(e);
	clock_t beginTime;
    Entity *entity = game->getEntityManager().getEntity(evt.getEntityId());
	if (entity == nullptr){
		return;
	}
	if (evt.isRunning()){
		beginTime = clock();
	}
    auto playerStatusComp = entity->getComponent<PlayerStatusComponent>();
    playerStatusComp->setAlive(evt.isAlive());
    playerStatusComp->setRunning(evt.isRunning());
    playerStatusComp->setJumping(evt.isJumping());
    playerStatusComp->setAttacking(evt.isAttacking());
    playerStatusComp->setDamaged(evt.isDamaged());
	EntityType type = entity->getType();
	Entity *player = game->getCurrentPlayer()->getEntity();
	auto bombPos = entity->getComponent<PositionComponent>()->getPosition();
	auto playerRot = player->getComponent<RotationComponent>()->getRotation();
	auto playerPos = player->getComponent<PositionComponent>()->getPosition();
	game->getSoundManager().setListenerPosition(playerPos);
	game->getSoundManager().setListenerRotation(playerRot);
	auto soundComp = entity->getComponent<SoundComponent>();
	if (evt.isRunning()&&!evt.isJumping()){
		//game->getSoundManager().playSound(SoundType::WALKING, bombPos);
		clock_t t = clock();
		soundComp->setListenerPosition(playerPos);
		soundComp->setListenerRotation(playerRot);
		soundComp->playSound(bombPos);
		std::cout <<"clock time"<< clock() - t << std::endl;
		std::cout << "afterwards" << std::endl;
	}
	else{
		soundComp->stopSound();
	}
	if (evt.isJumping()&&!playerStatusComp->getJumped()){
		game->getSoundManager().playSound(SoundType::JUMP, bombPos);
	}
	
		playerStatusComp->setJumped(evt.isJumping());
	

	auto modelComp = entity->getComponent<ModelComponent>();
	Model* model = modelComp->getModel();

	if (evt.isRunning()) {
		clock_t t = clock();
		model->playAnimation(RUNNING);
		std::cout << "clock time" << ((float)clock() - (float)beginTime)/CLOCKS_PER_SEC << std::endl;
		std::cout << "afterwards" << std::endl;
	}
	else {
		model->playAnimation(IDLE);
	}
}
