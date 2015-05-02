#include "Stopwatch.h"

Stopwatch::Stopwatch()
        : startTime(0),
          stopTime(0),
          running(false) {
}

void Stopwatch::start() {
    startTime = clock();
    running = true;
}

void Stopwatch::stop() {
    stopTime = clock();
    running = false;
}

clock_t Stopwatch::getTimeElapsed() const {
    clock_t timeElapsed;

    if (running) {
        timeElapsed = clock() - startTime;
    } else {
        timeElapsed = stopTime - startTime;
    }

    return timeElapsed * 1000 / CLOCKS_PER_SEC;
}

const clock_t &Stopwatch::getStartTime() const {
    return startTime;
}

const clock_t &Stopwatch::getStopTime() const {
    return stopTime;
}

bool Stopwatch::isRunning() const {
    return running;
}
