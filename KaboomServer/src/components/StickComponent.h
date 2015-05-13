#pragma once

#include <components/Component.h>

#include "../messaging/MessageHandler.h"

class Entity;

class StickComponent : public Component {
public:
    StickComponent()
            : attached(false),
              attachedEntity(nullptr) {
    }

    inline bool isAttached() const {
        return attached;
    }

    inline void setAttached(bool attached) {
        this->attached = attached;
    }

    inline Entity* getAttachedEntity() const {
        return attachedEntity;
    }

    void setAttachedEntity(Entity *attachedEntity) {
        this->attachedEntity = attachedEntity;
    }

private:
    bool attached;
    Entity *attachedEntity;
};
