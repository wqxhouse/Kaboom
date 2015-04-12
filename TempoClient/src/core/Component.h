#pragma once

#include "Entity.h"
#include "GameStateData.h"

class Entity;

class Component {
public:
    virtual ~Component() {}

    virtual void update(const GameStateData &data);

    Entity *getEntity() const;
    void setEntity(Entity *entity);

protected:
    Entity *entity;
};
