#pragma once

#include "Entity.h"

class Entity;

class Component {
public:
    virtual ~Component() {}

    virtual void update(float timeStep);

    Entity *getEntity() const;
    void setEntity(Entity *entity);

protected:
    Entity *entity;
};
