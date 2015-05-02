#pragma once

#include <ostream>
#include <unordered_map>
#include <vector>

#include "Component.h"
#include "Entity.h"

class BombContainerComponent : public Component {
public:
    void addToInventory(const EntityType &bombType, unsigned int amount = 1);
    void removeFromInventory(const EntityType &bombType, unsigned int amount = 1);

    unsigned int getSize(const EntityType &bombType) const;

	void addToActiveBomb(Entity *bomb);
	void removeFromActiveBomb(Entity *bomb);

	friend std::ostream &operator<<(std::ostream &os, const BombContainerComponent &o) {//TODO update this
        os << "BombContainerComponent: {" << std::endl;
        os << "    capacity: " << o.capacity << std::endl;
		os << "    inventory: { " << std::endl;
        for (auto kv : o.inventory) {
			os << "        " << kv.first << ": " << kv.second << std::endl;
		}
		os << "    }" << std::endl;
        os << "}";

        return os;
    }

private:
    int capacity; // TODO: capacity is not set

    std::unordered_map<EntityType, unsigned int> inventory;
	std::vector<Entity *> activeBombs;
};
