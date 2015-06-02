#pragma once

#include "MessageHandler.h"

#include <math/Vec3.h>

class Attack1Message;
class Attack2Message;
class Entity;
class NoAttackMessage;

class DefaultCharacterMessageHandler : public MessageHandler {
public:
    virtual bool handle(const Message &message) const;

private:
    bool handle(const Attack1Message &message) const;
    bool handle(const Attack2Message &message) const;
    bool handle(const NoAttackMessage &message) const;

    Vec3 calculateBombSpawnLocation(const Vec3 &pos, const Vec3 &dir) const;
};
