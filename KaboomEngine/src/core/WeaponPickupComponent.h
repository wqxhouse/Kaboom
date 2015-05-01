#pragma once

#include <ostream>
#include "BombType.h"
#include "Component.h"

class WeaponPickupComponent : public Component {
public:
	WeaponPickupComponent();
	WeaponPickupComponent(BombType, unsigned int);
	~WeaponPickupComponent();

    BombType getBombType() const;
	unsigned int getAmount() const;

    void setBombType(BombType );
	void setAmount(unsigned int);


	friend std::ostream& operator<<(std::ostream &os, const WeaponPickupComponent &o) {
        os << "WeaponPickupComponent: {" << std::endl;
        os << "    BombType: " << o.bombType << std::endl;
		os << "    amount:   " << o.amount << std::endl;
        os << "}";

        return os;
    }

private:
	BombType bombType;
	unsigned int amount;
};
