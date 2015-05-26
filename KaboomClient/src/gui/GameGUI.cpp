#include "stdafx.h"
#include "GameGUI.h"

#include <Core.h>
#include <LibRocketGUIManager.h>
#include <util/ConfigSettings.h>
#include "OnClickListener.h"
#include "OnClickEndGameListener.h"
#include "OnClickAddLetterListener.h"
#include "OnClickRemoveLetterListener.h"

void setupGUIDocuments(Game *game)
{
	LibRocketGUIManager *manager = Core::getInGameLibRocketGUIManager();
	std::string mediaPath;
	ConfigSettings::config->getValue("MediaPath", mediaPath);

	std::string guiPath = mediaPath + "DefaultAssets\\LibRocketGUI\\InGame\\";
	int i = manager->addWindow(guiPath + "window_rocket.rml", true);
	manager->getWindow(i)->Hide();
	i = manager->addWindow(guiPath + "window_start_screen.rml", true);
	manager->getWindow(i)->Hide();
	i = manager->addWindow(guiPath + "window_score.rml", true);
	manager->getWindow(i)->Hide();
	i = manager->addWindow(guiPath + "window_name_screen.rml", true);
	manager->getWindow(i)->Hide();

	Rocket::Core::ElementDocument* marty = manager->getWindow(1);
	Rocket::Core::EventListenerInstancer;
	OnClickListener *click = new OnClickListener(game);
	click->setMode(CONNECT_TO_SERVER);
	Rocket::Core::EventListener * startGameListener = click;
	Rocket::Core::EventListener * endGameListener = new OnClickEndGameListener(game);
	marty->GetFirstChild()->GetElementById("startgame")->AddEventListener("click", startGameListener);
	marty->GetFirstChild()->GetElementById("exitgame")->AddEventListener("click", endGameListener);
	std::string alpha[] = {"qwertyuiop","asdfghjkl","zxcvbnm"};
	Rocket::Core::ElementDocument *letters = manager->getWindow(3);
	Rocket::Core::Element *table = letters->GetFirstChild();
	Rocket::Core::Element *nameArea = letters->GetChild(1);
	for (int j = 0; j < 3; j++){
		Rocket::Core::Element *tr = table->GetChild(j);
		for (int k = 0; k < alpha[j].size(); k++){
			Rocket::Core::Element *col = tr->GetChild(k);
			Rocket::Core::EventListener * letterListener;
			OnClickAddLetterListener *o= new OnClickAddLetterListener(game);
			char a = alpha[j].at(k);
			o->setLetter(a);
			o->setName(game->name);
			o->setNameElement(nameArea);
			letterListener = o;
			col->AddEventListener("click",letterListener);
		}
	}
	OnClickRemoveLetterListener *rem = new OnClickRemoveLetterListener(game);
	rem->setName(game->name);
	rem->setNameElement(nameArea);
	Rocket::Core::EventListener * del=rem;
	table->GetChild(2)->GetLastChild()->AddEventListener("click",del);
	OnClickListener *next = new OnClickListener(game);
	next->setMode(START_SCREEN_MODE);
	Rocket::Core::EventListener *something = next;
	table->GetChild(1)->GetLastChild()->AddEventListener("click",something);
}