#include "stdafx.h" 

#include "Core.h"

#ifdef _WIN32
#include <windows.h>
#else
#incldue <unistd.h>
#endif

#include <osg/Vec3>
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>
#include <osg/TexGen>
#include <osg/Depth>

#include "World.h"
#include "GeometryObjectManager.h"
#include "LightManager.h"
#include "LightPrePassCallback.h"
#include "LightPassCallback.h"
#include "CustomFirstPersonManipulator.h"
#include "GeometryPicker.h"
#include "GeometryObjectManipulator.h"

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
	_viewer->getCamera()->setFinalDrawCallback(_gui.get());

	_viewer->setUpViewInWindow(_winPos.x(), _winPos.y(), _screenSize.x(), _screenSize.y());
	_viewer->setKeyEventSetsDone(0);

	_viewer->addEventHandler(new GeometryPicker);
}

void Core::run()
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

	configGeometryObjectManipulator();

	_analysisHUD = configureViewerForMode(*_viewer, _passes, NULL, 1);
	_analysisHUD->toggleHelper(); // disabled by default

	//_viewer->run();
	if (_camCallback == NULL)
	{ 
		_viewer->setCameraManipulator(new osgGA::TrackballManipulator);
	}
	else
	{
		_viewer->getCamera()->setUpdateCallback(new MainCameraCallback);
	}

	_viewer->realize();
	while (!_viewer->done())
	{
		if (_camCallback != NULL)
		{
			_camCallback(&_cam);
		}

		_viewer->frame();
	}
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
	_viewer->setCameraManipulator(_camManipulatorTemp);

	_savedManipulatorCam = Camera();
	_camManipulatorTemp = NULL;

	_gui->setCameraManipulatorActive(true);
}

void Core::configSkyBox()
{
	_skybox->setNodeMask(0x2);
	_skybox->setGeomRoot(_geomRoot);
	//osg::Box *box = new osg::Box;
	//osg::ShapeDrawable *sd = new osg::ShapeDrawable;
	//sd->setShape(box);
	//osg::Geode *geode = new osg::Geode;
	//geode->addDrawable(sd);
	//_skybox->addChild(geode);
	_passes->addChild(_skybox);
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
		_gameMode = true;
	}
}

void Core::disableGameMode()
{
	if (_gameMode)
	{
		_gameMode = false;
		enableGUI();
		enableCameraManipulator();

		// TODO: change back to editor key bindings

		_gameMode = false;
	}
}

void Core::addEventHandler(osgGA::GUIEventHandler *handler)
{
	_viewer->addEventHandler(handler);
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
CameraCallback Core::_camCallback;
osg::ref_ptr<CompositorAnalysis> Core::_analysisHUD;

bool Core::_gameMode;
bool Core::_passDataDisplay;
bool Core::_guiEnabled;