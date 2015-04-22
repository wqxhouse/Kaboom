#pragma once

#include <ConfigSettings.h>
#include <core/EntityManager.h>

#include "BombFactory.h"
#include "PlayerFactory.h"
#include "../network/GameClient.h"

class ClientEventHandlerLookup;
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

    bool addSceneNodeEntity(Entity *entity);

    const EntityManager &getEntityManager() const;
    const PlayerFactory &getPlayerFactory() const;
	const BombFactory &getBombFactory() const;

    ClientEventHandlerLookup *getEventHandlerLookup() const;

private:
    ConfigSettings *config;
    InputManager *inputManager;

    EntityManager entityManager;
    PlayerFactory playerFactory;
	BombFactory bombFactory;

    GameClient *client;
    ClientEventHandlerLookup *eventHandlerLookup;

    // osgViewer::Viewer viewer;
    // osg::Group *rootNode;
    // void setupCamera();
};
