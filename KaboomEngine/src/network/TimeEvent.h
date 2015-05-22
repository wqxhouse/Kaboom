#pragma once

#include <ostream>
#include <ctime>

#include "Event.h"

class TimeEvent : public Event {
public:
	TimeEvent() : Event(EVENT_SCORE, sizeof(TimeEvent)),
		_time(0){

	}

	TimeEvent(clock_t time)
		: Event(EVENT_SCORE, sizeof(TimeEvent)),
		 _time(time) {
	}

	inline virtual void serialize(char *buf) const {
		memcpy(buf, this, sizeof(TimeEvent));
	}

	inline virtual void deserialize(char *buf) {
		memcpy(this, buf, sizeof(TimeEvent));
	}

	inline int getTime() const {
		return _time;
	}

	friend std::ostream &operator<<(std::ostream &os, const TimeEvent &o) {
		os << "TimeEvent: {" << std::endl;
		os << "    Clock time: " << o._time << std::endl;
		os << "}";

		return os;
	}

private:
	clock_t _time;
};
