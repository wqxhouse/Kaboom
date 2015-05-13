#pragma once

#include "MessageHandler.h"

class ExplosionMessage;

class DefaultExplosionMessageHandler : public MessageHandler {
public:
    virtual bool handle(const Message &message) const;

private:
    bool handle(const ExplosionMessage &message) const;
};
