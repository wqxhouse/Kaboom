#pragma once

#include <components/Component.h>

#include "../core/Player.h"

class DeathComponent : public Component {
public:
    DeathComponent(Player *killer)
            : killer(killer) {
    }

    inline Player *getKiller() const {
        return killer;
    }

private:
    Player *killer;
};
