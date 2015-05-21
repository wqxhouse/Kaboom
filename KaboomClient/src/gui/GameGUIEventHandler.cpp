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
			healthTable->GetChild(i)->SetProperty("color", "red");
			continue;
		}
		healthTable->GetChild(i)->SetProperty("background-color", "green");
		healthTable->GetChild(i)->SetProperty("color", "green");
	}
	//win->GetChild(1);			
}

void GameGUIEventHandler::handle(const AmmoAmountEvent &e, InventoryComponent *bombConCom) const
{
	Rocket::Core::ElementDocument *window1 = _guiManager->getWindow(0);
	Rocket::Core::Element * body = window1->GetChild(1);
	Rocket::Core::Element *ammoTable = body->GetFirstChild();

	//hardcode the number amount of bomb; currently there are 3 bombs
	for (int i = 0; i < 3; i++)
	{
		int temp = e.getAmmoAmount(i);
		ammoTable->GetChild(i)->GetFirstChild()->SetInnerRML(std::to_string(temp).c_str());
	}
}
void  GameGUIEventHandler::handle(const ScoreEvent &e)const {
	Rocket::Core::ElementDocument *window2 = _guiManager->getWindow(2);
	Rocket::Core::Element * table = window2->GetChild(0);
	bool flag = true;
	for (int i = 1; i<table->GetNumChildren(); i++){
		Rocket::Core::Element * tr = table->GetChild(i);
		Rocket::Core::String s=tr->GetChild(0)->GetInnerRML();
		Rocket::Core::String d = Rocket::Core::String(std::to_string(e.getPlayerId()).c_str());
		if (s==d){
			flag = false;
			tr->GetChild(1)->SetInnerRML(std::to_string(e.getKills()).c_str());
			tr->GetChild(2)->SetInnerRML(std::to_string(e.getDeaths()).c_str());
		}
	}
	if (flag){
		//TODO add tr in with player id kills deaths
		Rocket::Core::Element* new_tr = window2->CreateElement("p");
		Rocket::Core::Element* new_td_id = window2->CreateElement("div");
		Rocket::Core::Element* new_td_kills = window2->CreateElement("div");
		Rocket::Core::Element* new_td_death = window2->CreateElement("div");

		new_td_id->SetInnerRML(std::to_string(e.getPlayerId()).c_str());
		new_td_kills->SetInnerRML(std::to_string(e.getKills()).c_str());
		new_td_death->SetInnerRML(std::to_string(e.getDeaths()).c_str());

		new_tr->AppendChild(new_td_id);
		new_tr->AppendChild(new_td_kills);
		new_tr->AppendChild(new_td_death);
		table->AppendChild(new_tr);
		printf("I am in\n");
	}

}

void GameGUIEventHandler::changeWeapon(int weapon) const
{
	Rocket::Core::ElementDocument *window1 = _guiManager->getWindow(0);
	Rocket::Core::Element * body = window1->GetChild(1);
	Rocket::Core::Element *ammoTable = body->GetFirstChild();

	//hardcode the number amount of bomb; currently there are 3 bombs
	for (int i = 0; i < 3; i++)
	{
		if (weapon == i)
			ammoTable->GetChild(i)->SetProperty("color", "red");
		else
			ammoTable->GetChild(i)->SetProperty("color", "purple");
	}
}
void GameGUIEventHandler::showScoreBoard() const{
	Core::getInGameLibRocketGUIManager()->getWindow(0)->Hide();
	Core::getInGameLibRocketGUIManager()->getWindow(1)->Hide();
	Core::getInGameLibRocketGUIManager()->getWindow(2)->Show();

}
void GameGUIEventHandler::hideScoreBoard() const{
	Core::getInGameLibRocketGUIManager()->getWindow(0)->Show();
	Core::getInGameLibRocketGUIManager()->getWindow(1)->Hide();
	Core::getInGameLibRocketGUIManager()->getWindow(2)->Hide();

}