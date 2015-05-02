#pragma once

#include <ostream>
#include <unordered_map>
#include <vector>

#include "Component.h"
#include "Entity.h"
#include "../util/Stopwatch.h"

class BombContainerComponent : public Component {
public:
    typedef std::unordered_map<EntityType, unsigned int> InventoryType;

    BombContainerComponent(const InventoryType &inventory = InventoryType());

    void addToInventory(const EntityType &bombType, unsigned int amount = 1);
    void removeFromInventory(const EntityType &bombType, unsigned int amount = 1);

    unsigned int getSize(const EntityType &bombType) const;
    Stopwatch &getStopwatch(const EntityType &bombType);

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

    InventoryType inventory;
	std::vector<Entity *> activeBombs;
    std::unordered_map<EntityType, Stopwatch> stopwatches;
};
