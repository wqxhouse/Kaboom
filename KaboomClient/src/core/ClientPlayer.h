#pragma once

#include <core/Player.h>

class ClientPlayer : public Player {
public:
    ClientPlayer(unsigned int id);
    ~ClientPlayer();

	void setPosition(float x, float y, float z);
};
