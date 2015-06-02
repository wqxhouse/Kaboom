#pragma once

#include <ostream>

#include "Component.h"
#include "../core/Player.h"

class PlayerComponent : public Component {
public:
    PlayerComponent(Player *player)
            : player(player) {
    }

    inline Player *getPlayer() const {
        return player;
    }

    inline void setPlayer(Player *player) {
        this->player = player;
    }

    friend std::ostream &operator<<(std::ostream &os, const PlayerComponent &o) {
        os << "PlayerComponent: {" << std::endl;
        os << "    player: {" << std::endl;
        os << "        id: " << o.player->getId() << std::endl;
        os << "        kills: " << o.player->getKills() << std::endl;
        os << "        deaths: " << o.player->getDeaths() << std::endl;
        os << "    }" << std::endl;
        os << "}";

        return os;
    }

private:
    Player *player;
};
