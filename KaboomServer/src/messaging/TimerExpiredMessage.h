#pragma once

#include "Message.h"
#include "MessageType.h"

class Timer;

class TimerExpiredMessage : public Message {
public:
    TimerExpiredMessage(Game *game, Entity *entity, Timer *timer)
        : Message(MessageType::TIMER_EXPIRED, game, entity),
          timer(timer) {
    }

    inline Timer *getTimer() const {
        return timer;
    }

private:
    Timer *timer;
};