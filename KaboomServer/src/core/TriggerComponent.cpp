#include "TriggerComponent.h"

TriggerComponent::TriggerComponent(btGhostObject *ghostObject)
        : ghostObject(ghostObject) {
}

TriggerComponent::~TriggerComponent() {
}

btGhostObject *TriggerComponent::getGhostObject() const {
    return ghostObject;
}
