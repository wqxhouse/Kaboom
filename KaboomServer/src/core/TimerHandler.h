#pragma once

class Game;
class Entity;

class TimerHandler {
public:
    virtual ~TimerHandler() {}

    virtual void handle(Game *game, Entity *entity) const = 0;
};
