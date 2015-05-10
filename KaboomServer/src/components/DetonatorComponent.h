#pragma once

#include <components/Component.h>

class Entity;

class DetonatorComponent : public Component {
public:
    DetonatorComponent(Entity *bomb);

    Entity *getBomb() const;
    bool isReady() const;
    bool isDetonated() const;

    void setReady(bool ready);
    void setDetonated(bool detonated);

private:
    Entity *bomb;

    bool ready;
    bool detonated;
};
