#pragma once

#include <components/Component.h>
#include <util/Timer.h>

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
