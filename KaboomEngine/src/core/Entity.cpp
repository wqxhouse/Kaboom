#include "Entity.h"

#include "Component.h"

Entity::Entity(const unsigned int &id)
        : id(id) {
}

Entity::~Entity() {
    for (auto it : components) {
        delete it.second;
    }
}

void Entity::attachComponent(Component *component) {
    component->setParentEntity(this);
    components[&typeid(*component)] = component;
}

const unsigned int &Entity::getId() const {
    return id;
}
