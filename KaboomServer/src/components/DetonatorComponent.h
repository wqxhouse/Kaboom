#pragma once

#include <components/Component.h>

class Entity;

class DetonatorComponent : public Component {
public:
    DetonatorComponent(Entity *bomb)
            : bomb(bomb) {
    }

    inline Entity *getBomb() const {
        return bomb;
    }

    inline bool isReady() const {
        return ready;
    }

    inline bool isDetonated() const {
        return detonated;
    }

    inline void setReady(bool ready) {
        this->ready = ready;
    }

    inline void setDetonated(bool detonated) {
        this->detonated = detonated;
    }

private:
    Entity *bomb;

    bool ready;
    bool detonated;
};
