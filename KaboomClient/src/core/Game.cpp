#include "Game.h"

#include <iostream>

#include <osg/Group>

#include "../Global.h"
#include "SceneNodeComponent.h"
#include "GameStateData.h"

Game::Game(ConfigSettings *config)
    : playerFactory(&entityManager),
    eventHandlerLookup(new ClientEventHandlerLookup(this)),
    rootNode(new osg::Group) {
    inputManager = new InputManager(&viewer);
    inputManager->loadConfig();

    int screen_width = 0, screen_height = 0;
    config->getValue(ConfigSettings::str_screen_width, screen_width);
    config->getValue(ConfigSettings::str_screen_height, screen_height);

    viewer.setUpViewInWindow(100, 100, screen_width, screen_height);

    viewer.setSceneData(rootNode);
}

Game::~Game() {
    delete inputManager;
    delete eventHandlerLookup;
}

void Game::run() {
    viewer.realize();

    setupCamera();

    while (!viewer.done()) {
        g_client->receive();
        viewer.frame();
    }
}

bool Game::addSceneNodeEntity(Entity *entity) {
    SceneNodeComponent *sceneNodeCom = entity->getComponent<SceneNodeComponent>();

    if (sceneNodeCom == nullptr) {
        return false;
    }

    rootNode->addChild(sceneNodeCom->getNode());

    return true;
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

ClientEventHandlerLookup *Game::getEventHandlerLookup() const {
    return eventHandlerLookup;
}
