#include "LibRocketGUIManager.h"

#include <osgLibRocket/FileInterface.h>
#include <osgLibRocket/GuiNode.h>
#include <osgLibRocket/SystemInterface.h>
#include <osgLibRocket/RenderInterface.h>

LibRocketGUIManager::LibRocketGUIManager(const std::string &guiResourcePath, osg::GraphicsContext *gc)
	: _guiPath(guiResourcePath)
{
	if (gc == NULL)
	{
		OSG_WARN << "NULL graphics context, something is wrong. Valid libRocket GUI is not constructed" << std::endl;
		return;
	}

	if (_rocketLibIntialized == false)
	{
		_guiCounter = 0;
		// create and set libRocket to OSG interfaces


		osgLibRocket::SystemInterface* system_interface = new osgLibRocket::SystemInterface();
		Rocket::Core::SetSystemInterface(system_interface);

		osgLibRocket::FileInterface* file_interface = new osgLibRocket::FileInterface();
		Rocket::Core::SetFileInterface(file_interface);

		Rocket::Core::Initialise();

		// load some fonts
		Rocket::Core::FontDatabase::LoadFontFace((_guiPath + "Resources\\Delicious-Roman.otf").c_str());
		Rocket::Core::FontDatabase::LoadFontFace((_guiPath + "Resources\\Delicious-Italic.otf").c_str());
		Rocket::Core::FontDatabase::LoadFontFace((_guiPath + "Resources\\Delicious-Bold.otf").c_str());
		Rocket::Core::FontDatabase::LoadFontFace((_guiPath + "Resources\\Delicious-BoldItalic.otf").c_str());
		Rocket::Core::FontDatabase::LoadFontFace((_guiPath + "Resources\\kinishinai.otf").c_str());
		Rocket::Core::FontDatabase::LoadFontFace((_guiPath + "Resources\\astropolis.otf").c_str());

		// TODO: add guard here for Rocket library init
	}

	std::string guiContextName = "libRocketGUI_" + std::to_string(_guiCounter++);
	_gui = new osgLibRocket::GuiNode(guiContextName);

	osg::ref_ptr<osg::Camera> cam = new osg::Camera();
	cam->setClearMask(GL_DEPTH_BUFFER_BIT);
	cam->setRenderOrder(osg::Camera::POST_RENDER, 100);
	cam->setAllowEventFocus(false);
	cam->setReferenceFrame(osg::Transform::ABSOLUTE_RF);

	// same graphics context as main camera
	cam->setGraphicsContext(gc);

	_gui->setCamera(cam);
	cam->addChild(_gui);

	_guiDisplayRoot = cam;
	_isGUIEnabled = true;

	if (_rocketLibIntialized == false && _gui != NULL)
	{
		// add error and other debug stuff to this gui. only one gui at a time may have the debug view
		Rocket::Debugger::Initialise(_gui->getContext());

		_rocketLibIntialized = true;
	}
}

void LibRocketGUIManager::bindDebugWindow(LibRocketGUIManager *manager)
{
	if (manager == NULL) return;
	Rocket::Debugger::SetContext(manager->getContext());
}

int LibRocketGUIManager::addWindow(const std::string &rmlPath, bool show)
{
	Rocket::Core::ElementDocument *window = _gui->getContext()->LoadDocument(rmlPath.c_str());
	if (!window) return -1;

	if (show)
	{
		window->Show();
	}

	// remove the initial reference as suggested in the official doc. 
	window->RemoveReference();
	_windows.push_back(window);
	return _windows.size() - 1;
}

Rocket::Core::ElementDocument *LibRocketGUIManager::getWindow(int id)
{
	if (id >= _windows.size())
	{
		OSG_WARN << "LibRocketGUIManager: Index out of bounds " << std::endl;
		return NULL;
	}
	return _windows[id];
}

bool LibRocketGUIManager::valid()
{
	return (_gui != NULL) && (_guiDisplayRoot != NULL) && (_rocketLibIntialized == true);
}

bool LibRocketGUIManager::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object *obj, osg::NodeVisitor *nv)
{
	// can put some extra handling logic here
	
	if (_isGUIEnabled)
	{
		auto eventhandler = _gui->GetGUIEventHandler();
		return eventhandler->handle(ea, aa, obj, nv);
	}
	return false;
}

Rocket::Core::Context *LibRocketGUIManager::getContext()
{
	return _gui->getContext();
}

bool LibRocketGUIManager::isMouseOver()
{
	return _gui->isMouseOver();
}

void LibRocketGUIManager::enableGUI()
{
	if (_isGUIEnabled == true) return;
	_guiDisplayRoot->addChild(_gui);
	_isGUIEnabled = true;
}

void LibRocketGUIManager::disableGUI()
{
	if (_isGUIEnabled == false) return;
	_guiDisplayRoot->removeChild(_gui);
	_isGUIEnabled = false;
}

bool LibRocketGUIManager::_rocketLibIntialized;
int LibRocketGUIManager::_guiCounter;
