#pragma once

#include <chrono>

class Timer {
public:
    Timer::Timer(
            long long duration = 0,
            std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now())
            : duration(std::chrono::milliseconds(duration)),
              startTime(startTime) {
    }

    inline void start() {
        startTime = std::chrono::high_resolution_clock::now();
    }

    inline bool isExpired() const {
        return std::chrono::high_resolution_clock::now() > startTime + duration;
    }

    inline std::chrono::milliseconds getDuration() const {
        return duration;
    }

    inline void setDuration(long long duration) {
        this->duration = std::chrono::milliseconds(duration);
    }

    inline std::chrono::high_resolution_clock::time_point getStartTime() const {
        return startTime;
    }

private:
    std::chrono::milliseconds duration;
    std::chrono::high_resolution_clock::time_point startTime;
};
