#include <iostream>
#include <ConfigSettings.h>
#include <Core.h>
#include "Scene.h"

#include "network/GameClient.h"
#include "input/InputManager.h"

GameClient *g_client;
int main() 
{
	ConfigSettings *config = ConfigSettings::config;
	std::string mediaPath, screenPosXStr, screenPosYStr, renbufferWStr, renbufferHStr, screenWStr, screenHStr;
	config->getValue(ConfigSettings::str_mediaFilePath, mediaPath);
	config->getValue(ConfigSettings::str_screenPosX, screenPosXStr);
	config->getValue(ConfigSettings::str_screenPosY, screenPosYStr);
	config->getValue(ConfigSettings::str_renderBuffer_width, renbufferWStr);
	config->getValue(ConfigSettings::str_renderBuffer_height, renbufferHStr);
	config->getValue(ConfigSettings::str_screen_width, screenWStr);
	config->getValue(ConfigSettings::str_screen_height, screenHStr);

	int posX = atoi(screenPosXStr.c_str());
	int posY = atoi(screenPosYStr.c_str());
	int bufferW = atoi(renbufferWStr.c_str());
	int bufferH = atoi(renbufferHStr.c_str());
	int screenW = atoi(screenWStr.c_str());
	int screenH = atoi(screenHStr.c_str());


	Core::init(posX, posY, screenW, screenH, bufferW, bufferH, mediaPath);

	// hack, refactor after having access to the lab computers
	InputManager *inputManager = new InputManager(NULL);
	inputManager->loadConfig();

	setupScene();
	Core::run();
}

#include <iostream>
#include <ConfigSettings.h>
//
//#include "network/GameClient.h"
//#include "core/Game.h"
//
//GameClient *g_client;
//
//int main() {
//	ConfigSettings* config = ConfigSettings::config;
//	Game game(config);
//
//	g_client = new GameClient(config, game.getEventHandlerLookup());
//
//	try {
//		game.run();
//	}
//	catch (std::exception &e) {
//		std::cerr << "ERROR: " << e.what() << std::endl;
//	}
//
//	delete g_client;
//}
