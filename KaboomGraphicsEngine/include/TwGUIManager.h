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
#include <fstream>

const std::string GEOM_GROUP_NAME = "Edit GeometryObject";
const std::string LIGHT_GROUP_NAME = "Edit Light";
const std::string MATERIAL_GROUP_NAME = "Edit Material";

const std::string POS_X_LABEL = "posX";
const std::string POS_Y_LABEL = "posY";
const std::string POS_Z_LABEL = "posZ";

const std::string SCALE_X_LABEL = "scaleX";
const std::string SCALE_Y_LABEL = "scaleY";
const std::string SCALE_Z_LABEL = "scaleZ";

const std::string ROT_X_LABEL = "rotX";
const std::string ROT_Y_LABEL = "rotY";
const std::string ROT_Z_LABEL = "rotZ";

const std::string COLOR_LABEL = "color";
const std::string ROUGHNESS_LABEL = "roughness";
const std::string SPECULAR_LABEL = "specular";
const std::string METALLIC_LABEL = "metallic";

const std::string REMOVE_LABEL = "Remove";
const std::string EDIT_NAME_LABEL = "Edit name";

struct BarItem {
	TwBar* bar;
	std::string name;
};

struct ModelMatrix {
	std::string name;
	osg::Matrix matrix;
};

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

	// deprecated. Let gui handle this var itself
	//inline void setCameraManipulatorActive(bool tf)
	//{
	//	_cameraManipulatorActive = tf;
	//}

	inline int getManipulatorBits()
	{
		return _manipulatorBits;
	}

	inline bool isMouseOver()
	{
		return _isMouseOver;
	}

	// NOTE: this is different from the actual cam manipulator status in Core
	inline bool isCamControlOn()
	{
		return _cameraManipulatorActive;
	}

	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
	virtual void operator()(osg::RenderInfo& renderInfo) const;
	
	void requestFreeze();
	void requestUnFreeze();

	static void exportXML();
	static void addModelToGUI(TwBar* bar, GeometryObject* geom, std::string group, int& index);
	static void addLightToGUI(TwBar* bar, Light* geom, std::string group, int& index);
	static void addMaterialToGUI(TwBar* bar, Material* mat, std::string group, int& index);

	static ModelMatrix* makeModelMatrix(GeometryObject* geom);
	static void addGeomToUndo(GeometryObject* geom);
	static void clearRedoList();

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
		g_addBar = NULL;
		g_materialBar = NULL;

		for (int i = 0; i < _undos.size(); i++) {
			delete _undos[i];
		}

		for (int i = 0; i < _redos.size(); i++) {
			delete _redos[i];
		}
	}

	TwMouseButtonID getTwButton(int button) const;
	int getTwKey(int key, bool useCtrl) const;
	int getTwModKeyMask(int modkey) const;


	void initMainBar();
	void initManipuatorSelectorBar();
	void initAddBar();
	void initMaterialBar();

	static void doUndoRedo(std::vector<ModelMatrix*> &from, std::vector<ModelMatrix*> &dest);

	std::queue< osg::ref_ptr<const osgGA::GUIEventAdapter> > _eventsToHandle;
	TwBar *g_twBar;
	TwBar *g_manipulatorSelectorBar;
	TwBar *g_addBar;
	TwBar *g_materialBar;
	char g_fileName[256]; // TODO: this is a hack, fix it

	static bool openFile(std::string &s);
	static std::string getFileName(std::string s);
	static void strToWCchar(wchar_t* w, std::string s);

	static void write(std::ofstream &f, int tabs, std::string s);

	static std::string addTags(std::string tag, std::string s);
	static std::string tagify(std::string tag, std::string s);
	static std::string tagify(std::string tag, float f);
	static std::string tagify(std::string tag, bool b);
	static std::string tagify(std::string tag, osg::Vec3 &v);
	static std::string tagify(std::string tag, osg::Vec4 &v);
	static std::string tagify(std::string tag, osg::Quat &v);

	LightManager *_lm;
	GeometryObjectManager *_gm;
	MaterialManager *_mm;

	bool _freezeGUI;
	bool _cameraManipulatorActive;
	int _manipulatorBits;

	std::string nameToCopy;
	static int _index;

	static std::vector<ModelMatrix*> _undos;
	static std::vector<ModelMatrix*> _redos;
	static ModelMatrix* _currChange;

	bool _isMouseOver;
};