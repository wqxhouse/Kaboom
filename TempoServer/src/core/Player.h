#pragma once

#include "PlayerData.h"

class Player {
public:
    Player();
    ~Player();

private:
    PlayerData *data;
};
