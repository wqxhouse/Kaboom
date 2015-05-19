#pragma once

#include <ostream>
#include <unordered_map>
#include <vector>

#include "Component.h"
#include "../core/Entity.h"
#include "../util/Timer.h"

class BombContainerComponent : public Component {
public:
    typedef std::unordered_map<EntityType, std::pair<unsigned int, Timer>> InventoryType;

    BombContainerComponent(const InventoryType &inventory = InventoryType())
        : capacity(0),
          inventory(inventory) {
    }

    inline void addToInventory(EntityType bombType, int amount = 1) {
        inventory[bombType].first += amount;
    }

    inline void removeFromInventory(const EntityType &bombType, int amount = 1) {
        if (inventory[bombType].first - amount < 0) {
            inventory.erase(bombType);
        } else {
            inventory[bombType].first -= amount;
        }
    }

    inline bool hasBomb(EntityType bombType) const {
        return inventory.count(bombType) > 0;
    }

    inline int getAmount(EntityType bombType) const {
        return inventory.at(bombType).first;
    }

    inline Timer &getTimer(EntityType bombType) {
        return inventory.at(bombType).second;
    }

    inline const std::vector<Entity *> getActiveBombs() const {
        return activeBombs;
    }

    inline void addToActiveBomb(Entity *bomb) {
        activeBombs.push_back(bomb);
    }

    inline void removeFromActiveBomb(Entity *bomb) {
        activeBombs.erase(std::find(activeBombs.begin(), activeBombs.end(), bomb));
    }

	friend std::ostream &operator<<(std::ostream &os, const BombContainerComponent &o) {
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
