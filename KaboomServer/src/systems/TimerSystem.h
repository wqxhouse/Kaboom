#pragma once

#include <systems/EntitySystem.h>

class Game;

// Notifies timer handler when a timer expires.
class TimerSystem : public EntitySystem {
public:
    TimerSystem(Game *game);

    virtual bool checkEntity(Entity *entity);

    virtual void processEntity(Entity *entity);

private:
    Game *game;
};
