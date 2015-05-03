#include "BombContainerComponent.h"

#include "Entity.h"

BombContainerComponent::BombContainerComponent(const InventoryType &inventory)
        : capacity(0),
          inventory(inventory) {
}

void BombContainerComponent::addToInventory(const EntityType &bombType, int amount) {
    inventory[bombType].first += amount;
}

void BombContainerComponent::removeFromInventory(const EntityType &bombType, int amount) {
    if (inventory[bombType].first - amount < 0) {
        inventory.erase(bombType);
    } else {
        inventory[bombType].first -= amount;
    }
}

bool BombContainerComponent::hasBomb(const EntityType &bombType) const {
    return inventory.count(bombType) > 0;
}

int BombContainerComponent::getAmount(const EntityType &bombType) const {
    return inventory.at(bombType).first;
};

Timer &BombContainerComponent::getTimer(const EntityType &bombType) {
    return inventory.at(bombType).second;
}

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