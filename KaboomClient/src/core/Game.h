#pragma once

#include <core/EntityManager.h>
#include <util/ConfigSettings.h>

#include <Camera.h>
#include <GeometryObjectManager.h>
#include <MaterialManager.h>
#include <ParticleEffectManager.h>

#include "CharacterFactory.h"
#include "BombFactory.h"
#include "JumpPadFactory.h"
#include "../network/ClientEventHandlerLookup.h"
#include "../network/GameClient.h"
#include "../gui/GameGUIEventHandler.h"
#include "../gui/GameGUIListener.h"

#include <osgAudio/Source.h>
#include <osgAudio/AudioEnvironment.h>
#include <osgAudio/Sample.h>
#include "SoundManager.h"
#include "LibRocketGUIManager.h"
#include "GameStateMachine.h"

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
	const JumpPadFactory &getJumpPadFactory() const;

    const GameClient &getGameClient() const;

    GeometryObjectManager *getGeometryManager();
    MaterialManager *getMaterialManager();
	ParticleEffectManager *getParticleEffectManager();

	const GameGUIEventHandler *getGameGUIEventHandler() const;
	inline const GameStateMachine getCurrentGameState() const
	{
		return gsm;
	}

    Camera &getCamera();
	osg::ref_ptr<Source> source;
	osg::ref_ptr<Source> backgroundMusic;
	osg::ref_ptr<Sample> sample;
	std::unordered_map<SOUNDS, osg::ref_ptr<Sample> > *sounds;

private:
	friend void GameGUIListener::setGameState(GameStateMachine state);
	
	GameStateMachine gsm = EDITOR_MODE;
    ConfigSettings *config;
    InputManager *inputManager;

    EntityManager entityManager;
	SoundManager soundManager;
    CharacterFactory characterFactory;
	BombFactory bombFactory;
	JumpPadFactory jumpPadFactory;

    ClientEventHandlerLookup eventHandlerLookup;
    GameClient client;

	GeometryObjectManager *_geometryManager;
	MaterialManager *_materialManager;
	ParticleEffectManager *_particleEffectManager;

	Camera &_camera;
	GameGUIEventHandler *_guiEventHandler;
};
