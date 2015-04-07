#include <iostream>

#include <Windows.h>

#include <osg/Camera>
#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osgViewer/Viewer>

#include "core/Player.h"
#include "core/PlayerData.h"
#include "input/InputManager.h"
#include "network/MockClient.h"
#include "network/MockClientUpdateCallback.h"
#include "util/ConfigSettings.h"

MockClient g_client;

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

    osgViewer::Viewer viewer;
    InputManager inputManager(&viewer);
    inputManager.loadConfig();

    osg::ref_ptr<osg::Group> root = new osg::Group;
    root->addUpdateCallback(new MockClientUpdateCallback(&g_client));

    PlayerData player1Data;
    player1Data.id = 1;
    player1Data.position = osg::Vec3(0, 0, 0);

    PlayerData player2Data;
    player2Data.id = 2;
    player2Data.position = osg::Vec3(2, 2, 0);

    osg::ref_ptr<Player> player1Node = new Player(&player1Data);
    osg::ref_ptr<Player> player2Node = new Player(&player2Data);

    root->addChild(player1Node);
    root->addChild(player2Node);

    viewer.setSceneData(root);

    viewer.realize();

    setupCamera(viewer); // Need to be called after viewer.realize()

    while (!viewer.done()) {
        viewer.frame();
    }
}
