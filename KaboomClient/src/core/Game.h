#pragma once

#include <Windows.h>

#include <osgViewer/Viewer>

#include <core/EntityManager.h>

#include "PlayerFactory.h"
#include "../input/InputManager.h"
#include "../network/ClientEventHandlerLookup.h"
#include "../util/ConfigSettings.h"

class ClientEventHandlerLookup;

class Game {
public:
    Game(ConfigSettings *config);
    ~Game();

    void run();

    bool addSceneNodeEntity(Entity *entity);

    const EntityManager &getEntityManager() const;
    const PlayerFactory &getPlayerFactory() const;
    ClientEventHandlerLookup *getEventHandlerLookup() const;

private:
    InputManager *inputManager;
    EntityManager entityManager;
    PlayerFactory playerFactory;
    ClientEventHandlerLookup *eventHandlerLookup;

    osgViewer::Viewer viewer;

    osg::Group *rootNode;

    void setupCamera();
};
