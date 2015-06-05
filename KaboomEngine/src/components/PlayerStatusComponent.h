#pragma once

#include "Component.h"
#include "../util/Timer.h"

class PlayerStatusComponent : public Component {
public:
    PlayerStatusComponent::PlayerStatusComponent()
            : alive(true),
              running(false),
              jumping(false),
              attacking(false),
              damaged(false),
              isKnockBacked(false),
              isStaggered(false),
			  jumped(false),
              knockBackTimer(0),
              staggerTimer(0) {
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

    // Below method is only intend to be use on the Server!
    // TODO: Extract this to KaboomServer project.
    inline bool checkIsKnockBacked() {
        if (isKnockBacked) {
            if (knockBackTimer.isExpired()) {
                isKnockBacked = false;
            }
        }
        return isKnockBacked;
    }

    inline bool checkIsStaggered() {
        if (isStaggered) {
            if (staggerTimer.isExpired()) {
                isStaggered = false;
            }
        }
        return isStaggered;
    }

    // Below method is only intend to be use on the Client!
    // TODO: Extract this to KaboomClient project.
    inline bool getIsKnockBacked() const {
        return isKnockBacked;
    }

    inline bool getIsStaggered() const {
        return isStaggered;
    }

    inline void setIsKnockBacked(bool isKnockBacked) {
        this->isKnockBacked = isKnockBacked;
    }

    inline void setIsStaggered(bool isStaggered) {
        this->isStaggered = isStaggered;
    }

    inline Timer &getKnockBackTimer() {
        return knockBackTimer;
    }

    inline Timer &getStaggerTimer() {
        return staggerTimer;
    }
	inline void setJumped(bool j){
		jumped = j;
	}
	inline bool getJumped(){
		return jumped;
	}

private:
    bool alive;
    bool running;
    bool jumping;
    bool attacking;
    bool damaged;
    bool isKnockBacked;
    bool isStaggered;
	bool jumped;

    Timer knockBackTimer;
    Timer staggerTimer;
};
