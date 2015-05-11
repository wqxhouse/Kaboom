#include "EntitySystem.h"

void EntitySystem::preprocessEntities(std::vector<Entity *> entities) {
}

void EntitySystem::processEntities(std::vector<Entity *> entities) {
    for (Entity *entity : entities) {
        if (checkEntity(entity)) {
            processEntity(entity);
        }
    }
}
