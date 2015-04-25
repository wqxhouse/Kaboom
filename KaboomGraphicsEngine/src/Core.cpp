#include "stdafx.h" 

#include "Core.h"

#include <osg/Vec3>
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>
#include <osg/TexGen>
#include <osg/Depth>
#include <osgViewer/GraphicsWindow>
//
//#include "../osgLibRocket/FileInterface.h"
//#include "../osgLibRocket/RenderInterface.h"
//#include "../osgLibRocket/SystemInterface.h"

#include "World.h"
#include "GeometryObjectManager.h"
#include "LightManager.h"
#include "LightPrePassCallback.h"
#include "LightPassCallback.h"
#include "CustomFirstPersonManipulator.h"
#include "GeometryPicker.h"
#include "GeometryObjectManipulator.h"
#include "CubemapUtil.h"

// TODO: log which one called the global functions in Core
// for debugging

extern osg::ref_ptr<CompositorAnalysis> configureViewerForMode(osgViewer::Viewer& viewer, osgFX::EffectCompositor* compositor,
		osg::Node* model, int displayMode);

void Core::init(int winPosX, int winPosY, int winWidth, int winHeight, int resolutionWidth, int resolutionHeight, const std::string &mediaPath)
{
	_mediaPath = mediaPath;
	// TODO: add reshape callback for winPos, winHeight, bufferSize
	_screenSize = osg::Vec2(winWidth, winHeight);
	// TODO: test separation of screenSize and the renderResolution
	_renderResolution = osg::Vec2(resolutionWidth, resolutionHeight);
	_winPos = osg::Vec2(winPosX, winPosY);
	_sceneRoot = new osg::Group;
	_gui = new TwGUIManager;
	_skybox = new SkyBox;

	_analysisHUD = NULL;
	_passDataDisplay = false;
	_gameMode = false;
	_guiEnabled = true;
	_isFirstFrame = true;
	_allowEditorChangeProjection = false;

	configFilePath();

	configSceneNode();
	configPasses();
	configViewer(); 

	_sceneRoot->addChild(_passes);
	_hasInit = true;
}

void Core::loadWorldFile(const std::string &worldFilePath)
{
	_world.loadWorldXMLFile(worldFilePath);
}

World &Core::getWorldRef()
{
	return _world;
}

osg::Vec2 Core::getScreenSize()
{
	return _screenSize;
}

void Core::configFilePath()
{
	// TODO: read from config file
	osgDB::FilePathList& filePaths = osgDB::getDataFilePathList();

	if (_mediaPath.empty())
	{
		OSG_WARN << "Core:: Media path for graphics engine not set" << std::endl;
	}

	std::string rootPath = _mediaPath;

	// TODO : currently minimum check, can do comprehensive checks later
	std::string pipelineRelative = "EffectFiles\\Pipeline.xml";
	std::string shaderConstantRelative = "Shaders\\constant.glsl";
	std::string pipelinePath = rootPath + pipelineRelative;
	std::string shaderConstantPath = rootPath + shaderConstantRelative;
	
	if (!osgDB::fileExists(pipelinePath))
	{
		fprintf(stderr, "Core::configFilePath: %s not found...\n", pipelinePath.c_str());
		exit(0);
	}

	if (!osgDB::fileExists(shaderConstantPath))
	{
		fprintf(stderr, "Core::configFilePath: %s not found...\n", shaderConstantPath.c_str());
		exit(0);
	}

	if (osgDB::findDataFile(pipelineRelative).empty())
	{
		filePaths.push_back(rootPath);
		
		if (osgDB::findDataFile(pipelineRelative).empty())
		{
			fprintf(stderr, "Core::configFilePath:: %s exists, but relative path %s "
				"is not working even though osg path is configured\n", pipelinePath.c_str(), pipelineRelative.c_str());
			exit(0);
		}
	}

	// TODO: do the same for other files
}

void Core::configPasses()
{
	_passes = new osgFX::EffectCompositor;

	// TODO: blit render buffer low res to final viewport high res
	_passes->setRenderTargetResolution(osg::Vec3(_renderResolution, 1));
	_passes->loadFromXMLFile("EffectFiles/Pipeline.xml");

	configGeometryPass();
	configLightPass();
}

void Core::configGeometryPass()
{
	// Generate GBuffer in the forward pass
	_geomRoot->setNodeMask(0x1);
	_passes->addChild(_geomRoot);

	// TODO: link GUI control
}

void Core::configLightPass()
{
	osgFX::EffectCompositor::PassData prePass;
	_passes->getPassData("LightPrePass", prePass);

	if (prePass.pass != NULL)
	{
		prePass.pass->getOrCreateStateSet()->
			setUpdateCallback(new LightPrePassCallback(_world.getLightManager()));
	}
	else
	{
		OSG_WARN << "Prepass not defined in the pipeline, check the xml file " << std::endl;
	}

	osgFX::EffectCompositor::PassData lightPass;
	_passes->getPassData("LightPass", lightPass);

	if (lightPass.pass != NULL)
	{
		lightPass.pass->getOrCreateStateSet()->
			setUpdateCallback(new LightPassCallback());
	}
	else
	{
		OSG_WARN << "Light pass not defined in the pipeline, check the xml file " << std::endl;
	}
}

void Core::configGeometryObjectManipulator()
{
	GeometryObjectManipulator::initWithRootNode(_passes);
}

void Core::configCubemapPrefilterPass()
{
}

void Core::configSceneNode()
{
	_geomRoot = new osg::Group;
	_geomRoot->addChild(_world.getGeometryManager()->getRootNode());
}

void Core::configViewer()
{
	_viewer = new osgViewer::Viewer;
	_viewer->setSceneData(_sceneRoot);

	// add gui
	_viewer->addEventHandler(_gui.get());
	// _viewer->addEventHandler(new osgViewer::StatsHandler);
	_viewer->getCamera()->setFinalDrawCallback(_gui.get());

	_viewer->setUpViewInWindow(_winPos.x(), _winPos.y(), _screenSize.x(), _screenSize.y());
	_viewer->setKeyEventSetsDone(0);

	_viewer->addEventHandler(new GeometryPicker);
}

void Core::AdvanceFrame()
{
	if (_hasInit)
	{
		if (_isFirstFrame)
		{
			finalize();
			_lastFrameStartTime = _frameStartTime 
				= osg::Timer::instance()->getStartTick();
			
			_isFirstFrame = false;
		}

		_viewer->frame();
		_lastFrameStartTime = _frameStartTime;
		_frameStartTime = osg::Timer::instance()->tick();
	}
}

double Core::getLastFrameDuration()
{
	return osg::Timer::instance()->delta_s(_lastFrameStartTime, _frameStartTime);
}

bool Core::isViewerClosed()
{
	return _viewer->done();
}

void Core::finalize()
{
	if (!_hasInit)
	{
		return;
	}
	
	// TODO: add console output : initializing GUI
	_gui->initializeTwGUI();
	
	// skybox
	if (_hasEnvMap)
	{
		configSkyBox();
	}

	configInGameGUI();
	configGeometryObjectManipulator();
	configAxisVisualizer();

	_analysisHUD = configureViewerForMode(*_viewer, _passes, NULL, 1);
	_analysisHUD->toggleHelper(); // disabled by default

	//_viewer->run();
	_viewer->setCameraManipulator(new osgGA::TrackballManipulator);
	_viewer->getCamera()->setUpdateCallback(new MainCameraCallback);

	_viewer->realize();
	//while (!_viewer->done())
	//{
	//	if (_camCallback != NULL)
	//	{
	//		_camCallback(&_cam);
	//	}

	//	Camera &c = getMainCamera();
	//	//std::cout << "Before: " << c.getLookAt() << std::endl;
	//	const osg::Vec3 &cc = osg::Vec3(100, 100, 100);
	//	//c.setLookAtAndUpdate(cc);
	//	//std::cout << "After1 : " << c.getLookAt() << std::endl;
	//	
	//	_viewer->frame();
	//}
}

Camera &Core::getMainCamera()
{
	return _cam;
}

// Used in GUI focus
void Core::disableCameraManipulator()
{
	if (_viewer->getCameraManipulator() != NULL)
	{
		_camManipulatorTemp = _viewer->getCameraManipulator();
		_savedManipulatorCam = _cam;
		std::cout << _savedManipulatorCam.getEyePosition() << std::endl;
		_viewer->setCameraManipulator(NULL);

		_gui->setCameraManipulatorActive(false);
	}
}

void Core::enableCameraManipulator()
{
	std::cout << _savedManipulatorCam.getEyePosition() << std::endl;
	_camManipulatorTemp->setHomePosition(_savedManipulatorCam.getEyePosition(), 
		_savedManipulatorCam.getLookAt(), _savedManipulatorCam.getUp());
	_viewer->getCamera()->setProjectionMatrix(_savedManipulatorCam.getProjectionMatrix());
	_viewer->setCameraManipulator(_camManipulatorTemp);

	_savedManipulatorCam = Camera();
	_camManipulatorTemp = NULL;

	_gui->setCameraManipulatorActive(true);
}


void Core::enableGeometryObjectManipulator()
{
	GeometryObjectManipulator::setVisible(_manipulatorEnabled);
}

void Core::disableGeometryObjectManipulator()
{
	// TODO: refactor bad pattern, since Manipuator is global
	// thus this variable may not be up to date
	// thus needs to be very careful
	// Actually can make it an instance.
	_manipulatorEnabled = GeometryObjectManipulator::isVisible();
	GeometryObjectManipulator::setVisible(false);
}

void Core::configSkyBox()
{
	_skybox->setNodeMask(0x2);
	_skybox->setGeomRoot(_geomRoot);
	_passes->addChild(_skybox);
}

void Core::configInGameGUI()
{
	//	osgLibRocket::FileInterface* file_interface = new osgLibRocket::FileInterface();
	//	Rocket::Core::SetFileInterface(file_interface);
	//
	//	// create and set libRocket to OSG interfaces
	//	osgLibRocket::RenderInterface* renderer = new osgLibRocket::RenderInterface();
	//	Rocket::Core::SetRenderInterface(renderer);
	//
	//	osgLibRocket::SystemInterface* system_interface = new osgLibRocket::SystemInterface();
	//	Rocket::Core::SetSystemInterface(system_interface);
	//
	//	Rocket::Core::Initialise();
	//}
}
void Core::setEnvironmentMap(
	const std::string &posX,
	const std::string &negX,
	const std::string &posY,
	const std::string &negY,
	const std::string &posZ,
	const std::string &negZ)
{
	_skybox->setEnvironmentMap(0,
		osgDB::readImageFile(posX), osgDB::readImageFile(negX),
		osgDB::readImageFile(posY), osgDB::readImageFile(negY),
		osgDB::readImageFile(posZ), osgDB::readImageFile(negZ));

	_hasEnvMap = true;
}

void Core::setEnvironmentMapVerticalCross(const std::string &cubemap)
{
	CubemapUtil ut;
	bool res = ut.loadVerticalCross(cubemap);
	if (res)
	{
		_skybox->setEnvironmentMap(0,
			ut.getImage(osg::TextureCubeMap::POSITIVE_X),
			ut.getImage(osg::TextureCubeMap::NEGATIVE_X),
			ut.getImage(osg::TextureCubeMap::POSITIVE_Y),
			ut.getImage(osg::TextureCubeMap::NEGATIVE_Y),
			ut.getImage(osg::TextureCubeMap::POSITIVE_Z),
			ut.getImage(osg::TextureCubeMap::NEGATIVE_Z));

		_hasEnvMap = true;
	}
	else
	{
		OSG_WARN << "Environment map set failed" << std::endl;
	}
}

void Core::disablePassDataDisplay()
{
	if (_passDataDisplay)
	{
		_analysisHUD->toggleHelper();
		_passDataDisplay = false;
	}
}

void Core::enablePassDataDisplay()
{
	if (!_passDataDisplay && !_gameMode)
	{
		_analysisHUD->toggleHelper();
		_passDataDisplay = true;
	}
}

void Core::enableGUI()
{
	if (!_guiEnabled && !_gameMode)
	{
		_viewer->addEventHandler(_gui.get());
		_viewer->getCamera()->setFinalDrawCallback(_gui.get());
		_guiEnabled = true;
	}
}

void Core::disableGUI()
{
	if (_guiEnabled)
	{
		_viewer->removeEventHandler(_gui.get());
		_viewer->getCamera()->setFinalDrawCallback(NULL);
		_guiEnabled = false;
	}
}

void Core::enableGameMode()
{
	if (!_gameMode)
	{
		_gameMode = true;

		disablePassDataDisplay();
		disableCameraManipulator();
		disableGUI();
		disableGeometryObjectManipulator();
		auto a = static_cast<osgViewer::GraphicsWindow *>(_viewer->getCamera()->getGraphicsContext());
		a->setCursor(osgViewer::GraphicsWindow::NoCursor);
	}
}

void Core::disableGameMode()
{
	if (_gameMode)
	{
		_gameMode = false;

		enableGUI();
		enableCameraManipulator();
		enableGeometryObjectManipulator();

		//// TODO: change back to editor key bindings
		auto a = static_cast<osgViewer::GraphicsWindow *>(_viewer->getCamera()->getGraphicsContext());
		a->setCursor(osgViewer::GraphicsWindow::LeftArrowCursor);
	}
}

void Core::setAllowChangeEditorProjection(bool tf)
{
	_allowEditorChangeProjection = tf;
	if (tf)
	{
		_viewer->getCamera()->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
	}
	else
	{
		_viewer->getCamera()->setComputeNearFarMode(osg::CullSettings::COMPUTE_NEAR_FAR_USING_BOUNDING_VOLUMES);
	}
}

bool Core::allowChangeEditorProjection()
{
	return _allowEditorChangeProjection;
}

void Core::addEventHandler(osgGA::GUIEventHandler *handler)
{
	if (_viewer == NULL) return;
	_viewer->addEventHandler(handler);
}

bool Core::isInGameMode()
{
	return _gameMode ? true : false;
}

void Core::configAxisVisualizer()
{
	_axisVisualizer.setPosition(osg::Vec3());
	_passes->addChild(_axisVisualizer.getRoot());
}

osg::ref_ptr<osgFX::EffectCompositor> Core::_passes;
osg::ref_ptr<osg::Group> Core::_sceneRoot;
osg::ref_ptr<osg::Group> Core::_geomRoot;
osg::ref_ptr<osgViewer::Viewer> Core::_viewer;
osg::Vec2 Core::_screenSize;
osg::Vec2 Core::_renderResolution;
osg::Vec2 Core::_winPos;
World Core::_world;
bool Core::_hasInit = false;

osg::ref_ptr<osgGA::CameraManipulator> Core::_camManipulatorTemp = NULL;
bool Core::_hasEnvMap = false;

Camera Core::_cam;
Camera Core::_savedManipulatorCam;

osg::ref_ptr<TwGUIManager> Core::_gui;
osg::ref_ptr<SkyBox> Core::_skybox;
std::string Core::_mediaPath;
osg::ref_ptr<CompositorAnalysis> Core::_analysisHUD;

bool Core::_gameMode;
bool Core::_passDataDisplay;
bool Core::_guiEnabled;
bool Core::_manipulatorEnabled;

bool Core::_isFirstFrame;
bool Core::_allowEditorChangeProjection;

osg::Timer_t Core::_lastFrameStartTime; 
osg::Timer_t Core::_frameStartTime; 

AxisVisualizer Core::_axisVisualizer;