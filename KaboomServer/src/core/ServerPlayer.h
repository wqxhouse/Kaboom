#pragma once

#include <core/Player.h>

class ServerPlayer : public Player {
public:
    ServerPlayer(unsigned int id);
    ~ServerPlayer();
};
