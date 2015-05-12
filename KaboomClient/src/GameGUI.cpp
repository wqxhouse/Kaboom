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
	/*int window_id = manager->addWindow(guiPath + "demo.rml", true);*/

	manager->addWindow(guiPath + "marty.rml", true);
	/*int i = _libRocketInGameGUI->addWindow(guiPath + "InGame\\marty.rml", true);
	_libRocketInGameGUI->getWindow(i - 1)->Hide();
	_libRocketInGameGUI->getWindow(i)->Hide();*/

	// get the window
	//Rocket::Core::ElementDocument *window1 = manager->getWindow(window_id);
	//Rocket::Core::Element * win = window1->GetFirstChild();
	//Rocket::Core::Element * newElement = window1->CreateElement("div");
	//if (newElement == NULL)
	//{
	//	return;
	//}
	//Rocket::Core::ElementText * new_text_element = window1->CreateTextNode("Sample Marty");
	//if (new_text_element == NULL)
	//{
	//	newElement->RemoveReference();
	//	return;
	//}
	////AppendChild->newElement(new_text_element);
	////win->AppendChild(newElement);
	//std::cout << "after: " << win->GetNumChildren() << std::endl;
}