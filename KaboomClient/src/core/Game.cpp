#include "Game.h"

#include <iostream>

#include <osg/Group>

#include "../Global.h"
#include "SceneNodeComponent.h"
#include "GameStateData.h"

Game::Game(ConfigSettings *config)
    : playerFactory(&entityManager) {
    inputManager = new InputManager(&viewer);
    inputManager->loadConfig();

    int screen_width = 0, screen_height = 0;
    config->getValue(ConfigSettings::str_screen_width, screen_width);
    config->getValue(ConfigSettings::str_screen_height, screen_height);

    viewer.setUpViewInWindow(100, 100, screen_width, screen_height);

    Player *player1 = playerFactory.createPlayer(0, 0, 0);
    Player *player2 = playerFactory.createPlayer(0, 0, 0);

    osg::Node *player1Node = player1->getComponent<SceneNodeComponent>()->getNode();
    osg::Node *player2Node = player2->getComponent<SceneNodeComponent>()->getNode();

    rootNode = new osg::Group;
    rootNode->addChild(player1Node);
    rootNode->addChild(player2Node);

    viewer.setSceneData(rootNode);
}

Game::~Game() {
    delete inputManager;
}

void Game::run() {
    viewer.realize();

    setupCamera();

    while (!viewer.done()) {
        GameStateData *gameState = g_client->receive();

        if (gameState != nullptr) {
            ClientPlayer *player1 = static_cast<ClientPlayer *>(entityManager.getEntity(0));
            ClientPlayer *player2 = static_cast<ClientPlayer *>(entityManager.getEntity(1));

            player1->setPosition(gameState->x1, gameState->y1, gameState->z1);
            player2->setPosition(gameState->x2, gameState->y2, gameState->z2);
        }

        viewer.frame();

        delete gameState;
    }
}

void Game::setupCamera() {
    const osg::Vec3 eye(0, -10, 0);
    const osg::Vec3 center(0, 1, 0);

    osgViewer::ViewerBase::Contexts context;
    viewer.getContexts(context, true);

    const osg::GraphicsContext::Traits *traits = context.front()->getTraits();

    if (!traits) {
        std::cerr << "Unable to detect screen resolution" << std::endl;
        exit(1);
    }

    float screenHeight = traits->height;
    float screenWidth = traits->width;

    osg::Matrixf viewMat;
    viewMat.makeLookAt(eye, center, osg::Vec3(0, 0, 1));

    osg::Matrixf projMat;
    projMat.makePerspective(30, screenWidth / screenHeight, 1, 1000);

    osg::Camera *camera = viewer.getCamera();
    camera->setViewMatrix(viewMat);
    camera->setProjectionMatrix(projMat);
}

const EntityManager &Game::getEntityManager() const {
    return entityManager;
}

const PlayerFactory &Game::getPlayerFactory() const {
    return playerFactory;
}
