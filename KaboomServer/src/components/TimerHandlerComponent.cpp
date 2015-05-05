#include "TimerHandlerComponent.h"

TimerHandlerComponent::TimerHandlerComponent(Timer *timer, TimerHandler *handler)
        : timer(timer),
          handler(handler) {
}

Timer *TimerHandlerComponent::getTimer() {
    return timer;
}

TimerHandler *TimerHandlerComponent::getHandler() const {
    return handler;
}
