#pragma once

#include <ostream>

#include "Component.h"
#include "../core/EntityType.h"

class WeaponPickupComponent : public Component {
public:
    WeaponPickupComponent(
            EntityType type = KABOOM_V2,
            int amount = 1)
            : type(type),
              amount(amount) {
    }

    inline EntityType getType() const {
        return type;
    }

    inline int getAmount() const {
        return amount;
    }

    inline void setBombType(EntityType type) {
        this->type = type;
    }

    inline void setAmount(int amount) {
        this->amount = amount;
    }

    friend std::ostream &operator<<(std::ostream &os, const WeaponPickupComponent &o) {
        os << "WeaponPickupComponent: {" << std::endl;
        os << "    type: " << o.type << std::endl;
        os << "    amount:   " << o.amount << std::endl;
        os << "}";

        return os;
    }

private:
    EntityType type;
    int amount;
};
