#pragma once

#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include <core/Component.h>

class ExplosionComponent : public Component {
public:
    ExplosionComponent(btGhostObject *ghostObject, bool exploded);

    btGhostObject *getGhostObject() const;
    const bool &isExploded() const;

    void setExploded(const bool &exploded);

private:
    btGhostObject *ghostObject;
    bool exploded;
};
