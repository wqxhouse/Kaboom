#include "ExplosionComponent.h"

ExplosionComponent::ExplosionComponent(ExplosionHandler *handler)
        : handler(handler) {
}

ExplosionHandler *ExplosionComponent::getHandler() const {
    return handler;
}
