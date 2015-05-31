#include "stdafx.h"
#include "OnClickEndGameListener.h"


OnClickEndGameListener::~OnClickEndGameListener()
{
}

void OnClickEndGameListener::ProcessEvent(Rocket::Core::Event& event)
{
	setGameState(EXIT_START_SCREEN_MODE);
}