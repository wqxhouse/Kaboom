#pragma once

#include <ostream>

#include "Event.h"
#include "../core/EntityType.h"

class SpawnEvent : public Event {
public:
    SpawnEvent();
    SpawnEvent(unsigned int entityId, float x, float y, float z, EntityType type, int feature);
    ~SpawnEvent();

    const unsigned int &getEntityId() const;

    const float &getX() const;
    const float &getY() const;
    const float &getZ() const;

	const EntityType &getType() const;
	const int &getFeature() const;

    virtual void serialize(char *buf) const;
    virtual void deserialize(char *buf);

	friend std::ostream& operator<<(std::ostream &os, const SpawnEvent &o) {
        os << "EntitySpawnEvent: {" << std::endl;
        os << "    entityId: " << o.entityId << std::endl;
        os << "    x: " << o.x << std::endl;
        os << "    y: " << o.y << std::endl;
        os << "    z: " << o.z << std::endl;
		os << "    type: " << o.type << std::endl;
		os << "    feature: " << o.feature << std::endl;
        os << "}";

        return os;
    }

private:
    unsigned int entityId;

    float x;
    float y;
    float z;

	EntityType type;
	int feature;
};
