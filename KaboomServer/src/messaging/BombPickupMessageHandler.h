#pragma once

#include "MessageHandler.h"

class PickupMessage;

class BombPickupMessageHandler : public MessageHandler {
public:
    virtual bool handle(const Message &message) const;

private:
    bool handle(const PickupMessage &message) const;
};
