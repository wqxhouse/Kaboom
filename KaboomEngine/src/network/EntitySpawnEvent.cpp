#include "EntitySpawnEvent.h"

#include <string>

EntitySpawnEvent::EntitySpawnEvent()
: EntitySpawnEvent(0, 0.0f, 0.0f, 0.0f,PLAYER,0) {
}

EntitySpawnEvent::EntitySpawnEvent(unsigned int playerId, float x, float y, float z,EntityType type,int feature)
: Event(EventOpcode::ENTITY_SPAWN, sizeof(EntitySpawnEvent)),
    playerId(playerId),
    x(x),
    y(y),
    z(z),
	type(type),
	feature(feature){
}

EntitySpawnEvent::~EntitySpawnEvent() {
}

const unsigned int &EntitySpawnEvent::getPlayerId() const {
    return playerId;
}

const float &EntitySpawnEvent::getX() const {
    return x;
}

const float &EntitySpawnEvent::getY() const {
    return y;
}

const float &EntitySpawnEvent::getZ() const {
    return z;
}


const EntityType &EntitySpawnEvent::getType() const {
	return type;
}

const int &EntitySpawnEvent::getFeature() const {
	return feature;
}
void EntitySpawnEvent::serialize(char * buf) const {
	memcpy(buf, this, sizeof(EntitySpawnEvent));
}

void EntitySpawnEvent::deserialize(char * buf) {
	memcpy(this, buf, sizeof(EntitySpawnEvent));
}
