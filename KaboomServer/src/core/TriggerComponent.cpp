#include "TriggerComponent.h"

TriggerComponent::TriggerComponent(btGhostObject *ghostObject)
        : ghostObject(ghostObject) {
}

TriggerComponent::~TriggerComponent() {
}

btGhostObject *TriggerComponent::getGhostObject() const {
    return ghostObject;
}

void TriggerComponent::addTriggerEntity(Entity *entity) {
    triggerEntities.insert(entity);
}

void TriggerComponent::removeTriggerEntity(Entity *entity) {
    triggerEntities.erase(entity);
}

void TriggerComponent::clearTriggerEntities() {
    triggerEntities.clear();
}

const std::unordered_set<Entity *> &TriggerComponent::getTriggerEntities() const {
    return triggerEntities;
}
