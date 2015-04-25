#include "AssignEvent.h"

#include <string>

AssignEvent::AssignEvent()
	: AssignEvent(0) {
}

AssignEvent::AssignEvent(unsigned int id)
	: Event(EventOpcode::ASSIGN_ENTITY, sizeof(AssignEvent)),
    entityId(id) {
}

AssignEvent::~AssignEvent() {
}

void AssignEvent::serialize(char *buf) const {
	memcpy(buf, this, sizeof(AssignEvent));
}

void AssignEvent::deserialize(char *buf) {
	memcpy(this, buf, sizeof(AssignEvent));
}

const unsigned int &AssignEvent::getEntityId() const {
	return entityId;
}
