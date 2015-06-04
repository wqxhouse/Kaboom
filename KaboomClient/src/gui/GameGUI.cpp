#include "stdafx.h"
#include "GameGUI.h"

#include <Core.h>
#include <LibRocketGUIManager.h>
#include <util/ConfigSettings.h>
#include "OnClickListener.h"
#include "OnClickEndGameListener.h"
#include "OnClickAddLetterListener.h"
#include "OnClickRemoveLetterListener.h"
#include "OnClickRespawnListener.h"
#include "OnClickChampSelectListener.h"

void setupGUIDocuments(Game *game)
{
	LibRocketGUIManager *manager = Core::getInGameLibRocketGUIManager();
	std::string mediaPath;
	ConfigSettings::config->getValue("MediaPath", mediaPath);
	
	//load the different windows
	std::string guiPath = mediaPath + "DefaultAssets\\LibRocketGUI\\InGame\\";
	int i = manager->addWindow(guiPath + "window_rocket.rml", true);
	manager->getWindow(i)->Hide();
	i = manager->addWindow(guiPath + "window_start_screen.rml", true);
	manager->getWindow(i)->Hide();
	i = manager->addWindow(guiPath + "window_score.rml", true);
	manager->getWindow(i)->Hide();
	i = manager->addWindow(guiPath + "window_name_screen.rml", true);
	manager->getWindow(i)->Hide();
	i = manager->addWindow(guiPath + "window_start_death.rml", true);
	manager->getWindow(i)->Hide();

	//added listener to start and exit button
	Rocket::Core::ElementDocument* marty = manager->getWindow(1);
	Rocket::Core::EventListenerInstancer;
	OnClickListener *click = new OnClickListener(game);
	click->setMode(NAME_SCREEN);
	Rocket::Core::EventListener * startGameListener = click;
	Rocket::Core::EventListener * endGameListener = new OnClickEndGameListener(game);
	marty->GetElementById("buttons")->GetFirstChild()->GetElementById("startgame")->AddEventListener("click", startGameListener);
	marty->GetElementById("buttons")->GetFirstChild()->GetElementById("exitgame")->AddEventListener("click", endGameListener);


	Rocket::Core::ElementDocument *letters = manager->getWindow(3);
	Rocket::Core::Element *table = letters->GetElementById("enter");
	Rocket::Core::Element *nameArea = letters->GetElementById("name");
	OnClickListener *next = new OnClickListener(game);
	next->setMode(CONNECT_TO_SERVER);
	Rocket::Core::EventListener *something = next;
	table->AddEventListener("click",something);

	//champ select
	Rocket::Core::Element *roboSelect = letters->GetElementById("robo-select");
	table = roboSelect->GetFirstChild();
	 
	for (unsigned int j = 1; j < table->GetNumChildren(); j++){
		Rocket::Core::Element *tr = table->GetChild(j);
		if (j == 2)
			j++;
		for (int k = 0; k <tr->GetNumChildren(); k++){
			Rocket::Core::Element *selection = tr->GetChild(k);
			OnClickChampSelectListener * champ = new OnClickChampSelectListener(game);
			champ->setNumber(game->colorId, j+k-1);
			champ->setNameElement(table);
			Rocket::Core::EventListener *e = champ;
			selection->AddEventListener("click", e);
		}
	}


	//adding a listener to the death button
	Rocket::Core::ElementDocument *deathScreen = manager->getWindow(4);
	Rocket::Core::Element * button = deathScreen->GetFirstChild();
	OnClickRespawnListener* respawn = new OnClickRespawnListener(game);
	respawn->setGame(game);
	Rocket::Core::EventListener * respawnButton = respawn;
	button->AddEventListener("click",respawnButton);
}