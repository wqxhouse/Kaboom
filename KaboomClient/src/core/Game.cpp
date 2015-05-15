#include "Game.h"

#include <iostream>

#include <osg/Group>
#include <osgAudio/FileStream.h>

#include <Core.h>
#include <GeometryObjectManager.h>
#include <MaterialManager.h>
#include <components/PositionComponent.h>
#include <core/Entity.h>

#include "../Scene.h"
#include "../gui/GameGUI.h"
#include "../components/SceneNodeComponent.h"
#include "../input/InputManager.h"
#include "../network/ClientEventHandlerLookup.h"
#include "../network/GameClient.h"

Game::Game(ConfigSettings *config)
        : config(config),
          characterFactory(entityManager),
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
	sounds = new std::unordered_map<SOUNDS, osg::ref_ptr<Sample> >();

    Core::init(posX, posY, screenW, screenH, bufferW, bufferH, mediaPath);
	// init gui handler after Core::init() is called, so that it gets the intialized in game gui 
	_guiEventHandler = new GameGUIEventHandler(this);

	setupScene();
	setupGUIDocuments(this);
	
	/* For testing in-game editor */
	
	std::string str_mediaPath = "";
	std::string str_material_xml = "";
	std::string str_world_xml = "";

	config->getValue(ConfigSettings::str_mediaFilePath, str_mediaPath);
	config->getValue(ConfigSettings::str_material_xml, str_material_xml);
	config->getValue(ConfigSettings::str_world_xml, str_world_xml);

	str_world_xml = str_mediaPath + str_world_xml;
	str_material_xml = str_mediaPath + str_material_xml;

	Core::loadMaterialFile(str_material_xml);
	Core::loadWorldFile(str_world_xml);
	/* End testing code */

    inputManager = new InputManager(client);
    inputManager->loadConfig();
	
    Core::addEventHandler(&inputManager->getKeyboardEventHandler());
    Core::addEventHandler(&inputManager->getMouseEventHandler());

	_geometryManager = Core::getWorldRef().getGeometryManager();
	_materialManager = Core::getWorldRef().getMaterialManager();
	source = new Source;
	
	printf("check for sound errors\n");
	sample = new Sample("sounds\\a.wav");
	soundManager.addToMap(KABOOM_EXPLODE,"sounds\\a.wav");
	source->setSound(sample.get());
	source->setGain(1);
	source->setLooping(false);
	printf("Adding KABOOM_EXPLODE TO MAP\n");
	sounds->insert(std::make_pair(KABOOM_EXPLODE,sample));
	//sounds->at(KABOOM_EXPLODE)=sample;
	printf("Added KABOOM_EXPLODE TO MAP\n");
	sample = new Sample("sounds\\a.wav");
	printf("Adding KABOOM_FIRE TO MAP\n");
	sounds->insert(std::make_pair(KABOOM_FIRE, sample));
	printf("Added KABOOM_FIRE TO MAP\n");
	sample = new Sample("sounds\\a.wav");
	printf("Adding BASIC TO MAP\n");
	sounds->insert(std::make_pair(BASIC, sample));
	printf("Added BASIC TO MAP\n");
	/*sample = new Sample("C:\\Users\\melapier\\Downloads\\djsona.wav");
	backgroundMusic = new Source;
	backgroundMusic->setSound(sample.get());
	backgroundMusic->setGain(1);
	backgroundMusic->setLooping(true);
	backgroundMusic->play();*/
	//delete source;
	printf("finished check sound errors");
}

Game::~Game() {
    delete inputManager;
	delete _guiEventHandler;
}

void Game::run() {
    //static bool connected = false;
	
	 //start with EDITOR_MODE

	std::string serverAddress;
	int serverPort;

	//Grab the doucment from libRocketInGameMangaer
	LibRocketGUIManager *guiManager = Core::getInGameLibRocketGUIManager();
	Rocket::Core::ElementDocument* in_game_screen_ui = guiManager->getWindow(0);
	Rocket::Core::ElementDocument* start_screen_ui = guiManager->getWindow(1);

    //while (!Core::isViewerClosed()) { // TODO: buggy right now
    while (true) {
		// printf("duration: %lf\n", Core::getLastFrameDuration());
		switch (gsm) {
		case EDITOR_MODE:
			if (Core::isInGameMode()) { //pressed the PlayGame Button
				gsm = CONNECT_TO_SERVER;
				//gsm = START_SCREEN_MODE;
			}
			break;
		case START_SCREEN_MODE:
		{
			in_game_screen_ui->Hide();
			start_screen_ui->Show();
			break;
		}
		case CONNECT_TO_SERVER:
		{
			config->getValue(ConfigSettings::str_server_address, serverAddress);
			config->getValue(ConfigSettings::str_server_port, serverPort);
			in_game_screen_ui->Show();
			start_screen_ui->Hide();

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
			
			Core::getMainCamera().setFovXAndUpdate(90);
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
        const Vec3 &pos = posComp->getPosition();

        const auto name = std::to_string(entity->getId());
        const auto osgPos = osg::Vec3(pos.x, pos.y, pos.z);

        getGeometryManager()->addGeometry(name, sceneNodeComp->getNode(), osgPos);
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
SoundManager &Game::getSoundManager() {
	return soundManager;
}

const CharacterFactory &Game::getCharacterFactory() const {
    return characterFactory;
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

const GameGUIEventHandler *Game::getGameGUIEventHandler() const
{
	return _guiEventHandler;
}