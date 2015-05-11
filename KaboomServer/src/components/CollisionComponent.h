#pragma once

#include <unordered_set>

#include <components/Component.h>

class Entity;

class CollisionComponent : public Component {
public:
    inline void addContactEntity(Entity *entity) {
        contactEntities.insert(entity);
    }

    inline void clearContactEntities() {
        contactEntities.clear();
    }

    inline bool isCollided() const {
        return collided;
    }

    inline void setCollided(bool collided) {
        this->collided = collided;
    }

    inline const std::unordered_set<Entity *> &getContactEntities() const {
        return contactEntities;
    }

private:
    bool collided;
    std::unordered_set<Entity *> contactEntities;
};
