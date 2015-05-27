#include "TimeEventHandler.h"
#include <network/TimeEvent.h>
#include "../core/Game.h"

TimeEventHandler::TimeEventHandler(Game *game)
: game(game) {
}

void TimeEventHandler::handle(const Event &e) const {
	const TimeEvent &evt = static_cast<const TimeEvent &>(e);

	//game->getGameGUIEventHandler()->handle(evt);
}
