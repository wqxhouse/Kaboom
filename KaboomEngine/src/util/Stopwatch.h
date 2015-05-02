#pragma once

#include <time.h> 

class Stopwatch {
public:
    Stopwatch();

    void start();
    void stop();

    clock_t getTimeElapsed() const;

    const clock_t &getStartTime() const;
    const clock_t &getStopTime() const;
    bool isRunning() const;

private:
    clock_t startTime;
    clock_t stopTime;
    bool running;
};
