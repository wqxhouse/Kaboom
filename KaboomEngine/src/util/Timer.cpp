#include "Timer.h"

Timer::Timer(clock_t duration)
        : duration(duration),
          startTime(clock()) {
}

void Timer::start() {
    startTime = clock();
}

bool Timer::isExpired() const {
    return clock() > startTime + duration;
}

clock_t Timer::getDuration() const {
    return duration;
}

void Timer::setDuration(clock_t duration) {
    this->duration = duration;
}