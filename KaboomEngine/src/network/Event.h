#pragma once

enum EventOpcode {
	EMPTY = 0,
    POSITION = 1,
    ROTATION = 2,
    PLAYER_INPUT = 3,
    PLAYER_SPAWN = 4
};

class Event {
public:
    Event();
    ~Event();

    EventOpcode getOpcode() const;

    const unsigned int getByteSize() const;

    virtual void serialize(char *buf);
    virtual void deserialize(char *buf);

protected:
    EventOpcode eventOpcode;
    unsigned int byteSize;
};
