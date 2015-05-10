#pragma once

#include "MessageHandler.h"

class KaboomV2MessageHandler : public MessageHandler {
public:
    virtual bool handle(const Message &message) const;
};
