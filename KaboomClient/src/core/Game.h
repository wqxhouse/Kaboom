#pragma once

#include <Camera.h>
#include <GeometryObjectManager.h>
#include <MaterialManager.h>
#include <ParticleEffectManager.h>
#include <core/DeathmatchMode.h>
#include <core/EntityManager.h>
#include <core/Player.h>
#include <util/ConfigSettings.h>

#include "CharacterFactory.h"
#include "BombFactory.h"
#include "JumpPadFactory.h"
#include "PickupFactory.h"
#include "../network/ClientEventHandlerLookup.h"
#include "../network/GameClient.h"
#include "../gui/GameGUIEventHandler.h"
#include "../gui/GameGUIListener.h"
#include "../sound/SoundManager.h"
#include "LibRocketGUIManager.h"
#include "GameStateMachine.h"

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

    inline SoundManager &getSoundManager() {
        return soundManager;
    }

    inline EntityManager &getEntityManager() {
        return entityManager;
    }

    inline const CharacterFactory &getCharacterFactory() const {
        return characterFactory;
    }

    inline const BombFactory &getBombFactory() const {
        return bombFactory;
    }

    inline const JumpPadFactory &getJumpPadFactory() const {
        return jumpPadFactory;
    }

    inline const PickupFactory &getPickupFactory() const {
        return pickupFactory;
    }

    inline const GameClient &getGameClient() const {
        return client;
    }

    inline GeometryObjectManager *getGeometryManager() {
        return _geometryManager;
    }

    inline MaterialManager *getMaterialManager() {
        return _materialManager;
    }

    inline ParticleEffectManager *getParticleEffectManager() {
        return _particleEffectManager;
    }

    inline const GameGUIEventHandler *getGameGUIEventHandler() const {
        return _guiEventHandler;
    }

    inline const GameStateMachine getCurrentGameState() const {
        return gsm;
    }

    inline Camera &getCamera() {
        return _camera;
    }

    inline Player *getCurrentPlayer() const {
        return currentPlayer;
    }

    inline void setCurrentPlayer(Player *currentPlayer) {
        this->currentPlayer = currentPlayer;
    }

    inline const IdToPlayerMap &getPlayers() const {
        return players;
    }

    inline DeathmatchMode &getGameMode() {
        return gameMode;
    }

	std::string *name;

private:
    friend void GameGUIListener::setGameState(GameStateMachine state);

    GameStateMachine gsm = EDITOR_MODE;
    ConfigSettings *config;
    InputManager *inputManager;
    SoundManager soundManager;

    EntityManager entityManager;
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

	bool abc;

    DeathmatchMode gameMode;
};