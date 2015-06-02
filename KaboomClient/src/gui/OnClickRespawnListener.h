#pragma once
#include "GameGUIListener.h"


class OnClickRespawnListener :
	public GameGUIListener
{
public:
	OnClickRespawnListener(Game *game)
		: GameGUIListener(game){}
	~OnClickRespawnListener();

	virtual void ProcessEvent(Rocket::Core::Event& event);
	void setGame(Game *g);

private:
	Game *game;
};

