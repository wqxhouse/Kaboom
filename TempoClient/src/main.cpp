#include <iostream>

#include <Windows.h>

#include <osg/Camera>
#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osgViewer/Viewer>

#include "core/Player.h"
#include "core/PlayerData.h"
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

int main() {
	// Load config file for the first time
	ConfigSettings* config = ConfigSettings::config;

	int screen_width = 0, screen_height = 0;
	config->getValue(ConfigSettings::str_screen_width, screen_width);
	config->getValue(ConfigSettings::str_screen_height, screen_height);

	cout << "str_screen_width: " << screen_width << endl;

    g_client = new GameClient(config);

    osgViewer::Viewer viewer;
    InputManager inputManager(&viewer);
    inputManager.loadConfig();

    osg::ref_ptr<osg::Group> root = new osg::Group;

    PlayerData player1Data;
    player1Data.id = 1;
    player1Data.x = 0;
    player1Data.y = 0;
    player1Data.z = 0;

    PlayerData player2Data;
    player2Data.id = 2;
    player2Data.x = 2;
    player2Data.y = 2;
    player2Data.z = 0;

    osg::ref_ptr<Player> player1Node = new Player(&player1Data);
    osg::ref_ptr<Player> player2Node = new Player(&player2Data);

    player1Node->addUpdateCallback(new PlayerUpdateCallback);
    player2Node->addUpdateCallback(new PlayerUpdateCallback);

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
