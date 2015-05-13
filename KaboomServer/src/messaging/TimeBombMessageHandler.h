#pragma once

#include "MessageHandler.h"

class TimerExpiredMessage;

class TimeBombMessageHandler : public MessageHandler {
public:
    virtual bool handle(const Message &message) const;

private:
    bool handle(const TimerExpiredMessage &message) const;
};
