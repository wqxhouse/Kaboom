#pragma once

#include <components/Component.h>

class Entity;

class OwnerComponent : public Component {
public:
    OwnerComponent(Entity *entity)
            : entity(entity) {
    }

    inline Entity *getEntity() const {
        return entity;
    }

private:
    Entity *entity;
};
