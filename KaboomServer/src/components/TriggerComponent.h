#pragma once

#include <unordered_set>

#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include <components/Component.h>

class Entity;

class TriggerComponent : public Component {
public:
    TriggerComponent(btGhostObject *ghostObject)
            : ghostObject(ghostObject) {
    }

    inline void addTriggerEntity(Entity *entity) {
        triggerEntities.insert(entity);
    }

    inline void removeTriggerEntity(Entity *entity) {
        triggerEntities.erase(entity);
    }

    inline void clearTriggerEntities() {
        triggerEntities.clear();
    }

    inline btGhostObject *getGhostObject() const {
        return ghostObject;
    }

    inline const std::unordered_set<Entity *> &getTriggerEntities() const {
        return triggerEntities;
    }

private:
    btGhostObject *ghostObject;
    std::unordered_set<Entity *> triggerEntities;
};
