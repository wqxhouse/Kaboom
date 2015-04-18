#include "stdafx.h" 
#include "Core.h"
#include "Scene.h"
#include "ConfigSettings.h"

int main()
{
	ConfigSettings* config = ConfigSettings::config;
	string str_world_xml = "";
	config->getValue(ConfigSettings::str_world_xml, str_world_xml);

	Core::init(160, 80,		// winPosX, winPosY
			   1280, 720,		// winSizeX, winSizeY
			   1280, 720);		// renderBufferX, renderBufferY

	// Core::loadWorldFile(str_world_xml);
	setupScene();
	Core::run();

	return 0;
}
