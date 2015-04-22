#include "Game.h"
#include <iostream>
#include <osg/Group>
#include <Core.h>
#include "../Global.h"
//#include "SceneNodeComponent.h"
#include "../Scene.h"
#include "../input/InputManager.h"
#include "../network/ClientEventHandlerLookup.h"
#include "../network/GameClient.h"

Game::Game(ConfigSettings *config)
    : playerFactory(&entityManager),
      bombFactory(&entityManager),
      eventHandlerLookup(new ClientEventHandlerLookup(this)),
      client(new GameClient(config, eventHandlerLookup)) {
    std::string mediaPath, screenPosXStr, screenPosYStr, renbufferWStr, renbufferHStr, screenWStr, screenHStr;
    config->getValue(ConfigSettings::str_mediaFilePath, mediaPath);
    config->getValue(ConfigSettings::str_screenPosX, screenPosXStr);
    config->getValue(ConfigSettings::str_screenPosY, screenPosYStr);
    config->getValue(ConfigSettings::str_renderBuffer_width, renbufferWStr);
    config->getValue(ConfigSettings::str_renderBuffer_height, renbufferHStr);
    config->getValue(ConfigSettings::str_screen_width, screenWStr);
    config->getValue(ConfigSettings::str_screen_height, screenHStr);

    int posX = atoi(screenPosXStr.c_str());
    int posY = atoi(screenPosYStr.c_str());
    int bufferW = atoi(renbufferWStr.c_str());
    int bufferH = atoi(renbufferHStr.c_str());
    int screenW = atoi(screenWStr.c_str());
    int screenH = atoi(screenHStr.c_str());

    Core::init(posX, posY, screenW, screenH, bufferW, bufferH, mediaPath);
    setupScene();

    inputManager = new InputManager(*client);
    inputManager->loadConfig();
}

Game::~Game() {
    delete inputManager;
    delete eventHandlerLookup;
}

void Game::run() {
    static bool connected = false;

    Core::finalize();
    while (true) {
        if (Core::isInGameMode() && !connected) {
            client->connectToServer();
            connected = true;
        }

        if (connected) {
            client->receive();
        }

        if (!Core::isInGameMode() && connected) {
            client->disconnectFromServer();
            connected = false;
        }

        Core::AdvanceFrame();
    }
}

bool Game::addSceneNodeEntity(Entity *entity) {
    //   SceneNodeComponent *sceneNodeCom = entity->getComponent<SceneNodeComponent>();
    //   if (sceneNodeCom == nullptr) 
    //{
    //       return false;
    //   }
    // rootNode->addChild(sceneNodeCom->getNode());


    return true;
}

const EntityManager &Game::getEntityManager() const {
    return entityManager;
}

const PlayerFactory &Game::getPlayerFactory() const {
    return playerFactory;
}

const BombFactory &Game::getBombFactory() const {
    return bombFactory;
}

ClientEventHandlerLookup *Game::getEventHandlerLookup() const {
    return eventHandlerLookup;
}
