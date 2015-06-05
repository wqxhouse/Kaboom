#pragma once

#include "MessageHandler.h"

class CollisionMessage;
class TickMessage;
class TimerExpiredMessage;

class FakeBombMessageHandler : public MessageHandler {
public:
    virtual bool handle(const Message &message) const;

private:
    bool handle(const CollisionMessage &message) const;
    bool handle(const TickMessage &message) const;
    bool handle(const TimerExpiredMessage &message) const;
};
