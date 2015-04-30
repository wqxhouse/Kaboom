#include "ExplosionComponent.h"

ExplosionComponent::ExplosionComponent(btGhostObject *ghostObject, bool exploded)
        : ghostObject(ghostObject),
          exploded(exploded) {
}

btGhostObject *ExplosionComponent::getGhostObject() const {
    return ghostObject;
}

const bool &ExplosionComponent::isExploded() const {
    return exploded;
}

void ExplosionComponent::setExploded(const bool &exploded) {
    this->exploded = exploded;
}
