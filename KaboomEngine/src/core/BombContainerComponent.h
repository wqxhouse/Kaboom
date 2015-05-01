#pragma once

#include <ostream>
#include <vector>
#include <unordered_map>

#include "CoolDown.h"
#include "BombType.h"

#include "Component.h"

class BombContainerComponent : public Component {
public:
	BombContainerComponent(unsigned int entityId);
	~BombContainerComponent();

	void addBombToInv(BombType bombType);
	void addBombToInv(BombType bombType, unsigned int howMany);
	unsigned getNumOfBomb(BombType bombType);
	void deleteBombInInv(BombType bombType);
	void deleteBombInInv(BombType bombType, unsigned int);

	void addBombInActiveBomb(Entity *);
	void deleteBombInActiveBomb(Entity *);

	CoolDown* getBombCoolDown(BombType bombType);

	friend std::ostream& operator<<(std::ostream &os, const BombContainerComponent &o) {//TODO update this
        os << "BombContainerComponent: {" << std::endl;
        os << "    entityId: " << o.entityId << std::endl;
        os << "    maxSize: " << o.maxSize << std::endl;
		os << "    bombInInv { " << std::endl;
		for (BombType bombType : BombTypeArr){
			os << "        " << bombType << " : " << o.bombInventoryMap.at(bombType) << std::endl;
		}
		os << "    }" << std::endl;
        os << "}";

        return os;
    }

private:
    int maxSize;
	unsigned int entityId; 

	std::unordered_map<BombType, unsigned int> bombInventoryMap;
	std::unordered_map<BombType, CoolDown *> bombCoolDownMap;
	//std::unordered_map<BombType, unsigned int, std::hash<unsigned int>> cooldown
	std::vector<Entity *> activeBombs;
};
