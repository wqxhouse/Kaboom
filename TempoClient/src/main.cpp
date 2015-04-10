#include <iostream>

#include <Windows.h>

#include <osg/Camera>
#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osgViewer/Viewer>

#include "core/Entity.h"
#include "core/EntityManager.h"
#include "core/SceneNodeComponent.h"
#include "core/PositionComponent.h"
#include "core/PlayerUpdateCallback.h"
#include "input/InputManager.h"
#include "network/GameClient.h"
#include "util/ConfigSettings.h"

GameClient *g_client;
GameStateUpdateEvent *g_gameState;

void setupCamera(osgViewer::Viewer &viewer) {
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

Entity * createPlayerEntity(EntityManager &entityManager, float x, float y, float z) {
    Entity *player = entityManager.createEntity();

    osg::Box *box = new osg::Box;
    osg::ShapeDrawable *drawable = new osg::ShapeDrawable(box);
    osg::Geode *model = new osg::Geode;
    model->addDrawable(drawable);

    osg::MatrixTransform *transformation = new osg::MatrixTransform;
    transformation->addChild(model);

    osg::Group *playerNode = new osg::Group;

    playerNode->addChild(transformation);
    playerNode->addUpdateCallback(new PlayerUpdateCallback(player));

    player->attachComponent(new SceneNodeComponent(playerNode));
    player->attachComponent(new PositionComponent(x, y, z));

    return player;
}

int main() {
	// Load config file for the first time
	ConfigSettings* config = ConfigSettings::config;

	int screen_width = 0, screen_height = 0;
	config->getValue(ConfigSettings::str_screen_width, screen_width);
	config->getValue(ConfigSettings::str_screen_height, screen_height);

    g_client = new GameClient(config);

    osgViewer::Viewer viewer;

    InputManager inputManager(&viewer);
    inputManager.loadConfig();

    EntityManager entityManager;

    Entity *player1 = createPlayerEntity(entityManager, 0, 0, 0);
    Entity *player2 = createPlayerEntity(entityManager, 2, 2, 0);

    osg::Node *player1Node = static_cast<SceneNodeComponent *>(player1->getComponent(SCENE_NODE))->getNode();
    osg::Node *player2Node = static_cast<SceneNodeComponent *>(player2->getComponent(SCENE_NODE))->getNode();

    osg::ref_ptr<osg::Group> root = new osg::Group;
    root->addChild(player1Node);
    root->addChild(player2Node);

    viewer.setSceneData(root);
	viewer.setUpViewInWindow(100, 100, screen_width, screen_height);

    viewer.realize();

    setupCamera(viewer); // Need to be called after viewer.realize()

    while (!viewer.done()) {
        g_gameState = g_client->receive();

        viewer.frame();

        delete g_gameState;
    }

    delete g_client;
}
