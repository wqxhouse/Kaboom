#include "TimerComponent.h"

TimerComponent::TimerComponent(Timer *timer, TimerHandler *handler)
        : timer(timer),
          handler(handler) {
}

Timer *TimerComponent::getTimer() {
    return timer;
}

TimerHandler *TimerComponent::getHandler() const {
    return handler;
}
