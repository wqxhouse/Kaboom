#pragma once

#include <Windows.h>

#include <osgViewer/Viewer>

#include <core/EntityManager.h>

#include "PlayerFactory.h"
#include "../input/InputManager.h"
#include "../util/ConfigSettings.h"

class Game {
public:
    Game(ConfigSettings *config);
    ~Game();

    void run();

    const EntityManager &getEntityManager() const;
    const PlayerFactory &getPlayerFactory() const;

private:
    InputManager *inputManager;
    EntityManager entityManager;
    PlayerFactory playerFactory;

    osgViewer::Viewer viewer;

    osg::Group *rootNode;

    void setupCamera();
};
