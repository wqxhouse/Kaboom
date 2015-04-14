#pragma once

#include "Entity.h"

class Entity;

class Component {
public:
    virtual ~Component() {}

    Entity *getParentEntity() const;
    void setParentEntity(Entity *parent);

protected:
    Entity *parent;
};
