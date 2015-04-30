#pragma once

#include <osgViewer/Viewer>

//#include "../osgLibRocket/GuiNode.h"

#include "EffectCompositor.h"
#include "CompositorAnalysis.h"
#include "World.h"
#include "Camera.h"
#include "TwGUIManager.h"
#include "SkyBox.h"
#include "AxisVisualizer.h"
#include "CubeMapPreFilter.h"

class Core
{
public:
	static void init(int winPosX, int winPosY, int winWidth, int winHeight, int resolutionWidth, int resolutionHeight, const std::string &mediaPath);

	static osg::Vec2 getScreenSize();

	static void loadWorldFile(const std::string &worldFilePath);
	static World &getWorldRef();

	static Camera &getMainCamera();

	static void AdvanceFrame();
	static void finalize();

	static void setEnvironmentMap(
		const std::string &posX,
		const std::string &negX,
		const std::string &posY,
		const std::string &negY,
		const std::string &posZ, 
		const std::string &negZ);

	static void setEnvironmentMapVerticalCross(const std::string &cubemap);

	static void enableCameraManipulator();
	static void disableCameraManipulator();

	static void enablePassDataDisplay();
	static void disablePassDataDisplay();

	static void enableGUI();
	static void disableGUI();

	static void enableGameMode();
	static void disableGameMode();

	static void enableGeometryObjectManipulator();
	static void disableGeometryObjectManipulator();

	static bool isInGameMode();
	static double getLastFrameDuration();
	static bool isViewerClosed();

	static void addEventHandler(osgGA::GUIEventHandler *handler);

	static void setAllowChangeEditorProjection(bool tf);
	static bool allowChangeEditorProjection();

	static void requestPrefilterCubeMap();
	static void requestPrefilterCubeMapWithCubeMap(osg::TextureCubeMap *cubemap);

	// static void run();

private:
	static void configPasses();
	static void configSceneNode();
	static void configViewer();
	static void configSkyBox();
	static void configGeometryObjectManipulator();

	static void configCubemapPrefilterPass();
	static void configGeometryPass();
	static void configLightPass();

	static void configFilePath();
	static void configInGameGUI();
	static void configAxisVisualizer();

	static std::string _mediaPath;
		
	static osg::ref_ptr<osgFX::EffectCompositor> _passes;
	static osg::ref_ptr<osg::Group> _sceneRoot;
	static osg::ref_ptr<osg::Group> _geomRoot;
	static osg::ref_ptr<SkyBox> _skybox;

	static osg::ref_ptr<osgViewer::Viewer> _viewer;
	//static osgLibRocket::GuiNode *_gameGUI;

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
	static Camera _savedManipulatorCam;

	static osg::ref_ptr<CompositorAnalysis> _analysisHUD;

	// on screen flags
	static bool _gameMode;
	static bool _passDataDisplay;
	static bool _guiEnabled;
	static bool _manipulatorEnabled;

	static bool _isFirstFrame;

	static bool _allowEditorChangeProjection;
	static bool _requestPrefilterCubeMap;

	static osg::Timer_t _lastFrameStartTime; 
	static osg::Timer_t _frameStartTime; 

	static AxisVisualizer _axisVisualizer;
	static CubeMapPreFilter _cubemapPreFilter;

};

class MainCameraCallback : public osg::NodeCallback
{
public:
	void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		if (!Core::isInGameMode() && !Core::allowChangeEditorProjection()) return;
		osg::Camera *mainCam = static_cast<osg::Camera *>(node);
		mainCam->setViewMatrix(Core::getMainCamera().getViewMatrix());
		mainCam->setProjectionMatrix(Core::getMainCamera().getProjectionMatrix());
	}
};