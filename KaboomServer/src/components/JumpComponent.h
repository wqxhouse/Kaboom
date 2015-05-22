#pragma once

#include <components/Component.h>
#include <util/Timer.h>

class JumpComponent : public Component {
public:
	JumpComponent(bool jumping = false, bool isLaunchByJumpPad = false)
		: jumping(jumping),
		isLaunchByJumpPad(isLaunchByJumpPad),
		isLaunchByJumpPadTimer(0) {
    }

    inline bool isJumping() const {
        return jumping;
    }

    inline void setJumping(bool jumping) {
        this->jumping = jumping;
    }
	
	inline bool checkIsLaunchByJumpPad() {
		if (isLaunchByJumpPad){
			if (isLaunchByJumpPadTimer.isExpired()){
				isLaunchByJumpPad = false;
			}
		}
		return isLaunchByJumpPad;
	}

	inline bool getIsLaunchByJumpPad() const {
		return isLaunchByJumpPad;
	}

	inline void setIsLaunchByJumpPad(bool isLaunchByJumpPad){
		this->isLaunchByJumpPad = isLaunchByJumpPad;
	}

	inline Timer getIsLaunchByJumpPadTimer(){
		return isLaunchByJumpPadTimer;
	}

private:
    bool jumping;
	bool isLaunchByJumpPad;
	Timer isLaunchByJumpPadTimer;
};
