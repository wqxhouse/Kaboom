#pragma once

#include <osg/Texture2D>
#include <osg/MatrixTransform>
#include <osgDB/ReadFile>
#include <osgGA/StateSetManipulator>
#include <osgGA/TrackballManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/Viewer>
#include <queue>

#include <AntTweakBar.h>

#include "GeometryObjectManager.h"
#include "LightManager.h"
#include "MaterialManager.h"

class Core;
class TwGUIManager : public osgGA::GUIEventHandler, public osg::Camera::DrawCallback
{
public:
	TwGUIManager();
	TwGUIManager(const TwGUIManager& copy, const osg::CopyOp& op = osg::CopyOp::SHALLOW_COPY)
		: _eventsToHandle(copy._eventsToHandle) {}

	META_Object(osg, TwGUIManager)

	static void TW_CALL loadModelFunc(void* clientData);

	// call it inside Core::run();
	void initializeTwGUI();

	void updateEvents() const;

	inline void setCameraManipulatorActive(bool tf)
	{
		_cameraManipulatorActive = tf;
	}

	inline int getManipulatorBits()
	{
		return _manipulatorBits;
	}

	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
	virtual void operator()(osg::RenderInfo& renderInfo) const;

protected:
	struct Position
	{
		float x;
		float y;
		float z;
	};

	virtual ~TwGUIManager()
	{
		// TODO: handle ESC key, currently crash the 
		// program, since managers gets deallocated 
		// before TwGUIManager
		TwDeleteAllBars();
		TwTerminate();
		g_twBar = NULL;
		g_manipulatorSelectorBar = NULL;
	}

	TwMouseButtonID getTwButton(int button) const;
	int getTwKey(int key, bool useCtrl) const;
	int getTwModKeyMask(int modkey) const;

	void initMainBar();
	void initManipuatorSelectorBar();
	
	std::queue< osg::ref_ptr<const osgGA::GUIEventAdapter> > _eventsToHandle;
	TwBar *g_twBar;
	TwBar *g_manipulatorSelectorBar;
	char g_fileName[256]; // TODO: this is a hack, fix it

	LightManager *_lm;
	GeometryObjectManager *_gm;
	MaterialManager *_mm;

	bool _cameraManipulatorActive;
	int _manipulatorBits;

};