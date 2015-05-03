#pragma once

#include <core/EntityManager.h>
#include <util/ConfigSettings.h>

#include <Camera.h>
#include <GeometryObjectManager.h>
#include <MaterialManager.h>

#include "BombFactory.h"
#include "PlayerFactory.h"
#include "../network/ClientEventHandlerLookup.h"
#include "../network/GameClient.h"

#include <osgAudio/Source.h>
#include <osgAudio/AudioEnvironment.h>
#include <osgAudio/Sample.h>

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
    const PlayerFactory &getPlayerFactory() const;
    const BombFactory &getBombFactory() const;

    const GameClient &getGameClient() const;

    GeometryObjectManager* getGeometryManager();
    MaterialManager* getMaterialManager();

    Camera &getCamera();
	osg::ref_ptr<Source> source;
	osg::ref_ptr<Sample> sample;

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
    PlayerFactory playerFactory;
	BombFactory bombFactory;

    ClientEventHandlerLookup eventHandlerLookup;
    GameClient client;

	GeometryObjectManager * _geometryManager;
	MaterialManager * _materialManager;

	Camera &_camera;
};
