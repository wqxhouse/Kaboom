#include "Entity.h"

Entity::Entity(unsigned int id)
    : id(id) {
}

Entity::~Entity() {
}

void Entity::attachComponent(Component *component) {
    components[component->getType()] = component;
}

bool Entity::hasComponent(ComponentType type) {
    return components.count(type) > 0;
}

Component *Entity::getComponent(ComponentType type) {
    if (hasComponent(type)) {
        return components[type];
    } else {
        return NULL;
    }
}

unsigned int Entity::getId() const {
    return id;
}
