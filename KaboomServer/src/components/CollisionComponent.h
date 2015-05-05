#pragma once

#include <unordered_set>

#include <components/Component.h>

#include "../core/CollisionHandler.h"

class Entity;

class CollisionComponent : public Component {
public:
    CollisionComponent(CollisionHandler *handler);

    CollisionHandler *getHandler() const;

    void addContactEntity(Entity *entity);
    void clearContactEntities();

    const bool &isCollided() const;
    void setCollided(bool collided);

    const std::unordered_set<Entity *> &getContactEntities() const;

private:
    CollisionHandler *handler;

    bool collided;
    std::unordered_set<Entity *> contactEntities;
};
