#include "AssignEvent.h"

#include <string>

AssignEvent::AssignEvent(unsigned int entityId)
	    : Event(EventOpcode::ASSIGN_ENTITY, sizeof(AssignEvent)),
          entityId(entityId) {
}

void AssignEvent::serialize(char *buf) const {
	memcpy(buf, this, sizeof(AssignEvent));
}

void AssignEvent::deserialize(char *buf) {
	memcpy(this, buf, sizeof(AssignEvent));
}

unsigned int AssignEvent::getEntityId() const {
	return entityId;
}
