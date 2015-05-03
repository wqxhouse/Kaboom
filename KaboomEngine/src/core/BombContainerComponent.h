#pragma once

#include <ostream>
#include <unordered_map>
#include <vector>

#include "Component.h"
#include "Entity.h"
#include "../util/Timer.h"

class BombContainerComponent : public Component {
public:
    typedef std::unordered_map<EntityType, std::pair<unsigned int, Timer>> InventoryType;

    BombContainerComponent(const InventoryType &inventory = InventoryType());

    void addToInventory(const EntityType &bombType, int amount = 1);
    void removeFromInventory(const EntityType &bombType, int amount = 1);

    bool hasBomb(const EntityType &bombType) const;
    int getAmount(const EntityType &bombType) const;
    Timer &getTimer(const EntityType &bombType);

	void addToActiveBomb(Entity *bomb);
	void removeFromActiveBomb(Entity *bomb);

	friend std::ostream &operator<<(std::ostream &os, const BombContainerComponent &o) {//TODO update this
        os << "BombContainerComponent: {" << std::endl;
        os << "    capacity: " << o.capacity << std::endl;
		os << "    inventory: { " << std::endl;
        for (auto kv : o.inventory) {
			os << "        " << kv.first << ": {" << std::endl;
            os << "            amount: " << kv.second.first << std::endl;
            os << "            expired: " << kv.second.second.isExpired() << std::endl;
            os << "        }" << std::endl;
		}
		os << "    }" << std::endl;
        os << "}";

        return os;
    }

private:
    int capacity; // TODO: capacity is not set

    InventoryType inventory;
	std::vector<Entity *> activeBombs;
};
