#pragma once

#include <core/EntityManager.h>
#include <core/Player.h>
#include <util/ConfigSettings.h>

#include <Camera.h>
#include <GeometryObjectManager.h>
#include <MaterialManager.h>
#include <ParticleEffectManager.h>

#include "CharacterFactory.h"
#include "BombFactory.h"
#include "JumpPadFactory.h"
#include "PickupFactory.h"

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
    typedef std::unordered_map<unsigned int, Player *> IdToPlayerMap;

    Game(ConfigSettings *config);
    ~Game();

    void run();

    inline void addPlayer(Player *player) {
        players[player->getId()] = player;
    }

    inline void removePlayer(Player *player) {
        players.erase(players.find(player->getId()));

        delete player;
    }

    void addEntity(Entity *entity);
    void removeEntity(Entity *entity);
    void removeAllEntities();

    EntityManager &getEntityManager();
	SoundManager &getSoundManager();
    const CharacterFactory &getCharacterFactory() const;
    const BombFactory &getBombFactory() const;
	const JumpPadFactory &getJumpPadFactory() const;
	const PickupFactory &getPickupFactory() const;

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

    inline Player *getCurrentPlayer() const {
        return currentPlayer;
    }

    inline void setCurrentPlayer(Player *currentPlayer) {
        this->currentPlayer = currentPlayer;
    }

    inline const IdToPlayerMap &getPlayers() const {
        return players;
    }

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
	PickupFactory pickupFactory;

    ClientEventHandlerLookup eventHandlerLookup;
    GameClient client;

    Player *currentPlayer;
    IdToPlayerMap players;

	GeometryObjectManager *_geometryManager;
	MaterialManager *_materialManager;
	ParticleEffectManager *_particleEffectManager;

	Camera &_camera;
	GameGUIEventHandler *_guiEventHandler;
};
