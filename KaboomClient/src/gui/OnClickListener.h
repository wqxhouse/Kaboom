#pragma once

#include "GameGUIListener.h"

class OnClickListener : public GameGUIListener
{
public:
	OnClickListener(Game *game)
		: GameGUIListener(game) {}

	virtual void ProcessEvent(Rocket::Core::Event& event);
	void setMode(GameStateMachine );
private:
	GameStateMachine mode;
};
