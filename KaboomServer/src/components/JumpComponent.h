#pragma once

#include <components/Component.h>

class JumpComponent : public Component {
public:
    JumpComponent(bool jumping = false)
            : jumping(jumping) {
    }

    inline bool isJumping() const {
        return jumping;
    }

    inline void setJumping(bool jumping) {
        this->jumping = jumping;
    }

private:
    bool jumping;
};
