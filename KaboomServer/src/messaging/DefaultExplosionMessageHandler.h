#pragma once

#include "MessageHandler.h"

class DefaultExplosionMessageHandler : public MessageHandler {
public:
    virtual bool handle(const Message &message) const;
};
