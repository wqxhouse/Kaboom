#include "BombContainerComponent.h"

#include "CharacteristicComponent.h"
#include "Entity.h"

BombContainerComponent::BombContainerComponent(unsigned int entityId) 
:	entityId(entityId){

	//default bombs and bomb fire rate, hardcoded right now
	for (BombType bombType : BombTypeArr){ //hardcode, or we can randomize it here
		bombInventoryMap[bombType] = 10;// 10 # of bombs for each type for now
		bombCoolDownMap[bombType] = new CoolDown(500);//all the bombs has a 500ms cooldown
	}
}

BombContainerComponent::~BombContainerComponent() {
	for (BombType bombType : BombTypeArr) {
		delete bombCoolDownMap[bombType]; //prevent memory leak
	}
}

void BombContainerComponent::addBombToInv(BombType bombType) {
	addBombToInv(bombType, 1);
}
void BombContainerComponent::addBombToInv(BombType bombType, unsigned int howMany) {
	//todo add uppper limit here
	bombInventoryMap[bombType] += howMany;
}

void BombContainerComponent::deleteBombInInv(BombType bombType) {
	deleteBombInInv(bombType, 1);
}
void BombContainerComponent::deleteBombInInv(BombType bombType, unsigned int howMany) {
	//todo add uppper limit here
	if (bombInventoryMap[bombType] - howMany < 0){
		bombInventoryMap[bombType] = 0;
	} else {
		bombInventoryMap[bombType] -= howMany;
	}

}

unsigned int BombContainerComponent::getNumOfBomb(BombType bombType){
	return bombInventoryMap[bombType];
};

void BombContainerComponent::addBombInActiveBomb(Entity * bomb){
	activeBombs.push_back(bomb);
}
void BombContainerComponent::deleteBombInActiveBomb(Entity * bomb){
	for (std::vector<Entity*>::iterator it = activeBombs.begin(); it != activeBombs.end(); ++it) {
		if ((*it)->getId() == bomb->getId()){
			activeBombs.erase(it);
		}
	}
}
CoolDown * BombContainerComponent::getBombCoolDown(BombType bombtype) {
	return bombCoolDownMap[bombtype];
}
/*
Entity *BombContainerComponent::fireBombIfValid(BombType bombType, FireMode fireMode) {

	switch (fireMode) {
	case FireMode::LEFT_CLICK:
		

		break;
	case FireMode::RIGHT_CLICK:
		break;
	}



    Entity *bomb = nullptr;
	
	if (bombInventoryMap[bombType] > 0){
		//create the bomb entity here
		bomb = activeBombs.push_back(bombInventory[selectedBomb]);
	}
    if (bombInventory.size()>0) {
        ;
        bomb = bombInventory[selectedBomb];
        bombInventory.erase(bombInventory.begin() + selectedBomb);
    }

    return bomb;
}*/