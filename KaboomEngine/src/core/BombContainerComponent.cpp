#include "BombContainerComponent.h"

#include "Entity.h"

void BombContainerComponent::addToInventory(const EntityType &bombType, unsigned int amount) {
    inventory[bombType] += amount;
}

void BombContainerComponent::removeFromInventory(const EntityType &bombType, unsigned int amount) {
    if (inventory[bombType] - amount < 0) {
        inventory[bombType] = 0;
    } else {
        inventory[bombType] -= amount;
    }
}

unsigned int BombContainerComponent::getSize(const EntityType &bombType) const {
    if (inventory.count(bombType) > 0) {
        return inventory.at(bombType);
    } else {
        return 0;
    }
};

void BombContainerComponent::addToActiveBomb(Entity *bomb) {
    activeBombs.push_back(bomb);
}

void BombContainerComponent::removeFromActiveBomb(Entity *bomb) {
    activeBombs.erase(std::find(activeBombs.begin(), activeBombs.end(), bomb));
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