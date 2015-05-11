#include "HealthEvent.h"

#include <string>

HealthEvent::HealthEvent(unsigned int healthAmount)
: Event(EVENT_HEALTH, sizeof(HealthEvent)),
healthAmount(healthAmount) {
}

HealthEvent::HealthEvent()
: Event(EVENT_HEALTH, sizeof(HealthEvent)),
healthAmount(100) {
}
void HealthEvent::serialize(char *buf) const {
	memcpy(buf, this, sizeof(HealthEvent));
}

void HealthEvent::deserialize(char *buf) {
	memcpy(this, buf, sizeof(HealthEvent));
}

unsigned int HealthEvent::getHealthAmount() const {
	return healthAmount;
}
