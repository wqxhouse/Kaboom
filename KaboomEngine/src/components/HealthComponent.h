#pragma once

#include "Component.h"

class HealthComponent : public Component {
public:
    HealthComponent(int amount, int capacity)
            : amount(amount),
              capacity(capacity) {
    }

    inline int getAmount() const {
        return amount;
    }

    inline void setHealthAmount(int amount) {
        this->amount = amount;
    }

    inline void addToHealthAmount(int amount) {
        if (this->amount + amount > capacity) {
            this->amount = capacity;
        } else {
            this->amount += amount;
        }
    }

    inline void subtractFromHealthAmount(int amount) {
        if (this->amount - amount < 0) {
            this->amount = 0;
        } else {
            this->amount -= amount;
        }
    }

private:
    int amount;
    int capacity;
};
