#include "Game.h"

#include <iostream>
#include <osg/Group>

#include <Core.h>
#include <core/Entity.h>
#include <core/PositionComponent.h>

#include "../Global.h"
#include "SceneNodeComponent.h"
#include "../Scene.h"
#include "../input/InputManager.h"
#include "../network/ClientEventHandlerLookup.h"
#include "../network/GameClient.h"

Game::Game(ConfigSettings *config)
    : config(config),
      playerFactory(entityManager),
      bombFactory(entityManager),
      eventHandlerLookup(this),
      client(eventHandlerLookup) {

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
    setupScene();

    inputManager = new InputManager(client);
    inputManager->loadConfig();

    Core::addEventHandler(&inputManager->getKeyboardEventHandler());
    Core::addEventHandler(&inputManager->getMouseEventHandler());
}

Game::~Game() {
    delete inputManager;
}

void Game::run() {
    //static bool connected = false;
	
	GameStateMachine gsm = EDITOR_MODE; //start with EDITOR_MODE

	std::string serverAddress;
	int serverPort;

    //while (!Core::isViewerClosed()) { // TODO: buggy right now
    while (true) {
		// printf("duration: %lf\n", Core::getLastFrameDuration());
		switch (gsm) {
		case EDITOR_MODE:
			if (Core::isInGameMode()) { //pressed the PlayGame Button
				gsm = CONNECT_TO_SERVER;
			}
			break;
		case CONNECT_TO_SERVER:
		{
			config->getValue(ConfigSettings::str_server_address, serverAddress);
			config->getValue(ConfigSettings::str_server_port, serverPort);

            bool res = client.connectToServer(serverAddress, serverPort);
			if (res)
			{
				gsm = GAME_MODE;
			}
			else
			{
				gsm = EDITOR_MODE;
				Core::disableGameMode();
			}
			break;
		}
		case GAME_MODE:
			// TODO: Robin: need to check this.
			// Since receive fails when the packet received is zero (from the source code, not sure if it is the intended behavior)
			// and the client will be disconnected from the server 
			// Thus, we want to check if receive fails. If fails, since we are disconnected, should fall back to editor state.
			// E.g: close the server whlie running the game 
            client.receive();
			if (!Core::isInGameMode()) { //have a way to switch back to the editor
				gsm = DISCONNECT_TO_SERVER;
			}
			break;
		case DISCONNECT_TO_SERVER:
			client.disconnectFromServer();
			gsm = EDITOR_MODE;
			break;
		}
        Core::AdvanceFrame();
    }
}

bool Game::addSceneNodeEntity(Entity *entity) {
    SceneNodeComponent *sceneNodeCom = entity->getComponent<SceneNodeComponent>();
    if (sceneNodeCom == nullptr) {
       return false;
    }

	PositionComponent *posCom = entity->getComponent<PositionComponent>();
	if (posCom == nullptr) {
		return false;
	}
	
	Core::getWorldRef().getGeometryManager()->addGeometry(std::to_string(static_cast<int>(entity->getId())),
														  sceneNodeCom->getNode(), 
														  osg::Vec3(posCom->getX(), posCom->getY(), posCom->getZ()));

    //rootNode->addChild(sceneNodeCom->getNode());




    return true;
}

const EntityManager &Game::getEntityManager() const {
    return entityManager;
}

const PlayerFactory &Game::getPlayerFactory() const {
    return playerFactory;
}

const BombFactory &Game::getBombFactory() const {
    return bombFactory;
}
