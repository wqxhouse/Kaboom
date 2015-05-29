#pragma once

#include <ostream>

#include "Event.h"

class PlayerStatusEvent : public Event {
public:
    PlayerStatusEvent() : PlayerStatusEvent(0, false, false, false, false, false) {}

    PlayerStatusEvent(
            unsigned int entityId,
            bool alive,
            bool running,
            bool jumping,
            bool attacking,
            bool damaged)
            : Event(EVENT_PLAYER_STATUS, sizeof(PlayerStatusEvent)),
              entityId(entityId),
              alive(alive),
              running(running),
              jumping(jumping),
              attacking(attacking),
              damaged(damaged) {
    }

    inline virtual void serialize(char *buf) const {
        memcpy(buf, this, sizeof(PlayerStatusEvent));
    }

    inline virtual void deserialize(char *buf) {
        memcpy(this, buf, sizeof(PlayerStatusEvent));
    }

    inline unsigned int getEntityId() const {
        return entityId;
    }

    inline void setEntityId(unsigned int entityId) {
        this->entityId = entityId;
    }

    inline bool isAlive() const {
        return alive;
    }

    inline void setAlive(bool alive) {
        this->alive = alive;
    }

    inline bool isRunning() const {
        return running;
    }

    inline void setRunning(bool running) {
        this->running = running;
    }

    inline bool isJumping() const {
        return jumping;
    }

    inline void setJumping(bool jumping) {
        this->jumping = jumping;
    }

    inline bool isAttacking() const {
        return attacking;
    }

    inline void setAttacking(bool attacking) {
        this->attacking = attacking;
    }

    inline bool isDamaged() const {
        return damaged;
    }

    inline void setDamaged(bool damaged) {
        this->damaged = damaged;
    }

    friend std::ostream &operator<<(std::ostream &os, const PlayerStatusEvent &o) {
        os << "PlayerStatusEvent: {" << std::endl;
        os << "    entityId: " << o.entityId << std::endl;
        os << "    alive: " << o.alive << std::endl;
        os << "    running: " << o.running << std::endl;
        os << "    jumping: " << o.jumping << std::endl;
        os << "    attacking: " << o.attacking << std::endl;
        os << "    damaged: " << o.damaged << std::endl;
        os << "}";

        return os;
    }

private:
    unsigned int entityId;
    bool alive;
    bool running;
    bool jumping;
    bool attacking;
    bool damaged;
};
