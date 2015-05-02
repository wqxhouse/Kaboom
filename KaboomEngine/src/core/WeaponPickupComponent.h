#pragma once

#include <ostream>

#include "Component.h"
#include "EntityType.h"

class WeaponPickupComponent : public Component {
public:
	WeaponPickupComponent();
    WeaponPickupComponent(EntityType, unsigned int);
	~WeaponPickupComponent();

    EntityType getBombType() const;
	unsigned int getAmount() const;

    void setBombType(EntityType);
	void setAmount(unsigned int);


	friend std::ostream& operator<<(std::ostream &os, const WeaponPickupComponent &o) {
        os << "WeaponPickupComponent: {" << std::endl;
        os << "    BombType: " << o.bombType << std::endl;
		os << "    amount:   " << o.amount << std::endl;
        os << "}";

        return os;
    }

private:
    EntityType bombType;
	unsigned int amount;
};
