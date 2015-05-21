#pragma once

#include <components/Component.h>

class Entity;

class DetonatorComponent : public Component {
public:
    DetonatorComponent()
            : ready(false),
              detonated(false) {
    }

    inline std::vector<Entity *> &getBombs() {
        return bombs;
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
    std::vector<Entity *> bombs;

    bool ready;
    bool detonated;
};
