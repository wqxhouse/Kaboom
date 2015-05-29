#pragma once

#include "MessageHandler.h"

class BombDropDespawnMessage;

class BombDropDespawnMessageHandler : public MessageHandler {
public:
	virtual bool handle(const Message &message) const;

private:
	bool handle(const BombDropDespawnMessage &message) const;
};
