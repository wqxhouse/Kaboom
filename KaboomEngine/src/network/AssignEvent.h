#pragma once

#include <ostream>
#include <string>

#include "Event.h"

class AssignEvent : public Event {
public:
	AssignEvent(clock_t clock,clock_t duration,unsigned int playerId = 0 )
            : Event(EVENT_ASSIGN, sizeof(AssignEvent)),
              playerId(playerId),time(clock),duration(duration) {
	}

	AssignEvent(unsigned int playerId = 0)
		:Event(EVENT_ASSIGN, sizeof(AssignEvent)),
		playerId(playerId){
	}

    inline virtual void serialize(char *buf) const {
        memcpy(buf, this, sizeof(AssignEvent));
    }

    inline virtual void deserialize(char *buf) {
        memcpy(this, buf, sizeof(AssignEvent));
    }

    inline unsigned int getPlayerId() const {
        return playerId;
    }
	inline clock_t getTime() const{
		return time;
	}
	inline clock_t getDuration() const{
		return duration;
	}

	friend std::ostream &operator<<(std::ostream &os, const AssignEvent &o) {
        os << "AssignEvent: {" << std::endl;
		os << "    playerId: " << o.playerId 
			<< "\ntime: " << o.time
			<< "\nduration: " << o.duration << std::endl;
        os << "}";

        return os;
    }

private:
    unsigned int playerId;
	clock_t time;
	clock_t duration;
};
