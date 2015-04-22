#pragma once

enum EventOpcode {
	EMPTY = 0,
    POSITION = 1,
    ROTATION = 2,
    PLAYER_INPUT = 3,
    ENTITY_SPAWN = 4,
	DELETE_ENTITY=5
};

class Event {
public:
    ~Event();

    EventOpcode getOpcode() const;

    const unsigned int getByteSize() const;

    virtual void serialize(char *buf) const = 0;
    virtual void deserialize(char *buf) = 0;

protected:
    Event(EventOpcode opcode, unsigned int byteSize);

    EventOpcode eventOpcode;
    unsigned int byteSize;
};
