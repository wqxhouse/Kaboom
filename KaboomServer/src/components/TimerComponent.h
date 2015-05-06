#pragma once

#include <components/Component.h>
#include <util/Timer.h>

#include "../core/TimerHandler.h"

class TimerComponent : public Component {
public:
    TimerComponent(Timer *timer, TimerHandler *handler);

    Timer *getTimer();
    TimerHandler *getHandler() const;

private:
    Timer *timer;
    TimerHandler *handler;
};
