#pragma once

#include <components/Component.h>
#include <util/Timer.h>

class JumpComponent : public Component {
public:
	JumpComponent(bool jumping = false, bool launched = false)
		: jumping(jumping),
		launched(launched),
		isLaunchingTimer(0) {
    }

    inline bool isJumping() const {
        return jumping;
    }

    inline void setJumping(bool jumping) {
        this->jumping = jumping;
    }
	
	inline bool checkIsOkToLaunch() const {
		return isLaunchingTimer.isExpired();
	}

	inline bool isLaunched() const {
		return launched;
	}

	inline void setLaunched(bool launched) {
		this->launched = launched;
	}

	inline Timer getIsLaunchingTimer(){
		return isLaunchingTimer;
	}

private:
    bool jumping;
	bool launched;
	Timer isLaunchingTimer;
};
