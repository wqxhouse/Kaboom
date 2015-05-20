#pragma once

#include <components/Component.h>

class JumpComponent : public Component {
public:
    JumpComponent(bool jumping = false, bool launched = false)
            : jumping(jumping),
			  launched(launched){
    }

    inline bool isJumping() const {
        return jumping;
    }

    inline void setJumping(bool jumping) {
        this->jumping = jumping;
    }

	inline bool isLaunched() const {
		return launched;
	}

	inline void setLaunched(bool launched) {
		this->launched = launched;
	}

private:
    bool jumping;
	bool launched;
};
