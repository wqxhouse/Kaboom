#include "Component.h"

Entity *Component::getParentEntity() const {
    return parent;
}

void Component::setParentEntity(Entity *parent) {
    this->parent = parent;
}
