#pragma once

#include <unordered_set>

#include <core/Component.h>

class Entity;

class CollisionComponent : public Component {
public:
    CollisionComponent();

    void addContactEntity(Entity *entity);
    void clearContactEntities();

    const bool &isCollided() const;
    void setCollided(bool collided);

    const std::unordered_set<Entity *> &getContactEntities() const;

private:
    bool collided;
    std::unordered_set<Entity *> contactEntities;
};
