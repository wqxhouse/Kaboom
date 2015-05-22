#pragma once

#include <time.h> 

class Timer {
public:
    Timer::Timer(clock_t duration = 0)
            : duration(duration),
              startTime(clock()) {
    }

    inline void start() {
        startTime = clock();
    }

    inline bool isExpired() const {
        return clock() > startTime + duration;
    }

    inline clock_t getDuration() const {
        return duration;
    }

    inline void setDuration(clock_t duration) {
        this->duration = duration;
    }

private:
    clock_t duration;
    clock_t startTime;
};
