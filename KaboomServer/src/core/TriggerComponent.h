#pragma once

#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include <core/Component.h>

class TriggerComponent : public Component {
public:
    TriggerComponent(btGhostObject *ghostObject);
    ~TriggerComponent();

    btGhostObject *getGhostObject() const;

private:
    btGhostObject *ghostObject;
};
