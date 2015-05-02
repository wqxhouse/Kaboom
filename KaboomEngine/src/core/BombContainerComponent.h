#pragma once

#include <ostream>
#include <vector>
#include <unordered_map>

#include "CoolDown.h"
#include "EntityType.h"

#include "Component.h"

class BombContainerComponent : public Component {
public:
	BombContainerComponent(unsigned int entityId);
	~BombContainerComponent();

    void addBombToInv(EntityType bombType);
    void addBombToInv(EntityType bombType, unsigned int howMany);
    unsigned getNumOfBomb(EntityType bombType);
    void deleteBombInInv(EntityType bombType);
    void deleteBombInInv(EntityType bombType, unsigned int);

	void addBombInActiveBomb(Entity *);
	void deleteBombInActiveBomb(Entity *);

    CoolDown* getBombCoolDown(EntityType bombType);

	friend std::ostream& operator<<(std::ostream &os, const BombContainerComponent &o) {//TODO update this
        os << "BombContainerComponent: {" << std::endl;
        os << "    entityId: " << o.entityId << std::endl;
        os << "    maxSize: " << o.maxSize << std::endl;
		os << "    bombInInv { " << std::endl;
// TODO: Fix this
//        for (EntityType bombType : BombTypeArr) {
//			os << "        " << bombType << " : " << o.bombInventoryMap.at(bombType) << std::endl;
//		}
		os << "    }" << std::endl;
        os << "}";

        return os;
    }

private:
    int maxSize;
	unsigned int entityId; 

    std::unordered_map<EntityType, unsigned int> bombInventoryMap;
    std::unordered_map<EntityType, CoolDown *> bombCoolDownMap;
	//std::unordered_map<BombType, unsigned int, std::hash<unsigned int>> cooldown
	std::vector<Entity *> activeBombs;
};
