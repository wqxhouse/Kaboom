#include "stdafx.h"
#include "OnClickRespawnListener.h"
#include "../core/Game.h"

OnClickRespawnListener::~OnClickRespawnListener()
{

}

void OnClickRespawnListener::ProcessEvent(Rocket::Core::Event& event)
{
	game->getGameClient().sendPlayerRespawnRequestEvent();
	std::cout << "test" << std::endl;
}
void OnClickRespawnListener::setGame(Game *g){

	game = g;
}