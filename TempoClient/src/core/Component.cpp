#include "Component.h"

void Component::update(const GameStateData &data) {
}

Entity *Component::getEntity() const {
    return entity;
}

void Component::setEntity(Entity *entity) {
    this->entity = entity;
}
