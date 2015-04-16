#pragma once

#include "Event.h"

class PlayerSpawnEvent : public Event {
public:
	PlayerSpawnEvent();
    PlayerSpawnEvent(unsigned int playerId, float x, float y, float z);
    ~PlayerSpawnEvent();


    const unsigned int &getPlayerId() const;

    const float &getX() const;
    const float &getY() const;
    const float &getZ() const;

	void serialize(char * buf) {
		memcpy(buf, this, sizeof(PlayerSpawnEvent));
	}

	void deserialize(char * buf) {
		memcpy(this, buf, sizeof(PlayerSpawnEvent));
	}


private:
    unsigned int playerId;

    float x;
    float y;
    float z;
};
