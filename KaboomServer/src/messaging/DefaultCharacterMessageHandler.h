#pragma once

#include "MessageHandler.h"

class Attack1Message;
class NoAttackMessage;

class DefaultCharacterMessageHandler : public MessageHandler {
public:
    virtual bool handle(const Message &message) const;

private:
    bool handle(const Attack1Message &message) const;
    bool handle(const NoAttackMessage &message) const;
};
