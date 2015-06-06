#pragma once

#include <components/Component.h>

class Player;

class KilledByComponent : public Component {
public:
    KilledByComponent(Player *killer)
            : killer(killer) {
    }

    inline Player *getKiller() const {
        return killer;
    }

private:
    Player *killer;
};
