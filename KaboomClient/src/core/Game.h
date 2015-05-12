#pragma once

#include <core/EntityManager.h>
#include <util/ConfigSettings.h>

#include <Camera.h>
#include <GeometryObjectManager.h>
#include <MaterialManager.h>

#include "CharacterFactory.h"
#include "BombFactory.h"
#include "../network/ClientEventHandlerLookup.h"
#include "../network/GameClient.h"

#include <osgAudio/Source.h>
#include <osgAudio/AudioEnvironment.h>
#include <osgAudio/Sample.h>
#include "SoundManager.h"
#include "LibRocketGUIManager.h"

using namespace osgAudio;

class InputManager;


class Game {
public:
    Game(ConfigSettings *config);
    ~Game();

    void run();

    void addEntity(Entity *entity);
    void removeEntity(Entity *entity);
    void removeAllEntities();

    EntityManager &getEntityManager();
	SoundManager &getSoundManager();
    const CharacterFactory &getCharacterFactory() const;
    const BombFactory &getBombFactory() const;

    const GameClient &getGameClient() const;

    GeometryObjectManager* getGeometryManager();
    MaterialManager* getMaterialManager();
	osg::ref_ptr<LibRocketGUIManager> getLibRocket();

    Camera &getCamera();
	osg::ref_ptr<Source> source;
	osg::ref_ptr<Source> backgroundMusic;
	osg::ref_ptr<Sample> sample;
	std::unordered_map<SOUNDS, osg::ref_ptr<Sample> > *sounds;

private:


    enum GameStateMachine {
        EDITOR_MODE,
        CONNECT_TO_SERVER,
        GAME_MODE,
        DISCONNECT_TO_SERVER
    };
	class onClickListener : public Rocket::Core::EventListener
	{
	public:
		GameStateMachine *gsm;
		onClickListener(GameStateMachine gsm){
			this->gsm = &gsm;
		}
		void ProcessEvent(Rocket::Core::Event& event)
		{
			*gsm= CONNECT_TO_SERVER;
			std::cout << "marty was clicked" << std::endl;
		}
	};
	GameStateMachine gsm = EDITOR_MODE;
    ConfigSettings *config;
    InputManager *inputManager;
	osg::ref_ptr<LibRocketGUIManager> libRocketInGameManager;

    EntityManager entityManager;
	SoundManager soundManager;
    CharacterFactory characterFactory;
	BombFactory bombFactory;

    ClientEventHandlerLookup eventHandlerLookup;
    GameClient client;

	GeometryObjectManager * _geometryManager;
	MaterialManager * _materialManager;

	Camera &_camera;
};
