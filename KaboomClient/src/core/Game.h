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
    const CharacterFactory &getCharacterFactory() const;
    const BombFactory &getBombFactory() const;

    const GameClient &getGameClient() const;

    GeometryObjectManager* getGeometryManager();
    MaterialManager* getMaterialManager();

    Camera &getCamera();

private:
    enum GameStateMachine {
        EDITOR_MODE,
        CONNECT_TO_SERVER,
        GAME_MODE,
        DISCONNECT_TO_SERVER
    };

    ConfigSettings *config;
    InputManager *inputManager;

    EntityManager entityManager;
    CharacterFactory characterFactory;
	BombFactory bombFactory;

    ClientEventHandlerLookup eventHandlerLookup;
    GameClient client;

	GeometryObjectManager * _geometryManager;
	MaterialManager * _materialManager;

	Camera &_camera;
};
