#pragma once

#include <ostream>

#include "Event.h"
#include "..\core\CharacteristicComponent.h"

class EntitySpawnEvent : public Event {
public:
	EntitySpawnEvent();
	EntitySpawnEvent(unsigned int playerId, float x, float y, float z,EntityType type,int feature);
	~EntitySpawnEvent();

    const unsigned int &getPlayerId() const;

    const float &getX() const;
    const float &getY() const;
    const float &getZ() const;
	const EntityType &getType() const;
	const int &getFeature() const;

    virtual void serialize(char *buf) const;
    virtual void deserialize(char *buf);

	friend std::ostream& operator<<(std::ostream &os, const EntitySpawnEvent &o) {
        os << "EntitySpawnEvent: {" << std::endl;
        os << "    playerId: " << o.playerId << std::endl;
        os << "    x: " << o.x << std::endl;
        os << "    y: " << o.y << std::endl;
        os << "    z: " << o.z << std::endl;
		os << "    type: " << o.type << std::endl;
		os << "    feature: " << o.feature << std::endl;
        os << "}";

        return os;
    }

private:
    unsigned int playerId;

    float x;
    float y;
    float z;
	EntityType type;
	int feature;
};
