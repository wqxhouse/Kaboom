#pragma once

#include <ostream>

#include "Component.h"
#include "CharacteristicComponent.h"

class BombContainerComponent : public Component {
public:
	BombContainerComponent();
	~BombContainerComponent();
	void addBombToInv(Entity *,int owner);
	Entity * fireBomb();
	void selectRightBomb();
	void selectLeftBomb();
	std::vector<Entity *> inventory();
	std::vector<Entity *> active();
	

	friend std::ostream& operator<<(std::ostream &os, const BombContainerComponent &o) {
        os << "RotationComponent: {" << std::endl;
        os << "    yaw: " << o.selectedBomb << std::endl;
        os << "}";

        return os;
    }

private:
	std::vector<Entity *> bombInventory;
	int selectedBomb;
	int maxSize;
	std::vector<Entity *> activeBombs;
};
