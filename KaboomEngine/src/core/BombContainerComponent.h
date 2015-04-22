#pragma once

#include <ostream>
#include <vector>

#include "Component.h"

class BombContainerComponent : public Component {
public:
	BombContainerComponent();
	~BombContainerComponent();

	void addBombToInv(Entity *bomb, int owner);
	Entity *fireBomb();
	void selectRightBomb();
	void selectLeftBomb();

	std::vector<Entity *> inventory();
	std::vector<Entity *> active();

	friend std::ostream& operator<<(std::ostream &os, const BombContainerComponent &o) {
        os << "BombContainerComponent: {" << std::endl;
        os << "    selectedBomb: " << o.selectedBomb << std::endl;
        os << "    maxSize: " << o.maxSize << std::endl;
        os << "}";

        return os;
    }

private:
    int selectedBomb;
    int maxSize;

	std::vector<Entity *> bombInventory;
	std::vector<Entity *> activeBombs;
};
