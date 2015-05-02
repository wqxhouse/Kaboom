#pragma once

#include <unordered_set>

#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include <core/Component.h>
#include <core/Entity.h>

class TriggerComponent : public Component {
public:
    TriggerComponent(btGhostObject *ghostObject);
    ~TriggerComponent();

    btGhostObject *getGhostObject() const;

    void addTriggerEntity(Entity *entity);
    void removeTriggerEntity(Entity *entity);
    void clearTriggerEntities();

    const std::unordered_set<Entity *> &getTriggerEntities() const;

private:
    btGhostObject *ghostObject;
    std::unordered_set<Entity *> triggerEntities;
};
