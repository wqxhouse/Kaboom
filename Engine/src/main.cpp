#include "stdafx.h" 
#include "Core.h"
#include "Scene.h"
#include "ConfigSettings.h"

int main()
{
	ConfigSettings* config = ConfigSettings::config;
	string str_world_xml = "";
	config->getValue(ConfigSettings::str_world_xml, str_world_xml);

	Core::init(800, 600, 800, 600);
	Core::loadWorldFile(str_world_xml);
	//setupScene();

	Core::run();

	return 0;
}
