#pragma once

#include <components/Component.h>

class Player;

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
