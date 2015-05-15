#pragma once
#include "C:\Users\xlunardragoonx\Documents\GitHub\Kaboom\KaboomClient\src\gui\GameGUIListener.h"
class OnClickEndGameListener :
	public GameGUIListener
{
public:
	OnClickEndGameListener(Game *game)
		: GameGUIListener(game) {}
	~OnClickEndGameListener();

	virtual void ProcessEvent(Rocket::Core::Event& event);
};

