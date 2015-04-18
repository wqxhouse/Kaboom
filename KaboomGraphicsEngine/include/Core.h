#pragma once

#include <osgViewer/Viewer>
#include "EffectCompositor.h"
#include "World.h"
#include "Camera.h"
#include "TwGUIManager.h"
#include "SkyBox.h"

class Core
{
public:
	static void init(int winPosX, int winPosY, int winWidth, int winHeight, int resolutionWidth, int resolutionHeight, const std::string &mediaPath);

	static osg::Vec2 getScreenSize();

	static void loadWorldFile(const std::string &worldFilePath);
	static World &getWorldRef();

	static const Camera &getMainCamera();

	static void setEnvironmentMap(
		const std::string &posX,
		const std::string &negX,
		const std::string &posY,
		const std::string &negY,
		const std::string &posZ, 
		const std::string &negZ);

	static void disableCameraManipulator();
	static void enableCameraManipulator();

	static void run();

private:
	static void configPasses();
	static void configSceneNode();
	static void configViewer();
	static void configSkyBox();

	static void configGeometryPass();
	static void configLightPass();

	static void configFilePath();

	static std::string _mediaPath;
		
	static osg::ref_ptr<osgFX::EffectCompositor> _passes;
	static osg::ref_ptr<osg::Group> _sceneRoot;
	static osg::ref_ptr<osg::Group> _geomRoot;
	static osg::ref_ptr<SkyBox> _skybox;

	static osg::ref_ptr<osgViewer::Viewer> _viewer;

	static osg::Vec2 _screenSize;
	static osg::Vec2 _renderResolution;
	static osg::Vec2 _winPos;

	static World _world;
	static bool _hasInit;
	static bool _hasEnvMap;

	// mainCamera
	friend class osgFX::EffectCompositor;
	static Camera _cam;

	static osg::ref_ptr<TwGUIManager> _gui;

	// use as a temp for temporarily remove the manipulator when out of focus
	// CAUTIOUS: when enabled, this variable is NULL
	static osg::ref_ptr<osgGA::CameraManipulator> _camManipulatorTemp;
};

