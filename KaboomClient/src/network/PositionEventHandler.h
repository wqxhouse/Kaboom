#pragma once

#include <network/EventHandler.h>
#include <network/PositionEvent.h>

#include "../core/Game.h"

class PositionEventHandler : public EventHandler<PositionEvent> {
public:
    PositionEventHandler(Game *game);
    ~PositionEventHandler();

    void handle(const PositionEvent &evt) const;

private:
    Game *game;
};
