#include "stdafx.h"
#include "GameGUI.h"

#include <Core.h>
#include <LibRocketGUIManager.h>
#include <util/ConfigSettings.h>

void setupGUI()
{
	LibRocketGUIManager *manager = Core::getInGameLibRocketGUIManager();
	std::string mediaPath;
	ConfigSettings::config->getValue("MediaPath", mediaPath);

	std::string guiPath = mediaPath + "DefaultAssets\\LibRocketGUI\\InGame\\";
	int window_id = manager->addWindow(guiPath + "demo.rml", true);

	// get the window
	Rocket::Core::ElementDocument *window1 = manager->getWindow(window_id);
	Rocket::Core::Element * win = window1->GetFirstChild();
	Rocket::Core::Element * newElement = window1->CreateElement("div");
	if (newElement == NULL)
	{
		return;
	}
	Rocket::Core::ElementText * new_text_element = window1->CreateTextNode("Sample Marty");
	if (new_text_element == NULL)
	{
		newElement->RemoveReference();
		return;
	}
	newElement->AppendChild(new_text_element);
	win->AppendChild(newElement);
	std::cout << "after: " << win->GetNumChildren() << std::endl;
}