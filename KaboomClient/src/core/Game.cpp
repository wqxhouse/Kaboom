#include "Game.h"

#include <iostream>

#include <osg/Group>
#include <osgAudio/SoundRoot.h>

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
		  jumpPadFactory(entityManager),
		  pickupFactory(entityManager),
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

    osg::ref_ptr<osgAudio::SoundRoot> soundRoot = new osgAudio::SoundRoot;
    Core::init(posX, posY, screenW, screenH, bufferW, bufferH, mediaPath, soundRoot);
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

    inputManager = new InputManager(client, this);
    inputManager->loadConfig();
	
	//loading the bomb asset.
	bombFactory.loadBomb(str_mediaPath);
	pickupFactory.loadPickup(str_mediaPath);

    Core::addEventHandler(&inputManager->getKeyboardEventHandler());
    Core::addEventHandler(&inputManager->getMouseEventHandler());

	_geometryManager = Core::getWorldRef().getGeometryManager();
	_materialManager = Core::getWorldRef().getMaterialManager();
	_particleEffectManager = Core::getWorldRef().getParticleEffectManager();

    printf("Loading KABOOM_EXPLODE sound\n");
    soundManager.loadSound(SoundType::KABOOM_EXPLODE, "sounds\\a.wav");
    printf("Loading KABOOM_FIRE sound\n");
    soundManager.loadSound(SoundType::KABOOM_FIRE, "sounds\\a.wav");
    printf("Loading BASIC sound\n");
    soundManager.loadSound(SoundType::BASIC, "sounds\\a.wav");
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
			if (Core::isInStartScreenMode()) { //pressed the PlayGame Button
				gsm = START_SCREEN_MODE;
			}
			break;
		case START_SCREEN_MODE:
		{
			in_game_screen_ui->Hide();
			start_screen_ui->Show();
			break;
		}
		case EXIT_START_SCREEN_MODE:
		{
			gsm = EDITOR_MODE;
			Core::disableStartScreen();
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
				Core::enableGameMode();
			}
			else
			{
				gsm = START_SCREEN_MODE;
				//Core::disableGameMode();
			}
			break;
		}
		case GAME_MODE:
			// TODO: the following doesn't need to be updated for every event
			// but need to set as soon as the game mode is on
			// TODO: put this two as initial values in the config file
			
			Core::getMainCamera().setFovXAndUpdate(90);
			Core::getMainCamera().setNearAndFarAndUpdate(0.1, 500);

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
			gsm = START_SCREEN_MODE;
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
