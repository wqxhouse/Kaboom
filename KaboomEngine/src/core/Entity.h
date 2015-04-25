#pragma once

#include <typeinfo>
#include <unordered_map>

class Component;

class Entity {
public:
    Entity(const unsigned int &id);
    ~Entity();

    void attachComponent(Component *component);

    template <typename T>
    bool hasComponent() const {
        return components.count(&typeid(T)) > 0;
    }

    template <typename T>
    T *getComponent() const {
        if (hasComponent<T>()) {
            return static_cast<T *>(components.at(&typeid(T)));
        } else {
            return nullptr;
        }
    }

    const unsigned int &getId() const;

private:
    unsigned int id;
    std::unordered_map<const std::type_info *, Component *> components;
};
