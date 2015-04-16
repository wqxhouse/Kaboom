#pragma once

#include <string.h>

enum EventOpcode {
	EMPTY = 0,
    POSITION = 1,
    ROTATION = 2,
    PLAYER_INPUT = 3,
    PLAYER_SPAWN = 4
};

class Event {
public:
	Event() {
		eventOpcode = EventOpcode::EMPTY;
		byteSize = sizeof(Event);
	}
    ~Event() {}

	EventOpcode eventOpcode;
	unsigned int byteSize;

	EventOpcode getOpcode() const {
		return eventOpcode;
	};

	const unsigned int getByteSize() const {
		return byteSize;
	};

	void serialize(char* buf) {
		memcpy(buf, this, sizeof(Event));
	};

	void deserialize(char * buf) {
		memcpy(this, buf, sizeof(Event));
	}
};
