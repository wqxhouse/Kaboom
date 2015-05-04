#pragma once

#include <unordered_set>

#include <core/Component.h>

#include "CollisionController.h"
#include "DefaultCollisionController.h"

class Entity;

class CollisionComponent : public Component {
public:
    CollisionComponent(CollisionController *controller);

    CollisionController *getController() const;

    void addContactEntity(Entity *entity);
    void clearContactEntities();

    const bool &isCollided() const;
    void setCollided(bool collided);

    const std::unordered_set<Entity *> &getContactEntities() const;

private:
    CollisionController *controller;

    bool collided;
    std::unordered_set<Entity *> contactEntities;
};
