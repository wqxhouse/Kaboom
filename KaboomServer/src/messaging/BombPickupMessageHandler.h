#pragma once

#include "MessageHandler.h"

class PickupMessage;
class TickMessage;

class BombPickupMessageHandler : public MessageHandler {
public:
    virtual bool handle(const Message &message) const;

private:
    bool handle(const PickupMessage &message) const;
    bool handle(const TickMessage &message) const;
};
