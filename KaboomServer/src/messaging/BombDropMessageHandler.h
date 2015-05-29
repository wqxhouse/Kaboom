#pragma once

#include "MessageHandler.h"

class BombDropMessage;

class BombDropMessageHandler : public MessageHandler {
public:
    virtual bool handle(const Message &message) const;

private:
	bool handle(const BombDropMessage &message) const;
};
