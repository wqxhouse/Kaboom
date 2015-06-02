#pragma once

#include <components/Component.h>

class InputComponent : public Component {
public:
    InputComponent(
            bool movingForward = false,
            bool movingBackward = false,
            bool movingLeft = false,
            bool movingRight = false,
            bool jumping = false,
            bool attacking1 = false,
            bool attacking2 = false)
            : movingForward(movingForward),
              movingBackward(movingBackward),
              movingLeft(movingLeft),
              movingRight(movingRight),
              jumping(jumping),
              attacking1(attacking1),
              attacking2(attacking2) {
    }

    inline bool isMovingForward() const {
        return movingForward;
    }

    inline bool isMovingBackward() const {
        return movingBackward;
    }

    inline bool isMovingLeft() const {
        return movingLeft;
    }

    inline bool isMovingRight() const {
        return movingRight;
    }

    inline bool isJumping() const {
        return jumping;
    }

    inline bool isAttacking1() const {
        return attacking1;
    }

    inline bool isAttacking2() const {
        return attacking2;
    }

    inline void setMovingForward(bool movingForward) {
        this->movingForward = movingForward;
    }

    inline void setMovingBackward(bool movingBackward) {
        this->movingBackward = movingBackward;
    }

    inline void setMovingLeft(bool movingLeft) {
        this->movingLeft = movingLeft;
    }

    inline void setMovingRight(bool movingRight) {
        this->movingRight = movingRight;
    }

    inline void setJumping(bool jumping) {
        this->jumping = jumping;
    }

    inline void setAttacking1(bool attacking1) {
        this->attacking1 = attacking1;
    }

    inline void setAttacking2(bool attacking2) {
        this->attacking2 = attacking2;
    }

private:
    bool movingForward;
    bool movingBackward;
    bool movingLeft;
    bool movingRight;
    bool jumping;
    bool attacking1;
    bool attacking2;
};
