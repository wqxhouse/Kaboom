#include "stdafx.h"
#include "GameGUI.h"

#include <Core.h>
#include <LibRocketGUIManager.h>
#include <util/ConfigSettings.h>
#include "OnClickListener.h"

void setupGUIDocuments(Game *game)
{
	LibRocketGUIManager *manager = Core::getInGameLibRocketGUIManager();
	std::string mediaPath;
	ConfigSettings::config->getValue("MediaPath", mediaPath);

	//std::string guiPath = mediaPath + "DefaultAssets\\LibRocketGUI\\InGame\\";
	//manager->addWindow(guiPath + "window_rocket.rml", true);
	//int i=manager->addWindow(guiPath + "window_start_screen.rml", true);
	//manager->getWindow(i)->Hide();

	//Rocket::Core::ElementDocument* marty = manager->getWindow(1);
	//Rocket::Core::EventListenerInstancer;
	//Rocket::Core::EventListener * startGameListener = new OnClickListener(game);
	//marty->GetFirstChild()->GetElementById("startgame")->AddEventListener("click", startGameListener);
}