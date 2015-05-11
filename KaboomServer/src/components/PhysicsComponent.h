#pragma once

#include <components/Component.h>

class btRigidBody;

class PhysicsComponent : public Component {
public:
    PhysicsComponent(btRigidBody *rigidBody)
            : rigidBody(rigidBody) {
    }

    inline btRigidBody *getRigidBody() const {
        return rigidBody;
    }

    inline void setRigidBody(btRigidBody *rigidBody) {
        this->rigidBody = rigidBody;
    }

private:
    btRigidBody *rigidBody;
};
