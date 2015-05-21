#pragma once

#include <ostream>
#include <unordered_map>

#include "Component.h"
#include "../core/Entity.h"
#include "../util/Timer.h"

class InventoryComponent : public Component {
public:
    typedef std::unordered_map<EntityType, std::pair<unsigned int, Timer>> InventoryType;

    InventoryComponent(const InventoryType &inventory = InventoryType())
            : capacity(0),
              inventory(inventory) {
    }

    inline void add(EntityType bombType, int amount = 1) {
        inventory[bombType].first += amount;
    }

    inline void remove(const EntityType &bombType, int amount = 1) {
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

    friend std::ostream &operator<<(std::ostream &os, const InventoryComponent &o) {
        os << "InventoryComponent: {" << std::endl;
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
};
