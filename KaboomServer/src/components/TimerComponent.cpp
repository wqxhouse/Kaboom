#include "TimerComponent.h"

TimerComponent::TimerComponent(Timer *timer)
        : timer(timer) {
}

Timer *TimerComponent::getTimer() {
    return timer;
}
