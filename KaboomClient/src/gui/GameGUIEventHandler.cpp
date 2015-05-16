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
	Rocket::Core::Element * body = window1->GetFirstChild();
	Rocket::Core::Element *healthTable = body->GetFirstChild();
	for (int i = 0; i < healthTable->GetNumChildren(); i++){
		//printf("\nthe health is at %d\n", healthComponent->getAmount());
		if (healthComponent->getAmount() / 20.0 <= i){
			healthTable->GetChild(i)->SetProperty("background-color", "red");
			continue;
		}
		healthTable->GetChild(i)->SetProperty("background-color", "green");
	}
	//win->GetChild(1);			
}

void GameGUIEventHandler::handle(const AmmoAmountEvent &e, BombContainerComponent *bombConCom) const
{
	Rocket::Core::ElementDocument *window1 = _guiManager->getWindow(0);
	Rocket::Core::Element * body = window1->GetChild(1);
	Rocket::Core::Element *ammoTable = body->GetFirstChild();

	//hardcode the number amount of bomb; currently there are 3 bombs
	for (int i = 0; i < 3; i++)
	{
		int temp = e.getAmmoAmount(i);
		ammoTable->GetChild(i)->SetInnerRML(std::to_string(temp).c_str());
	}
}