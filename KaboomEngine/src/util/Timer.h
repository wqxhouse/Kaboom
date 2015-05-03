#pragma once

#include <time.h> 

class Timer {
public:
    Timer(clock_t duration = 0);

    void start();

    bool isExpired() const;

    clock_t getDuration() const;
    void setDuration(clock_t duration);

private:
    long duration;

    clock_t startTime;
};
