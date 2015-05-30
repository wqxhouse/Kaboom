#pragma once

#include "MessageHandler.h"

class CollisionMessage;

class FakeBombMessageHandler : public MessageHandler {
public:
    virtual bool handle(const Message &message) const;

private:
    bool handle(const CollisionMessage &message) const;
};
