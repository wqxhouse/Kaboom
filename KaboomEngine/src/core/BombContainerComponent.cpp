#include "BombContainerComponent.h"

#include "CharacteristicComponent.h"
#include "Entity.h"

BombContainerComponent::BombContainerComponent() {
    selectedBomb = 0;
}

BombContainerComponent::~BombContainerComponent() {
}

void BombContainerComponent::addBombToInv(Entity *bomb, int owner) {
    if (bomb->getComponent<CharacteristicComponent>() == nullptr) {
        return;
    }

    if (bomb->getComponent<CharacteristicComponent>()->getType() != BOMB) {
        return;
    }

    bombInventory.push_back(bomb);
}

Entity *BombContainerComponent::fireBomb() {
    Entity *bomb = nullptr;

    if (bombInventory.size()>0) {
        activeBombs.push_back(bombInventory[selectedBomb]);
        bomb = bombInventory[selectedBomb];
        bombInventory.erase(bombInventory.begin() + selectedBomb);
    }

    return bomb;
}

void BombContainerComponent::selectLeftBomb() {
    if (selectedBomb == 0) {
        selectedBomb = (int)bombInventory.size() - 1;
    } else {
        selectedBomb--;
    }
}

void  BombContainerComponent::selectRightBomb() {
    if (selectedBomb == bombInventory.size() - 1) {
        selectedBomb = 0;
    } else {
        selectedBomb++;
    }
}