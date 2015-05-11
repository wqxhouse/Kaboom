#include "stdafx.h" 

#include "Core.h"

#include <osg/Vec3>
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>
#include <osg/TexGen>
#include <osg/Depth>
#include <osgViewer/GraphicsWindow>

#include "TypeIdLoader.h"
#include "MaterialLoader.h"
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

	// let configLibRocketGUI create the instance
	_libRocketInGameGUI = NULL;
	_libRocketEditorGUI = NULL;
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

	// need to do it before finalize(), since client needs this before running
	configLibRocketGUI();

	_sceneRoot->addChild(_passes);
	_hasInit = true;
}

void Core::loadMaterialFile(const std::string &filePath)
{
	MaterialLoader loader = MaterialLoader(_world.getMaterialManager());
	loader.loadXMLFile(filePath);
}

void Core::loadTypeIdFile(const std::string &filePath)
{
	TypeIdLoader loader;
	loader.loadXMLFile(filePath);
}

void Core::loadWorldFile(const std::string &worldFilePath)
{
	_world.loadXMLFile(worldFilePath);
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

	_passes->getOrCreateStateSet()->setMode(GL_TEXTURE_CUBE_MAP_SEAMLESS, osg::StateAttribute::ON);
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
			setUpdateCallback(new LightPassCallback(&_cubemapPreFilter));
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
	osg::TextureCubeMap *cubemap = _skybox->getCubeMap();

	// Get prefilter pass from _passes
	//osgFX::EffectCompositor::PassData passData;
	//_passes->getPassData("PreFilterCubeMapPass", passData);
	//osg::Camera *cam = passData.pass;
	//_cubemapPreFilter = CubeMapPreFilter(cam); // is there an elegant way?

	_cubemapPreFilter.initWithCubeMap(cubemap);
	osgFX::EffectCompositor::PassData lightPass;
	_passes->getPassData("LightPass", lightPass);
	osg::Camera *cam = lightPass.pass;
	cam->getOrCreateStateSet()->addUniform(new osg::Uniform("u_cubeMapTex", 4));
	cam->getOrCreateStateSet()->setTextureAttributeAndModes(4, _cubemapPreFilter.getGeneratedSpecularCubemap());
	cam->getOrCreateStateSet()->addUniform(new osg::Uniform("u_cubeMapDiffuseTex", 5));
	cam->getOrCreateStateSet()->setTextureAttributeAndModes(5, _cubemapPreFilter.getGeneratedDiffuseCubemap());

	_passes->addChild(_cubemapPreFilter.getRoot());
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

	osg::Matrix defaultProjection;
	
	float fovy = Camera::fovXToY(90, _screenSize);
	defaultProjection.makePerspective(fovy, _screenSize.x() / _screenSize.y(), 1, 500);
	_viewer->getCamera()->setProjectionMatrix(defaultProjection);

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

		if (_requestPrefilterCubeMap)
		{
			_cubemapPreFilter.enableCompute();
			_requestPrefilterCubeMap = false;
		}
		_viewer->frame();

		// _cubemapPreFilter.saveImagesToFile("C:\\3DEngine\\temp");
		_cubemapPreFilter.disableCompute();
		_passes->setPassActivated("SpecularLutPass", false);

		// handle mouse over of libRocket
		// TODO: is it proper to place it here???
		freezeCameraOnGUIDemand();

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

void Core::freezeCameraOnGUIDemand()
{
	if (_gui->isMouseOver() || _libRocketEditorGUI->isMouseOver())
	{
		disableCameraManipulator();
	}
	else
	{
		// preserve the GUI cam lock setting
		if (_gui->isCamControlOn())
		{
			enableCameraManipulator();
		}
	}
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
	if (!_hasEnvMap)
	{
		OSG_WARN << "Must setup a cubemap, loading default cubemap" << std::endl;
		setEnvironmentMapVerticalCross("DefaultAssets/Cubemap/Sky.hdr");
		configSkyBox();
		configCubemapPrefilterPass();
		configSpecularIBLLutPass();
		_hasEnvMap = true;
	}

	configAxisVisualizer();
	configLightVisualizer();
	configGeometryObjectManipulator();

	_analysisHUD = configureViewerForMode(*_viewer, _passes, NULL, 1);
	_analysisHUD->toggleHelper(); // disabled by default

	//_viewer->run();
	CustomFirstPersonManipulator *fpsManipulator = new CustomFirstPersonManipulator;
	fpsManipulator->setHomePosition(osg::Vec3(0, 0, 10), osg::Vec3(0, 1, 10), osg::Vec3(0, 0, 1));
	fpsManipulator->setAllowThrow(false);
	_viewer->setCameraManipulator(fpsManipulator);
	_viewer->getCamera()->setUpdateCallback(new MainCameraCallback);
	_currCamManipulatorType = Core::FIRSTPERSON;

	// TODO: keep it single threaded for now.
	// I just notice our Core and all shared static classes are not thread-safe
	// so that it will result in crashes or unexpected behaviors, such as dead-lock of a cameramanipulator enable/disable;
	_viewer->setThreadingModel(osgViewer::ViewerBase::ThreadingModel::SingleThreaded);
	_viewer->realize();
}

Camera &Core::getMainCamera()
{
	return _cam;
}

// Used in GUI focus
void Core::disableCameraManipulator()
{
	// std::cout << "disableCam" << std::endl;
	if (_viewer->getCameraManipulator() != NULL)
	{
		_camManipulatorTemp = _viewer->getCameraManipulator();
		_savedManipulatorCam = _cam;
		// std::cout << _savedManipulatorCam.getEyePosition() << std::endl;
		_viewer->setCameraManipulator(NULL);
	}
}

void Core::enableCameraManipulator()
{
	// std::cout << "enableCam" << std::endl;
	if (_camManipulatorTemp == NULL) return;
	// std::cout << _savedManipulatorCam.getEyePosition() << std::endl;

	_camManipulatorTemp->setHomePosition(_savedManipulatorCam.getEyePosition(),
		_savedManipulatorCam.getLookAt(), _savedManipulatorCam.getUp());
	_viewer->getCamera()->setProjectionMatrix(_savedManipulatorCam.getProjectionMatrix());
	_viewer->setCameraManipulator(_camManipulatorTemp);

	_savedManipulatorCam = Camera();
	_camManipulatorTemp = NULL;

	// TODO: test if this will crash the program!
	GeometryObjectManipulator::detachManipulator();
}

void Core::switchToFirstPersonCamManipulator()
{
	CustomFirstPersonManipulator *firstPersonCam = new CustomFirstPersonManipulator;
	firstPersonCam->setAllowThrow(false);
	firstPersonCam->setHomePosition(_cam.getEyePosition(),
		_cam.getLookAt(), osg::Vec3(0, 0, 1));
	_viewer->setCameraManipulator(firstPersonCam);
	_currCamManipulatorType = Core::FIRSTPERSON;
}

void Core::switchToTrackBallCamManipulator()
{
	osgGA::TrackballManipulator *trackBallCam = new osgGA::TrackballManipulator;
	trackBallCam->setHomePosition(_cam.getEyePosition(),
		_cam.getLookAt(), _cam.getUp());
	_viewer->setCameraManipulator(trackBallCam);
	_currCamManipulatorType = Core::TRACKBALL;
}

void Core::setCurrentCameraManipulatorHomePosition(const osg::Vec3 &eye, const osg::Vec3 &lookAt, const osg::Vec3 &up)
{
	osgGA::CameraManipulator *cam;
	if ((cam = _viewer->getCameraManipulator()) != NULL)
	{
		cam->setHomePosition(eye, lookAt, up);
	}
	else
	{
		// temporoary enable it without detaching the node
		_camManipulatorTemp->setHomePosition(eye, lookAt, up);
		_viewer->getCamera()->setProjectionMatrix(_savedManipulatorCam.getProjectionMatrix());
		_viewer->setCameraManipulator(_camManipulatorTemp);

		_savedManipulatorCam = Camera();
		_camManipulatorTemp = NULL;

		// disable after changing the position
		disableCameraManipulator();
	}
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

void Core::enableLightVisualizer()
{
	bool enabled = _passes->containsNode(_world.getLightManager()->getVisualizerRoot());
	if (enabled) return;

	_passes->addChild(_world.getLightManager()->getVisualizerRoot());
}

void Core::disableLightVisualizer()
{
	bool disabled = !_passes->containsNode(_world.getLightManager()->getVisualizerRoot());
	if (disabled) return;

	_passes->removeChild(_world.getLightManager()->getVisualizerRoot());
}

void Core::configSkyBox()
{
	_skybox->setNodeMask(0x2);
	_skybox->setGeomRoot(_geomRoot);
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

void Core::setEnvironmentMapVerticalCross(const std::string &cubemap)
{
	CubemapUtil ut;
	bool res = ut.loadVerticalCross(cubemap);
	// ut.saveImageToFile("C:\\3DEngine\\temp\\cubemap");
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

void Core::enableTwGUI()
{
	if (!_guiEnabled && !_gameMode)
	{
		_viewer->addEventHandler(_gui.get());
		_viewer->getCamera()->setFinalDrawCallback(_gui.get());
		_guiEnabled = true;
	}
}

void Core::disableTwGUI()
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
		disableTwGUI();
		disableGeometryObjectManipulator();
		disableLightVisualizer();

		_libRocketEditorGUI->disableGUI();
		_libRocketInGameGUI->enableGUI();

		LibRocketGUIManager::bindDebugWindow(_libRocketInGameGUI);

		auto a = static_cast<osgViewer::GraphicsWindow *>(_viewer->getCamera()->getGraphicsContext());
		a->setCursor(osgViewer::GraphicsWindow::NoCursor);
	}
}

void Core::disableGameMode()
{
	if (_gameMode)
	{
		_gameMode = false;

		enableTwGUI();
		enableCameraManipulator();
		enableGeometryObjectManipulator();
		enableLightVisualizer();

		_libRocketEditorGUI->enableGUI();
		_libRocketInGameGUI->disableGUI();
		LibRocketGUIManager::bindDebugWindow(_libRocketEditorGUI);

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
	_axisVisualizer.init();
	_axisVisualizer.setPosition(osg::Vec3());
	_passes->addChild(_axisVisualizer.getRoot());
}


void Core::configLightVisualizer()
{
	_passes->addChild(_world.getLightManager()->getVisualizerRoot());
}

void Core::requestPrefilterCubeMap()
{
	_requestPrefilterCubeMap = true;

	// TODO: add support of different cubemaps
	_cubemapPreFilter.changeCubeMap(_skybox->getCubeMap());
}

void Core::requestPrefilterCubeMapWithCubeMap(osg::TextureCubeMap *cubemap)
{
	_requestPrefilterCubeMap = true;
	_cubemapPreFilter.changeCubeMap(cubemap);
}

void Core::configSpecularIBLLutPass()
{
	osgFX::EffectCompositor::PassData pd;
	_passes->getPassData("SpecularLutPass", pd);
	osg::ref_ptr<osg::Camera> cam = pd.pass;
	auto buffer = (*cam->getBufferAttachmentMap().begin()).second._texture;
	osg::ref_ptr<osg::Texture> lutTex = static_cast<osg::Texture *>(buffer);

	// TODO: since same across different cubemaps
	// consider save it to hard drive 
	// feed the lut_texture to lightpass

	osgFX::EffectCompositor::PassData lightPass;
	_passes->getPassData("LightPass", lightPass);
	osg::ref_ptr<osg::Camera> lightPassCam = lightPass.pass;
	lightPassCam->getOrCreateStateSet()->addUniform(new osg::Uniform("u_lutTex", 6));
	lightPassCam->getOrCreateStateSet()->setTextureAttributeAndModes(6, lutTex);
}

void Core::configLibRocketGUI()
{
	osgViewer::ViewerBase::Views views;
	_viewer->getViews(views);
	osg::GraphicsContext* gc = _viewer->getCamera()->getGraphicsContext();

	std::string guiPath = _mediaPath + "DefaultAssets\\LibRocketGUI\\";
	_libRocketEditorGUI = new LibRocketGUIManager(guiPath, gc);
	_libRocketInGameGUI = new LibRocketGUIManager(guiPath, gc);
	_libRocketInGameGUI->disableGUI();
	
	// Create Editor GUI 
	std::string testWindowPath = guiPath + "InEditor\\setNameWindow.rml";
	_libRocketEditorGUI->addWindow(testWindowPath, true);

	_viewer->addEventHandler(_libRocketEditorGUI);
	_sceneRoot->addChild(_libRocketEditorGUI->getRoot());
	_viewer->addEventHandler(_libRocketInGameGUI);
	_sceneRoot->addChild(_libRocketInGameGUI->getRoot());
}

bool Core::isCamLocked()
{
	// TODO: this is a hack
	return _camManipulatorTemp != NULL ? true : false;
}

enum Core::CamManipulatorType Core::getCurrCamManipulatorType()
{
	return _currCamManipulatorType;
}

void Core::setEditorFPSCamWalkingSpeed(float metersPerSec)
{
	if (_currCamManipulatorType == FIRSTPERSON && _viewer->getCameraManipulator() != NULL)
	{
		CustomFirstPersonManipulator *m = static_cast<CustomFirstPersonManipulator *>(_viewer->getCameraManipulator());
		m->setWalkingSpeed(metersPerSec);
	}
	else
	{
		if (_camManipulatorTemp == NULL) return;
		CustomFirstPersonManipulator *m = static_cast<CustomFirstPersonManipulator *>(_camManipulatorTemp.get());
		m->setWalkingSpeed(metersPerSec);
	}
}

float Core::getEditorFPSCamWalkingSpeed()
{
	if (_currCamManipulatorType == FIRSTPERSON && _viewer->getCameraManipulator() != NULL)
	{
		CustomFirstPersonManipulator *m = static_cast<CustomFirstPersonManipulator *>(_viewer->getCameraManipulator());
		return m->getWalkingSpeed();
	}
	else
	{ 
		if (_camManipulatorTemp == NULL) return 0.0;
		CustomFirstPersonManipulator *m = static_cast<CustomFirstPersonManipulator *>(_camManipulatorTemp.get());
		return m->getWalkingSpeed();
	}
}


void Core::enableLibRocketInEditorGUI()
{
	if (_libRocketEditorGUI == NULL) return;
	_libRocketEditorGUI->enableGUI();
}

void Core::disableLibRocketInEditorGUI()
{
	if (_libRocketEditorGUI == NULL) return;
	_libRocketEditorGUI->disableGUI();
}

bool Core::isLibRocketInEditorGUIEnabled()
{
	if (_libRocketEditorGUI == NULL) return false;
	return _libRocketEditorGUI->isGUIEnabled();
}


osg::ref_ptr<LibRocketGUIManager> Core::getInGameLibRocketGUIManager()
{
	return _libRocketInGameGUI;
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
osg::ref_ptr<LibRocketGUIManager> Core::_libRocketEditorGUI;
osg::ref_ptr<LibRocketGUIManager> Core::_libRocketInGameGUI;

osg::ref_ptr<SkyBox> Core::_skybox;
std::string Core::_mediaPath;
osg::ref_ptr<CompositorAnalysis> Core::_analysisHUD;

bool Core::_gameMode;
bool Core::_passDataDisplay;
bool Core::_guiEnabled;
bool Core::_manipulatorEnabled;

bool Core::_isFirstFrame;
bool Core::_allowEditorChangeProjection;
bool Core::_requestPrefilterCubeMap = false;

osg::Timer_t Core::_lastFrameStartTime;
osg::Timer_t Core::_frameStartTime;

AxisVisualizer Core::_axisVisualizer;

CubeMapPreFilter Core::_cubemapPreFilter;

enum Core::CamManipulatorType Core::_currCamManipulatorType;
