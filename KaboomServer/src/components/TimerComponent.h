#pragma once

#include <components/Component.h>

class Timer;

class TimerComponent : public Component {
public:
    TimerComponent(Timer *timer)
            : timer(timer) {
    }

    inline Timer *getTimer() {
        return timer;
    }

private:
    Timer *timer;
};
