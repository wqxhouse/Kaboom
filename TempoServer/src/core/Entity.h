#pragma once

#include <typeinfo>
#include <unordered_map>

#include "Component.h"

class Component;

class Entity {
public:
    Entity(unsigned int id);
    ~Entity();

    void attachComponent(Component *component);

    template <typename T>
    bool hasComponent() {
        return components.count(&typeid(T)) > 0;
    }

    template <typename T>
    T *getComponent() {
        if (hasComponent<T>()) {
            return static_cast<T *>(components[&typeid(T)]);
        } else {
            return nullptr;
        }
    }

    virtual void update(float timeStep);

    unsigned int getId() const;

private:
    unsigned int id;
    std::unordered_map<const std::type_info *, Component *> components;
};
