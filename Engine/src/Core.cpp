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

#include "World.h"
#include "GeometryObjectManager.h"
#include "LightManager.h"
#include "LightPrePassCallback.h"

// TODO: log which one called the global functions in Core
// for debugging

extern void configureViewerForMode(osgViewer::Viewer& viewer, osgFX::EffectCompositor* compositor,
		osg::Node* model, int displayMode);

void Core::init(int winWidth, int winHeight, int resolutionWidth, int resolutionHeight)
{
	_screenSize = osg::Vec2(winWidth, winHeight);

	// TODO: test separation of screenSize and the renderResolution
	_renderResolution = osg::Vec2(resolutionWidth, resolutionHeight);
	_sceneRoot = new osg::Group;
	_gui = new TwGUIManager;

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

	std::string rootPath = "C:\\3DEngine\\Tempo\\Media\\";

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

	_viewer->setUpViewInWindow(0, 0, _screenSize.x(), _screenSize.y());
}

void Core::run()
{
	if (!_hasInit)
	{
		return;
	}
	
	// TODO: add console output : initializing GUI
	_gui->initializeTwGUI();

	// TODO: add switches using keyboard
	configureViewerForMode(*_viewer, _passes, NULL, 1);
	//_viewer->setThreadingModel(osgViewer::ViewerBase::ThreadingModel::SingleThreaded);
	_viewer->run();
}

const Camera &Core::getMainCamera()
{
	return _cam;
}

// Used in GUI focus
void Core::disableCameraManipulator()
{
	_camManipulatorTemp = _viewer->getCameraManipulator();
	_viewer->setCameraManipulator(NULL);
}

void Core::enableCameraManipulator()
{
	_viewer->setCameraManipulator(_camManipulatorTemp);
	_camManipulatorTemp = NULL;
}

osg::ref_ptr<osgFX::EffectCompositor> Core::_passes;
osg::ref_ptr<osg::Group> Core::_sceneRoot;
osg::ref_ptr<osg::Group> Core::_geomRoot;
osg::ref_ptr<osgViewer::Viewer> Core::_viewer;
osg::Vec2 Core::_screenSize;
osg::Vec2 Core::_renderResolution;
World Core::_world;
bool Core::_hasInit = false;

osg::ref_ptr<osgGA::CameraManipulator> Core::_camManipulatorTemp = NULL;

Camera Core::_cam;
osg::ref_ptr<TwGUIManager> Core::_gui;
