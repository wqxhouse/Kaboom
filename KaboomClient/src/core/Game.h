#pragma once

#include <ConfigSettings.h>
#include <core/EntityManager.h>
#include <Camera.h>
#include <GeometryObjectManager.h>
#include <MaterialManager.h>

#include "BombFactory.h"
#include "PlayerFactory.h"
#include "../network/ClientEventHandlerLookup.h"
#include "../network/GameClient.h"

class InputManager;

enum GameStateMachine {
	EDITOR_MODE,
	CONNECT_TO_SERVER,
	GAME_MODE,
	DISCONNECT_TO_SERVER
};

class Game {
public:
    Game(ConfigSettings *config);
    ~Game();

    void run();

	const GameClient &getGameClient() const;
    //bool addSceneNodeEntity(Entity *entity);

    EntityManager &getEntityManager();

    const PlayerFactory &getPlayerFactory() const;
	const BombFactory &getBombFactory() const;

	GeometryObjectManager* getGeometryManager();
	MaterialManager* getMaterialManager();
	
	Camera &getCamera();

private:
    ConfigSettings *config;
    InputManager *inputManager;

    EntityManager entityManager;
    PlayerFactory playerFactory;
	BombFactory bombFactory;

    ClientEventHandlerLookup eventHandlerLookup;
    GameClient client;

	GeometryObjectManager * _geometryManager;
	MaterialManager * _materialManager;

	Camera &_camera;
    // osgViewer::Viewer viewer;
    // osg::Group *rootNode;
    // void setupCamera();
};
