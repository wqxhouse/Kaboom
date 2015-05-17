#pragma once

#include "Component.h"
#include "../util/Timer.h"

class PlayerStatusComponent : public Component {
public:
    PlayerStatusComponent::PlayerStatusComponent()
            : isKnockBacked(false),
              isStaggered(false),
              isDamaged(false),
              isAlive(true),
              knockBackTimer(0),
              staggerTimer(0),
              damageTimer(0) {
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

    inline bool checkIsDamaged() {
        if (isDamaged) {
            if (staggerTimer.isExpired()) {
                isDamaged = false;
            }
        }
        return isDamaged;
    }

	// Below method is only intend to be use on the Client!
    // TODO: Extract this to KaboomClient project.
    inline bool getIsKnockBacked() const {
        return isKnockBacked;
    }

    inline bool getIsStaggered() const {
        return isStaggered;
    }

    inline bool getIsDamaged() const {
        return isDamaged;
    }

    inline bool getIsAlive() const {
        return isAlive;
    }

    inline void setIsKnockBacked(bool isKnockBacked) {
        this->isKnockBacked = isKnockBacked;
    }

    inline void setIsStaggered(bool isStaggered) {
        this->isStaggered = isStaggered;
    }

    inline void setIsDamaged(bool isDamaged) {
        this->isDamaged = isDamaged;
    }

    inline void setIsAlive(bool isAlive){
        this->isAlive = isAlive;
    }

    inline Timer &getKnockBackTimer() {
        return knockBackTimer;
    }

    inline Timer &getStaggerTimer() {
        return staggerTimer;
    }

    inline Timer &getDamageTimer() {
        return damageTimer;
    }

private:
	bool isKnockBacked;
	bool isStaggered;
	bool isDamaged;
	bool isAlive;

	Timer knockBackTimer;
	Timer staggerTimer;
	Timer damageTimer;
};
