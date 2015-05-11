#pragma once
#include <vector>
#include <osgGA/GUIEventHandler>
#include <osgLibRocket/GuiNode.h>

class LibRocketGUIManager : public osgGA::GUIEventHandler
{
public:
	LibRocketGUIManager(const std::string &guiResourcePath, osg::GraphicsContext *gc);
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object *obj, osg::NodeVisitor *nv);

	bool isMouseOver();
	bool valid();

	int addWindow(const std::string &rmlPath, bool show);
	Rocket::Core::Context *getContext();
	Rocket::Core::ElementDocument *getWindow(int id);

	inline osg::ref_ptr<osg::Camera> getRoot()
	{
		return _guiDisplayRoot;
	}

	void enableGUI();
	void disableGUI();

	inline bool isGUIEnabled()
	{
		return _isGUIEnabled;
	}

	static void bindDebugWindow(LibRocketGUIManager *manager);

private:
	osg::ref_ptr<osgLibRocket::GuiNode> _gui;
	osg::ref_ptr<osg::Camera> _guiDisplayRoot;
	std::string _guiPath;

	std::vector<Rocket::Core::ElementDocument *> _windows;

	bool _isGUIEnabled;
	static bool _rocketLibIntialized;
	static int _guiCounter;
};