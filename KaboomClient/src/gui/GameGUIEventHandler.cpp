#include "GameGUIEventHandler.h"

#include <Core.h>
#include <time.h>
#include "../core/Game.h"

GameGUIEventHandler::GameGUIEventHandler(Game *game)
	: _game(game), _guiManager(Core::getInGameLibRocketGUIManager())
{
}

void GameGUIEventHandler::hudScore(const ScoreEvent &evt) const{
	Rocket::Core::ElementDocument *window1 = _guiManager->getWindow(0);
	Rocket::Core::Element * health = window1->GetElementById("health");
	Rocket::Core::Element * inner = health->GetElementById("inner");
	inner->GetElementById("kills")->SetInnerRML(std::to_string(evt.getKills()).c_str());
	inner->GetElementById("deaths")->SetInnerRML(std::to_string(evt.getDeaths()).c_str());
}

void GameGUIEventHandler::handle(const HealthEvent &e, HealthComponent *healthComponent) const
{
	Rocket::Core::ElementDocument *window1 = _guiManager->getWindow(0);
	Rocket::Core::Element * health = window1->GetElementById("health");
	Rocket::Core::Element * inner = health->GetElementById("inner");
	//Rocket::Core::String hpAmount;
	//left:16 for 2 digits.
	//left: for 3 digits.
	//left: for 1 digit.
	inner->GetElementById("hp")->SetInnerRML(std::to_string(healthComponent->getAmount()).c_str());
	int currentHP = healthComponent->getAmount();
	if (currentHP == 100)
		health->SetAttribute("class", "hp_100");
	else if (currentHP >= 92)
		health->SetAttribute("class", "hp_92");
	else if (currentHP >= 83)
		health->SetAttribute("class", "hp_83");
	else if (currentHP >= 75)
		health->SetAttribute("class", "hp_75");
	else if (currentHP >= 67)
		health->SetAttribute("class", "hp_67");
	else if (currentHP >= 58)
		health->SetAttribute("class", "hp_58");
	else if (currentHP >= 50)
		health->SetAttribute("class", "hp_50");
	else if (currentHP >= 42)
		health->SetAttribute("class", "hp_42");
	else if (currentHP >= 33)
		health->SetAttribute("class", "hp_33");
	else if (currentHP >= 25)
		health->SetAttribute("class", "hp_25");
	else if (currentHP >= 19)
		health->SetAttribute("class", "hp_19");
	else if (currentHP >= 15)
		health->SetAttribute("class", "hp_15");
	else if (currentHP >= 12)
		health->SetAttribute("class", "hp_12");
	else if (currentHP >= 10)
		health->SetAttribute("class", "hp_10");
	else if (currentHP >= 8)
		health->SetAttribute("class", "hp_8");
	else if (currentHP >= 6)
		health->SetAttribute("class", "hp_6");
	else if (currentHP >= 4)
		health->SetAttribute("class", "hp_4");
	else if (currentHP >= 2)
		health->SetAttribute("class", "hp_2");
	else if (currentHP > 0)
		health->SetAttribute("class", "hp_15");
	else
		health->SetAttribute("class", "hp_0");
	//win->GetChild(1);			
}

void GameGUIEventHandler::handle(const AmmoAmountEvent &e, InventoryComponent *bombConCom) const
{
	Rocket::Core::ElementDocument *window1 = _guiManager->getWindow(0);
	Rocket::Core::Element * body = window1->GetChild(1);
	Rocket::Core::Element *ammoTable = body->GetFirstChild();

	//hardcode the number amount of bomb; currently there are 3 bombs
	for (int i = 0; i < ammoTable->GetNumChildren(); i++)
	{
		int temp = e.getAmmoAmount(i);
		ammoTable->GetChild(i)->GetFirstChild()->GetChild(1)->GetFirstChild()->SetInnerRML(std::to_string(temp).c_str());
	}
}

void  GameGUIEventHandler::deletePlayer(const DisconnectEvent &e)const {
	Rocket::Core::ElementDocument *window2 = _guiManager->getWindow(2);
	Rocket::Core::Element * table = window2->GetChild(0);
	Rocket::Core::Element * score = table->GetFirstChild();

	bool flag = true;
	for (int i = 1; i < score->GetNumChildren(); i++){
		Rocket::Core::Element * tr = score->GetChild(i);
		Rocket::Core::String s = tr->GetChild(0)->GetId();
		Rocket::Core::String d = Rocket::Core::String(std::to_string(e.getPlayerId()).c_str());
		if (s == d){
			score->RemoveChild(tr);
	
		}
	}
}

void  GameGUIEventHandler::deleteAllPlayers()const {
	Rocket::Core::ElementDocument *window2 = _guiManager->getWindow(2);
	Rocket::Core::Element * table = window2->GetChild(0);
	Rocket::Core::Element * score = table->GetFirstChild();
	bool flag = true;
	while (score->GetNumChildren()>1){
		Rocket::Core::Element * tr = score->GetChild(1);
		//Rocket::Core::String s = tr->GetChild(0)->GetId();
		//Rocket::Core::String d = Rocket::Core::String(std::to_string(e.getPlayerId()).c_str());
		score->RemoveChild(tr);
	}
	changeWeapon(0);
}

void  GameGUIEventHandler::handle(const ScoreEvent &e, std::string name)const {
	Rocket::Core::ElementDocument *window2 = _guiManager->getWindow(2);
	Rocket::Core::Element * table = window2->GetChild(0);
	Rocket::Core::Element * score = table->GetFirstChild();

	bool flag = true;
	for (int i = 1; i<score->GetNumChildren(); i++){
		Rocket::Core::Element * tr = score->GetChild(i);
		Rocket::Core::String s=tr->GetChild(0)->GetId();
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
		new_td_id->SetClassNames("id");
		Rocket::Core::Element* new_td_kills = window2->CreateElement("div");
		new_td_kills->SetClassNames("kills");
		Rocket::Core::Element* new_td_death = window2->CreateElement("div");
		new_td_death->SetClassNames("deaths");

		new_td_id->SetId(std::to_string(e.getPlayerId()).c_str());

		if (name != "")
			new_td_id->SetInnerRML(name.c_str());
		else
			new_td_id->SetInnerRML(std::to_string(e.getPlayerId()).c_str());

		new_td_kills->SetInnerRML(std::to_string(e.getKills()).c_str());
		new_td_death->SetInnerRML(std::to_string(e.getDeaths()).c_str());

		new_tr->AppendChild(new_td_id);
		new_tr->AppendChild(new_td_kills);
		new_tr->AppendChild(new_td_death);
		score->AppendChild(new_tr);
		printf("I am in\n");
	}

}

void  GameGUIEventHandler::handle(const PlayerDeathEvent &e)const {
	//hide main HUD
	_guiManager->getWindow(0)->GetElementById("crosshair")->SetProperty("visibility","hidden");
	 Core::enableDeathScreen();
	 changeWeapon(0);

	 //show the death screen
	 _guiManager->getWindow(4)->Show();
}

void GameGUIEventHandler::handle(const PlayerRespawnEvent &e) const
{
	//hide the death screen
	_guiManager->getWindow(4)->Hide();

	//hide main HUD
	Core::disableDeathScreen();
	_guiManager->getWindow(0)->GetElementById("crosshair")->SetProperty("visibility", "visible");
}

void GameGUIEventHandler::changeWeapon(int weapon) const
{
	Rocket::Core::ElementDocument *window1 = _guiManager->getWindow(0);
	Rocket::Core::Element * body = window1->GetChild(1);

	Rocket::Core::Element * current_bomb = window1->GetChild(2);
	Rocket::Core::Element * ch = window1->GetElementById("crosshair");
	Rocket::Core::Element *bomb = current_bomb->GetFirstChild();
	switch (weapon){
		case 0:
			bomb->SetClassNames("kaboom_curr");
			ch->SetClassNames("kaboom_crosshair");
			break;
		case 1:
			bomb->SetClassNames("time_curr");
			ch->SetClassNames("time_crosshair");
			break;
		case 2:
			bomb->SetClassNames("remote_curr");
			ch->SetClassNames("sticky_crosshair");
			break;
		case 3:
			bomb->SetClassNames("SM_curr");
			ch->SetClassNames("SM_crosshair");
			break;
		case 4:
			bomb->SetClassNames("fake_curr");
			ch->SetClassNames("fake_crosshair");
			break;
		default:
			break;
	}
}

int GameGUIEventHandler::endGame() const{
	Rocket::Core::ElementDocument *window2 = _guiManager->getWindow(2);
	Rocket::Core::Element * table = window2->GetChild(0);
	Rocket::Core::Element * score = table->GetFirstChild();

	bool flag = true;
	int maxKills = 0;
	int playerId = 0;
	int location=0;
	for (int i = 1; i<score->GetNumChildren(); i++){
		Rocket::Core::Element * tr = score->GetChild(i);
		Rocket::Core::String s = tr->GetChild(1)->GetInnerRML();

		for (int j = 0; j < tr->GetNumChildren(); j++){
			Rocket::Core::Element * child = tr->GetChild(j);
			child->SetProperty("color", "rgba(0,0,0,55)");
		}
		std::string numString= s.CString();
		int num = std::stoi(numString);
		if (num>= maxKills){
			maxKills = num;
			location = i;

		}
	}
	for (int i = 1; i<score->GetNumChildren(); i++){
		Rocket::Core::Element * tr = score->GetChild(i);
		Rocket::Core::String s = tr->GetChild(1)->GetInnerRML();


		std::string numString = s.CString();
		int num = std::stoi(numString);
		if (num >= maxKills){
			for (int j = 0; j < tr->GetNumChildren(); j++){
				Rocket::Core::Element * child = tr->GetChild(j);
				child->SetProperty("color", "rgba(0,0,0,255)");
			}

		}
	}
	

	//->SetProperty("color", "rgba(0,0,0,255)");
	window2->Show();
	//TODO end game screen
	return maxKills;


}

void GameGUIEventHandler::showScoreBoard() const{
	//Core::getInGameLibRocketGUIManager()->getWindow(0)->Hide();
	//Core::getInGameLibRocketGUIManager()->getWindow(1)->Hide();
	if (Core::isInGameMode())
		Core::getInGameLibRocketGUIManager()->getWindow(2)->Show();

}

void GameGUIEventHandler::hideScoreBoard() const{
	//Core::getInGameLibRocketGUIManager()->getWindow(0)->Show();
	//Core::getInGameLibRocketGUIManager()->getWindow(1)->Hide();
	Core::getInGameLibRocketGUIManager()->getWindow(2)->Hide();

}

void GameGUIEventHandler::changeTime(Game *game) const{
	Rocket::Core::ElementDocument *window1 = _guiManager->getWindow(0);
	Rocket::Core::Element * clock = window1->GetChild(3);

    const auto &startTime = game->getGameMode().getTimer().getStartTime();
    const auto duration = game->getGameMode().getTimer().getDuration();

    const auto remainingTime = duration - (std::chrono::high_resolution_clock::now() - startTime);
    const auto remainingSeconds = std::chrono::duration_cast<std::chrono::seconds>(remainingTime);

    const int seconds = remainingSeconds.count() % 60;
    const int minutes = remainingSeconds.count() / 60;

    int minutesTensDigit = minutes / 10;
    int minutesOnesDigit = minutes % 10;
    int secondsTensDigit = seconds / 10;
    int secondsOnesDigit = seconds % 10;
    
    if (minutesTensDigit < 0) { minutesTensDigit = 0; }
    if (minutesOnesDigit < 0) { minutesOnesDigit = 0; }
    if (secondsTensDigit < 0) { secondsTensDigit = 0; }
    if (secondsOnesDigit < 0) { secondsOnesDigit = 0; }

    std::string timerString =
            std::to_string(minutesTensDigit) +
            std::to_string(minutesOnesDigit) +
            ":" +
            std::to_string(secondsTensDigit) +
            std::to_string(secondsOnesDigit);
    clock->SetInnerRML(timerString.c_str());
}

void GameGUIEventHandler::changeDeathTime(Game *game)const{
}

void GameGUIEventHandler::handle(const PlayerRenameEvent &e, Player* player) const
{
	Rocket::Core::ElementDocument *window2 = _guiManager->getWindow(2);
	Rocket::Core::Element * table = window2->GetChild(0);
	Rocket::Core::Element * score = table->GetFirstChild();
	bool flag = true;
	for (int i = 1; i<score->GetNumChildren(); i++){
		Rocket::Core::Element * tr = score->GetChild(i);
		Rocket::Core::String s = tr->GetChild(0)->GetId();
		Rocket::Core::String d = Rocket::Core::String(std::to_string(e.getPlayerId()).c_str());
		if (s == d && player->getName() != ""){
			flag = false;
			tr->GetChild(0)->SetInnerRML(player->getName().c_str());
		}
	}
}

void GameGUIEventHandler::updateUserName(std::string * name) const
{
	Rocket::Core::ElementDocument *window3 = _guiManager->getWindow(3);
	Rocket::Core::Element * nameElement = window3->GetChild(1);
	nameElement->SetInnerRML(name->c_str());
}

void GameGUIEventHandler::preGame(int& color, bool &change) const
{
	Rocket::Core::ElementDocument *window0 = _guiManager->getWindow(0);
	Rocket::Core::Element * gameStateMessage = window0->GetElementById("gameStateMessage");

	if (change)
	{
		color-=5;
		if (color <= 0)
		{
			change = false;
			color = 0;
		}
	}
	else
	{
		color+=5;
		if (color >= 255)
		{
			color = 255;
			change = true;
		}
	}

	std::string newColor = "rgba(255,255,255,";
	newColor +=  std::to_string(color);
	newColor +=  ")";
	//std::cout << newColor << std::endl;
	gameStateMessage->SetProperty("color", newColor.c_str());
}

void GameGUIEventHandler::setPreGame() const
{
	Rocket::Core::ElementDocument *window0 = _guiManager->getWindow(0);
	Rocket::Core::Element * gameStateMessage = window0->GetElementById("gameStateMessage");

	gameStateMessage->SetClassNames("preGameMessage");
	gameStateMessage->SetInnerRML("Pregame Loading...");
	resetScoreBoard();
}

void GameGUIEventHandler::inProgress() const
{
	Rocket::Core::ElementDocument *window0 = _guiManager->getWindow(0);
	Rocket::Core::Element * gameStateMessage = window0->GetElementById("gameStateMessage");
	gameStateMessage->SetInnerRML("");
	resetScoreBoard();
}

int GameGUIEventHandler::postGame() const
{
	Rocket::Core::ElementDocument *window0 = _guiManager->getWindow(0);
	Rocket::Core::Element * gameStateMessage = window0->GetElementById("gameStateMessage");
	gameStateMessage->SetClassNames("postGameMessage");
	gameStateMessage->SetInnerRML("Postgame");
	return endGame();
	
}

void GameGUIEventHandler::resetScoreBoard() const{
	Rocket::Core::ElementDocument *window2 = _guiManager->getWindow(2);
	Rocket::Core::Element * table = window2->GetChild(0);
	Rocket::Core::Element * score = table->GetFirstChild();

	bool flag = true;
	int maxKills = 0;
	int playerId = 0;
	int location = 0;
	for (int i = 1; i<score->GetNumChildren(); i++){
		Rocket::Core::Element * tr = score->GetChild(i);
		Rocket::Core::String s = tr->GetChild(1)->GetInnerRML();
		for (int j = 0; j < tr->GetNumChildren(); j++){
			Rocket::Core::Element * child = tr->GetChild(j);
			child->SetProperty("color", "rgba(0,0,0,255)");
		}
		//Rocket::Core::String d = Rocket::Core::String(std::to_string(e.getPlayerId()).c_str());

	}

	window2->Hide();

}