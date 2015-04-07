#include <iostream>

#include <Windows.h>

#include <osg/Camera>
#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osgViewer/Viewer>

#include "input/KeyboardEventHandler.h"
#include "core/Player.h"
#include "PlayerNode.h"
#include "PlayerNodeCallback.h"
#include "util/ConfigSettings.h"

Player player;

void setupKeyboardHandler(KeyboardEventHandler *handler) {
    handler->bindKey('w', KeyboardEventHandler::KEY_DOWN, Player::moveForwardDown);
    handler->bindKey('w', KeyboardEventHandler::KEY_UP, Player::moveForwardUp);
    handler->bindKey('s', KeyboardEventHandler::KEY_DOWN, Player::moveBackwardDown);
    handler->bindKey('s', KeyboardEventHandler::KEY_UP, Player::moveBackwardUp);
    handler->bindKey('a', KeyboardEventHandler::KEY_DOWN, Player::moveLeftDown);
    handler->bindKey('a', KeyboardEventHandler::KEY_UP, Player::moveLeftUp);
    handler->bindKey('d', KeyboardEventHandler::KEY_DOWN, Player::moveRightDown);
    handler->bindKey('d', KeyboardEventHandler::KEY_UP, Player::moveRightUp);
    handler->bindKey(' ', KeyboardEventHandler::KEY_DOWN, Player::jumpDown);
    handler->bindKey(' ', KeyboardEventHandler::KEY_UP, Player::jumpUp);
}

void setupCamera(osgViewer::Viewer &viewer) {
	const osg::Vec3 eye(0, -10, 0);
	const osg::Vec3 center(0, 1, 0);

	osgViewer::ViewerBase::Contexts context;
	viewer.getContexts(context, true);

	const osg::GraphicsContext::Traits* traits = context.front()->getTraits();

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

    osg::ref_ptr<osg::Group> rootNode = new osg::Group;

    osg::ref_ptr<PlayerNode> playerNode = new PlayerNode(&player);
    playerNode->addUpdateCallback(new PlayerNodeCallback);

    Player player2;
    player2.position = osg::Vec3(2, 2, 0);

    osg::ref_ptr<PlayerNode> player2Node = new PlayerNode(&player2);

    rootNode->addChild(playerNode);
    rootNode->addChild(player2Node);

    viewer.setSceneData(rootNode);

	osg::ref_ptr<KeyboardEventHandler> kbdHandler = new KeyboardEventHandler;
	setupKeyboardHandler(kbdHandler);
	viewer.addEventHandler(kbdHandler);

	viewer.realize();

	setupCamera(viewer); // Need to be called after viewer.realize()

	while (!viewer.done()) {
		viewer.frame();
	}
}
