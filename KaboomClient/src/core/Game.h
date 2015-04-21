#pragma once

#include <osgViewer/Viewer>
#include <core/EntityManager.h>
#include <ConfigSettings.h>
#include "BombFactory.h"
#include "PlayerFactory.h"
#include "../input/InputManager.h"
#include "../network/ClientEventHandlerLookup.h"

class ClientEventHandlerLookup;

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
    InputManager *inputManager;
    EntityManager entityManager;
    PlayerFactory playerFactory;
	BombFactory bombFactory;
    ClientEventHandlerLookup *eventHandlerLookup;

    //osgViewer::Viewer viewer;

    osg::Group *rootNode;

    void setupCamera();
};
