#include "Game.h"

#include <iostream>
#include <osg/Group>

#include <Core.h>
#include <GeometryObjectManager.h>
#include <MaterialManager.h>

#include <core/Entity.h>
#include <core/PositionComponent.h>

#include "SceneNodeComponent.h"
#include "../Scene.h"
#include "../input/InputManager.h"
#include "../network/ClientEventHandlerLookup.h"
#include "../network/GameClient.h"

#include <osgAudio/FileStream.h>

Game::Game(ConfigSettings *config)
        : config(config),
          playerFactory(entityManager),
          bombFactory(entityManager),
          eventHandlerLookup(this),
          client(eventHandlerLookup), 
	      _camera(Core::getMainCamera()) {
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

	// For testing in-game editor *
	/*
	std::string str_mediaPath = "";
	std::string str_world_xml = "";
	config->getValue(ConfigSettings::str_mediaFilePath, str_mediaPath);
	config->getValue(ConfigSettings::str_world_xml, str_world_xml);

	str_world_xml = str_mediaPath + str_world_xml;

	Core::loadWorldFile(str_world_xml);
	*/
	/* End testing code */

    inputManager = new InputManager(client);
    inputManager->loadConfig();
	
    Core::addEventHandler(&inputManager->getKeyboardEventHandler());
    Core::addEventHandler(&inputManager->getMouseEventHandler());

	_geometryManager = Core::getWorldRef().getGeometryManager();
	_materialManager = Core::getWorldRef().getMaterialManager();
	source = new Source;
	
	if (sample.valid()) {
		source->stop();
	}
	sample = new Sample("C:\\Users\\melapier\\Downloads\\ins\\share\\osgAudio\\data\\a.wav");
	source->setSound(sample.get());
	source->setGain(1);
	source->setLooping(false);
	//source->play();
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
			// TODO: the following doesn't need to be updated for every event
			// but need to set as soon as the game mode is on
			// TODO: put this two as initial values in the config file
			Core::getMainCamera().setFovYAndUpdate(89);
			Core::getMainCamera().setNearAndFarAndUpdate(1, 500);

			// TODO: Robin: need to check this.
			// Since receive fails when the packet received is zero (from the source code, not sure if it is the intended behavior)
			// and the client will be disconnected from the server 
			// Thus, we want to check if receive fails. If fails, since we are disconnected, should fall back to editor state.
			// E.g: close the server whlie running the game 
            client.receive();
			if (!Core::isInGameMode()) { //have a way to switch back to the editor
				removeAllEntities(); //remove all entity created dynamically when connected to the client
				gsm = DISCONNECT_TO_SERVER;
			}
			break;
		case DISCONNECT_TO_SERVER:
			//TODO: need to remove all the dynamically genereated objects! otherwise we still see them the next time we reconnect
			client.disconnectFromServer();
			gsm = EDITOR_MODE;
			break;
		}
        Core::AdvanceFrame();
    }
}

void Game::addEntity(Entity *entity) {
    auto *posComp = entity->getComponent<PositionComponent>();
    auto *sceneNodeComp = entity->getComponent<SceneNodeComponent>();

    if (posComp == nullptr || sceneNodeComp == nullptr) {
        return;
    }

    if (sceneNodeComp != nullptr) {
        const auto name = std::to_string(entity->getId());
        const auto pos = osg::Vec3(posComp->getX(), posComp->getY(), posComp->getZ());

        getGeometryManager()->addGeometry(name, sceneNodeComp->getNode(), pos);
    }
}

void Game::removeEntity(Entity *entity) {
    getGeometryManager()->deleteGeometry(std::to_string(entity->getId()));
    entityManager.destroyEntity(entity->getId());
}

void Game::removeAllEntities() {
	for (auto it : entityManager.getEntityList()) {
        removeEntity(it);
	}
}

EntityManager &Game::getEntityManager() {
    return entityManager;
}

const PlayerFactory &Game::getPlayerFactory() const {
    return playerFactory;
}

const BombFactory &Game::getBombFactory() const {
    return bombFactory;
}

const GameClient &Game::getGameClient() const {
    return client;
}

GeometryObjectManager * Game::getGeometryManager() {
	return _geometryManager;
}

MaterialManager *Game::getMaterialManager() {
	return _materialManager;
}

Camera &Game::getCamera() {
	return _camera;
}
