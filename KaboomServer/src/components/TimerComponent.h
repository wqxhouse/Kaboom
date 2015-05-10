#pragma once

#include <components/Component.h>
#include <util/Timer.h>

class TimerComponent : public Component {
public:
    TimerComponent(Timer *timer);

    Timer *getTimer();

private:
    Timer *timer;
};
