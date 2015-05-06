#include "Entity.h"

#include "../components/Component.h"

Entity::Entity(unsigned int id, const EntityType &type)
        : id(id),
          type(type) {
}

Entity::~Entity() {
    for (auto it : components) {
        delete it.second;
    }
}

void Entity::attachComponent(Component *component) {
    components[&typeid(*component)] = component;
}

unsigned int Entity::getId() const {
    return id;
}

const EntityType &Entity::getType() const {
    return type;
}

void Entity::setType(const EntityType &type) {
    this->type = type;
}
