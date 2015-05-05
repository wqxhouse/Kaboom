#pragma once

#include <core/Component.h>
#include <util/Timer.h>

#include "../core/TimerHandler.h"

class TimerHandlerComponent : public Component {
public:
    TimerHandlerComponent(Timer *timer, TimerHandler *handler);

    Timer *getTimer();
    TimerHandler *getHandler() const;

private:
    Timer *timer;
    TimerHandler *handler;
};
