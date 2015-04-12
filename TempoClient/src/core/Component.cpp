#include "Component.h"

void Component::update(const GameStateData &gameState) {
}

Entity *Component::getEntity() const {
    return entity;
}

void Component::setEntity(Entity *entity) {
    this->entity = entity;
}
