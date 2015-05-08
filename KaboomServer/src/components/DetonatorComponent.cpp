#include "DetonatorComponent.h"

DetonatorComponent::DetonatorComponent(Entity *bomb)
        : bomb(bomb),
          ready(false) {
}

Entity *DetonatorComponent::getBomb() const {
    return bomb;
}

bool DetonatorComponent::isReady() const {
    return ready;
}

void DetonatorComponent::setReady(bool ready) {
    this->ready = ready;
}

bool DetonatorComponent::isDetonated() const {
    return detonated;
}

void DetonatorComponent::setDetonated(bool detonated) {
    this->detonated = detonated;
}
