#pragma once

#include <network/EventHandler.h>

class Game;

class DeleteEventHandler : public EventHandler {
public:
	DeleteEventHandler(Game *game);
	~DeleteEventHandler();

    virtual void handle(const Event &evt) const;

private:
    Game *game;
};
