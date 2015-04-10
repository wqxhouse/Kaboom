#pragma once

#include <map>

#include "Component.h"

class Entity {
public:
    Entity(unsigned int id);
    ~Entity();

    void attachComponent(Component *component);

    bool hasComponent(ComponentType type);
    Component *Entity::getComponent(ComponentType type);

    unsigned int getId() const;

private:
    unsigned int id;
    std::map<ComponentType, Component *> components;
};
