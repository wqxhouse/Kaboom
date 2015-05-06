#pragma once

#include <components/Component.h>

class btRigidBody;

class PhysicsComponent : public Component {
public:
    PhysicsComponent(btRigidBody *rigidBody);

    btRigidBody *getRigidBody() const;
    void setRigidBody(btRigidBody *rigidBody);

private:
    btRigidBody *rigidBody;
};
