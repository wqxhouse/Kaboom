#include "GameGUIEventHandler.h"

#include <Core.h>
#include "../core/Game.h"

GameGUIEventHandler::GameGUIEventHandler(Game *game)
	: _game(game), _guiManager(Core::getInGameLibRocketGUIManager())
{
}

void GameGUIEventHandler::handle(const HealthEvent &e, HealthComponent *healthComponent) const
{
	Rocket::Core::ElementDocument *window1 = _guiManager->getWindow(0);
	Rocket::Core::Element * table = window1->GetFirstChild();
	Rocket::Core::Element *tr = table->GetFirstChild();
	for (int i = 0; i < tr->GetNumChildren(); i++){
		// printf("\nthe health is at %d\n", healthComponent->getHealthAmount());
		if (healthComponent->getHealthAmount() / 20.0 <= i){
			tr->GetChild(i)->SetProperty("background-color", "red");
			continue;
		}
		tr->GetChild(i)->SetProperty("background-color", "green");
	}
	//win->GetChild(1);			
}