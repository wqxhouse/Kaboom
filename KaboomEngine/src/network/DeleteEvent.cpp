#include "DeleteEvent.h"

#include <string>
DeleteEvent::DeleteEvent() :Event(EventOpcode::DELETE_ENTITY, sizeof(DeleteEvent)), entityId(0) {

}


DeleteEvent::DeleteEvent(unsigned int id):Event(EventOpcode::DELETE_ENTITY,sizeof(DeleteEvent)),entityId(id) {

}

DeleteEvent::~DeleteEvent() {

}
void DeleteEvent::serialize(char *buf) const {
	memcpy(buf, this, sizeof(DeleteEvent));
}

void DeleteEvent::deserialize(char *buf) {
	memcpy(this, buf, sizeof(DeleteEvent));
}