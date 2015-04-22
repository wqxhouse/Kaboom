#pragma once

class Entity;

class Component {
public:
    virtual ~Component() {}

    Entity *getParentEntity() const;
    void setParentEntity(Entity *parent);

private:
    Entity *parent;
};
