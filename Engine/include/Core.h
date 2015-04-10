#pragma once

#include <osgViewer/Viewer>
#include "EffectCompositor.h"
#include "World.h"
#include "Camera.h"
#include "TwGUIManager.h"

class Core
{
public:
	static void init(int winWidth, int winHeight, int resolutionWidth, int resolutionHeight);
	static osg::Vec2 getScreenSize();

	static void loadWorldFile(const std::string &worldFilePath);
	static World &getWorldRef();

	static const Camera &getMainCamera();

	static void disableCameraManipulator();
	static void enableCameraManipulator();

	static void run();

private:
	static void configPasses();
	static void configSceneNode();
	static void configViewer();

	static void configGeometryPass();
	static void configLightPass();

	static void configFilePath();
		
	static osg::ref_ptr<osgFX::EffectCompositor> _passes;
	static osg::ref_ptr<osg::Group> _sceneRoot;
	static osg::ref_ptr<osg::Group> _geomRoot;

	static osg::ref_ptr<osgViewer::Viewer> _viewer;

	static osg::Vec2 _screenSize;
	static osg::Vec2 _renderResolution;

	static World _world;
	static bool _hasInit;

	// mainCamera
	friend class osgFX::EffectCompositor;
	static Camera _cam;

	static osg::ref_ptr<TwGUIManager> _gui;

	// use as a temp for temporarily remove the manipulator when out of focus
	// CAUTIOUS: when enabled, this variable is NULL
	static osg::ref_ptr<osgGA::CameraManipulator> _camManipulatorTemp;
};

