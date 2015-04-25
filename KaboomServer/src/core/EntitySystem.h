#pragma once

class Game;

class EntitySystem {
public:
    EntitySystem(Game *game);
    ~EntitySystem();

    virtual void update(float timeStep) = 0;

protected:
    Game *game;
};
