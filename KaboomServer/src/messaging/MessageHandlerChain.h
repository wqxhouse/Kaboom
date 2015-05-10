#pragma once

#include <vector>

#include "MessageHandler.h"

class MessageHandlerChain : public MessageHandler {
public:
    void addHandler(MessageHandler *handler);

    virtual bool handle(const Message &message) const;

private:
    std::vector<MessageHandler *> handlers;
};
