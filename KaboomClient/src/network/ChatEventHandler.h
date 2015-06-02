#pragma once

#include <network/EventHandler.h>

class Game;

class ChatEventHandler : public EventHandler {
public:
    ChatEventHandler(Game *game);

    virtual void handle(const Event &e) const;

private:
    Game *game;
};
