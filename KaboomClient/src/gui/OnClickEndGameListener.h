#pragma once
#include "GameGUIListener.h"
class OnClickEndGameListener :
	public GameGUIListener
{
public:
	OnClickEndGameListener(Game *game)
		: GameGUIListener(game) {}
	~OnClickEndGameListener();

	virtual void ProcessEvent(Rocket::Core::Event& event);
};

