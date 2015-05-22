#pragma once

#include "MessageHandler.h"

class JumpPadMessage;

class JumpPadMessageHandler : public MessageHandler {
public:
    virtual bool handle(const Message &message) const;

private:
	bool handle(const JumpPadMessage &message) const;
};
