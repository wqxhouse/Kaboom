#pragma once

#include <osgViewer/Viewer>

#include <ConfigSettings.h>
#include <core/EntityManager.h>

#include "BombFactory.h"
#include "PlayerFactory.h"

class ClientEventHandlerLookup;
class InputManager;

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

    // osgViewer::Viewer viewer;
    // osg::Group *rootNode;
    // void setupCamera();
};
