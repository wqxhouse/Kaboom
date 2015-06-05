#include "Game.h"

#include <iostream>

#include <osg/Group>
#include <osgAudio/SoundRoot.h>

#include <Core.h>
#include <GeometryObjectManager.h>
#include <MaterialManager.h>
#include <ObjectGlowManager.h>
#include <components/PositionComponent.h>
#include <components/WeaponPickupComponent.h>
#include <core/Entity.h>

#include "../Scene.h"
#include "../gui/GameGUI.h"
#include "../components/SceneNodeComponent.h"
#include "../input/InputManager.h"
#include "../network/ClientEventHandlerLookup.h"
#include "../network/GameClient.h"
#include <osgAudio/AudioEnvironment.h>
#include <osgAudio/Sample.h>
using namespace osgAudio;

Game::Game(ConfigSettings *config)
        : config(config),
          characterFactory(entityManager),
          bombFactory(entityManager),
		  jumpPadFactory(entityManager),
		  pickupFactory(entityManager),
          eventHandlerLookup(this),
          client(eventHandlerLookup), 
	      _camera(Core::getMainCamera()) {
	name = new std::string();
	osgAudio::AudioEnvironment::instance()->init();
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
	std::string str_typeid_xml = "";

	config->getValue(ConfigSettings::str_mediaFilePath, str_mediaPath);
	config->getValue(ConfigSettings::str_material_xml, str_material_xml);
	config->getValue(ConfigSettings::str_world_xml, str_world_xml);
	config->getValue(ConfigSettings::str_typeid_xml, str_typeid_xml);

	str_world_xml = str_mediaPath + str_world_xml;
	str_material_xml = str_mediaPath + str_material_xml;

	Core::loadMaterialFile(str_material_xml);
	Core::loadWorldFile(str_world_xml);
	Core::loadTypeIdFile(str_typeid_xml);
	Core::loadModelCache(4);
	/* End testing code */

    inputManager = new InputManager(client, this);
    inputManager->loadConfig();

    Core::addEventHandler(&inputManager->getKeyboardEventHandler());
    Core::addEventHandler(&inputManager->getMouseEventHandler());

	_geometryManager = Core::getWorldRef().getGeometryManager();
	_materialManager = Core::getWorldRef().getMaterialManager();
	_particleEffectManager = Core::getWorldRef().getParticleEffectManager();

    printf("Loading KABOOM_EXPLODE sound\n");
	soundManager.loadSound(SoundType::KABOOM_EXPLODE, str_mediaPath + "DefaultAssets\\Sound\\bomb.wav");
	soundManager.loadSound(SoundType::REMOTE_EXPLODE, str_mediaPath + "DefaultAssets\\Sound\\c4.wav");
	soundManager.loadSound(SoundType::TIME_EXPLODE, str_mediaPath + "DefaultAssets\\Sound\\time_explosion.mp3");
    printf("Loading KABOOM_FIRE sound\n");
	soundManager.loadSound(SoundType::KABOOM_FIRE, str_mediaPath + "DefaultAssets\\Sound\\throw.wav");
	soundManager.loadSound(SoundType::REMOTE_FIRE, str_mediaPath + "DefaultAssets\\Sound\\throw2.wav");
	soundManager.loadSound(SoundType::TIME_FIRE, str_mediaPath + "DefaultAssets\\Sound\\bounce_fire.wav");
	printf("Loading WALKING sound\n");
	soundManager.loadSound(SoundType::WALKING, str_mediaPath + "DefaultAssets\\Sound\\walking.mp3");
	osg::ref_ptr<Sample> walk = new Sample(str_mediaPath + "DefaultAssets\\Sound\\walking.mp3");
	characterFactory.setWalkingSample(walk);
    printf("Loading BASIC sound\n");
	soundManager.loadSound(SoundType::BASIC, str_mediaPath + "DefaultAssets\\Sound\\a.wav"); 
	printf("Loading JUMP sound\n");
	soundManager.loadSound(SoundType::JUMP, str_mediaPath + "DefaultAssets\\Sound\\jump_sound.mp3");
	printf("Loading Background Music sound\n");
	for (int i = 0; i < 1; i++){
		std::unordered_map<VoiceActing, osg::ref_ptr<Sample>> *voice = new std::unordered_map<VoiceActing, osg::ref_ptr<Sample>>();
		addVoiceLines(str_mediaPath,i,voice);
		voiceActorList[i]=voice;
	}
	voiceMap = voiceActorList[0];
	backGroundMusic = new Source;
	angryRobot = new Sample(str_mediaPath + "DefaultAssets\\Sound\\angryRobot.mp3");
	backGroundMusic->setSound(angryRobot);
	backGroundMusic->setGain(1);
	backGroundMusic->setLooping(true);
	backGroundMusic->play();
	voiceSource = new Source;
	angry = true;

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
	Rocket::Core::ElementDocument* score_screen_ui = guiManager->getWindow(2);
	Rocket::Core::ElementDocument* name_screen_ui = guiManager->getWindow(3);
	Rocket::Core::ElementDocument* death_screen_ui = guiManager->getWindow(4);
    //while (!Core::isViewerClosed()) { // TODO: buggy right now
    while (true) {
		// printf("duration: %lf\n", Core::getLastFrameDuration());
		switch (gsm) {
		case EDITOR_MODE:
			
			if (!angry){
				backGroundMusic->setSound(angryRobot);
				backGroundMusic->setGain(1);
				backGroundMusic->setLooping();
				backGroundMusic->play();
				angry = true;
				
			}
			if (Core::isInStartScreenMode()) { //pressed the PlayGame Button
				gsm = START_SCREEN_MODE;
			}
			break;
		case START_SCREEN_MODE:
		{
			if (!angry){
				backGroundMusic->setSound(angryRobot);
				backGroundMusic->setGain(1);
				backGroundMusic->setLooping();
				backGroundMusic->play();
				angry = true;			
			}
			in_game_screen_ui->Hide();
			name_screen_ui->Hide();
			score_screen_ui->Hide();
			start_screen_ui->Show();
			death_screen_ui->Hide();
			abc = true;
			break;
		}
		case EXIT_START_SCREEN_MODE:
		{
			gsm = EDITOR_MODE;
			Core::disableStartScreen();
			break;
		}
		case NAME_SCREEN:
			//std::cout << colorId << std::endl;
			if (!angry){
				backGroundMusic->setSound(angryRobot);
				backGroundMusic->setGain(1);
				backGroundMusic->setLooping();
				backGroundMusic->play();
				angry = true;
			}
			if (colorId != previousValue){
				previousValue = colorId;
				//Change 0 to colorId once all voices are recorded
				voiceMap = voiceActorList[0];
				voiceSource->setSound(voiceActorList[0]->at(CHAMP_SELECT));
				voiceSource->setGain(1);
				voiceSource->play();
			}
			in_game_screen_ui->Hide();
			start_screen_ui->Hide();
			name_screen_ui->Show();
			inputManager->loadNameTyping();
			break;
		case CONNECT_TO_SERVER:
		{
			if (colorId==5){
				colorId = 0;
			}
			inputManager->loadConfig();
			config->getValue(ConfigSettings::str_server_address, serverAddress);
			config->getValue(ConfigSettings::str_server_port, serverPort);

			in_game_screen_ui->Show();
			start_screen_ui->Hide();
			name_screen_ui->Hide();
			death_screen_ui->Hide();
			playerAlive = true;
            bool res = client.connectToServer(serverAddress, serverPort);

			if (res)
			{
				gsm = GAME_MODE;
				Core::enableGameMode();
				client.sendPlayerRenameEvent(*name);
                client.sendPlayerSelectionEvent(GREEN_CHARACTER);
			//abc = false;
				
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
			if (angry&&!backGroundMusic->isPaused()){
				backGroundMusic->pause();
				angry = false;
			}
			Core::getMainCamera().setFovXAndUpdate(90);
			Core::getMainCamera().setNearAndFarAndUpdate(0.01f, 500);
			
			// TODO: Robin: need to check this.
			// Since receive fails when the packet received is zero (from the source code, not sure if it is the intended behavior)
			// and the client will be disconnected from the server 
			// Thus, we want to check if receive fails. If fails, since we are disconnected, should fall back to editor state.
			// E.g: close the server whlie running the game 
            client.receive();
			_guiEventHandler->changeTime(this);
			damageScreenCheck();
			
			/*if (currentPlayer->getEntity() != nullptr){
				PlayerStatusComponent *player = currentPlayer->getEntity()->getComponent<PlayerStatusComponent>();
				if (!currentPlayer->getEntity()->getComponent<PlayerStatusComponent>()->isAlive()){
					_guiEventHandler->changeDeathTime(this);
				}
			}*/
			if (!playerAlive){
				deathTimeUpdate();
			}
			if (gameMode.getMatchState() == GameMode::MatchState::PRE_MATCH){
				_guiEventHandler->preGame(color, changeColor);
			}
			if (!Core::isInGameMode()) { //have a way to switch back to the editor
				removeAllEntities(); //remove all entity created dynamically when connected to the client
				gsm = DISCONNECT_TO_SERVER;
			}
			break;
		case DISCONNECT_TO_SERVER:
			//TODO: need to remove all the dynamically genereated objects! otherwise we still see them the next time we reconnect
			getGameGUIEventHandler()->deleteAllPlayers();
			client.disconnectFromServer();
			gsm = START_SCREEN_MODE;
			break;
		}		
        Core::AdvanceFrame();
    }
}

void Game::removeEntity(Entity *entity) {
    if (entity->hasComponent<WeaponPickupComponent>()) {
        auto obj = getGeometryManager()->getGeometryObject(std::to_string(entity->getId()));
        Core::getWorldRef().getObjectGlowManager()->removeGlowGeometryObject(obj);
    }
    getGeometryManager()->deleteGeometry(std::to_string(entity->getId()));
    entityManager.destroyEntity(entity->getId());
}

void Game::removeAllEntities() {
	for (auto it : entityManager.getEntityList()) {
        removeEntity(it);
	}
}
void Game::addVoiceLines(std::string str_mediaPath, int i, std::unordered_map<Game::VoiceActing, osg::ref_ptr<Sample>> *voice){
	std::string folder = str_mediaPath + "DefaultAssets\\Sound\\actor" + std::to_string(i) + "\\";
	//osg::ref_ptr<Sample> sample = new Sample("champ_select.wav");
	voice->insert(std::make_pair	<VoiceActing, osg::ref_ptr<Sample>>(CHAMP_SELECT, new Sample(folder+"champ_select.wav")));
	voice->insert(std::make_pair	<VoiceActing, osg::ref_ptr<Sample>>(DEATH_1, new Sample(folder + "death_1.wav")));
	voice->insert(std::make_pair	<VoiceActing, osg::ref_ptr<Sample>>(KILL_1, new Sample(folder + "kill_1.wav")));
	voice->insert(std::make_pair	<VoiceActing, osg::ref_ptr<Sample>>(END_GAME_VICTORY_1, new Sample(folder + "victory_1.wav")));
}

void Game::damageScreenCheck()
{
	std::chrono::duration<double> elapsed_seconds = (std::chrono::high_resolution_clock::now() - damageTime);
	if (healthChanged && elapsed_seconds.count() >= 1&&playerAlive)
	{
		healthChanged = false;
		_guiEventHandler->damageScreen(false);
	}
}

void Game::deathTimeUpdate(){
	_guiEventHandler->changeDeathTime(this);
}