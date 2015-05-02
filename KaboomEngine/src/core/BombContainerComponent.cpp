#include "BombContainerComponent.h"

#include "Entity.h"

BombContainerComponent::BombContainerComponent(const InventoryType &inventory)
        : capacity(0),
          inventory(inventory) {
    for (auto kv : inventory) {
        Stopwatch stopwatch;
        stopwatch.start();
        stopwatches[kv.first] = stopwatch;
    }
}

void BombContainerComponent::addToInventory(const EntityType &bombType, int amount) {
    inventory[bombType] += amount;
}

void BombContainerComponent::removeFromInventory(const EntityType &bombType, int amount) {
    if (inventory[bombType] - amount < 0) {
        inventory.erase(bombType);
        stopwatches.erase(bombType);
    } else {
        inventory[bombType] -= amount;
    }
}

int BombContainerComponent::getAmount(const EntityType &bombType) const {
    if (inventory.count(bombType) > 0) {
        return inventory.at(bombType);
    } else {
        return 0;
    }
};

Stopwatch &BombContainerComponent::getStopwatch(const EntityType &bombType) {
    return stopwatches[bombType];
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