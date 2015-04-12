#include "Entity.h"

Entity::Entity(unsigned int id)
    : id(id) {
}

Entity::~Entity() {
}

void Entity::attachComponent(Component *component) {
    component->setEntity(this);
    components[&typeid(*component)] = component;
}

void Entity::update(float timeStep) {
    for (auto it : components) {
        it.second->update(timeStep);
    }
}

unsigned int Entity::getId() const {
    return id;
}
