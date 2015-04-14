#pragma once

#include "Entity.h"

class Entity;

class Component {
public:
    virtual ~Component() {}

    Entity *getEntity() const;
    void setEntity(Entity *entity);

protected:
    Entity *entity;
};
