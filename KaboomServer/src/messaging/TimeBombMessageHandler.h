#pragma once

#include "MessageHandler.h"

class TimeBombMessageHandler : public MessageHandler {
public:
    virtual bool handle(const Message &message) const;
};
