#pragma once

#include <core/Component.h>

class InputComponent : public Component {
public:
    InputComponent();
    InputComponent(bool movingForward,
            bool movingBackward,
            bool movingLeft,
            bool movingRight,
            bool jumping,
            bool firing);
    ~InputComponent();

    const bool &isMovingForward() const;
    const bool &isMovingBackward() const;
    const bool &isMovingLeft() const;
    const bool &isMovingRight() const;
    const bool &isJumping() const;
    const bool &isFiring() const;

    void setMovingForward(const bool &movingForward);
    void setMovingBackward(const bool &movingBackward);
    void setMovingLeft(const bool &movingLeft);
    void setMovingRight(const bool &movingRight);
    void setJumping(const bool &jumping);
    void setFiring(const bool &firing);

private:
    bool movingForward;
    bool movingBackward;
    bool movingLeft;
    bool movingRight;
    bool jumping;
    bool firing;
};
