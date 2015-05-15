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
#include "PointLight.h"
#include "MaterialManager.h"
#include <fstream>

const std::string GEOM_GROUP_NAME = "Edit GeometryObject";
const std::string LIGHT_GROUP_NAME = "Edit Light";
const std::string PLAIN_MATERIAL_GROUP_NAME = "Edit Plain Material";
const std::string TEXTURED_MATERIAL_GROUP_NAME = "Edit Textured Material";

const std::string POS_X_LABEL = "PosX";
const std::string POS_Y_LABEL = "PosY";
const std::string POS_Z_LABEL = "PosZ";

const std::string SCALE_X_LABEL = "ScaleX";
const std::string SCALE_Y_LABEL = "ScaleY";
const std::string SCALE_Z_LABEL = "ScaleZ";
const std::string SCALE_UNIFORM_LABEL = "Scale uniform";

const std::string ROT_X_LABEL = "RotX";
const std::string ROT_Y_LABEL = "RotY";
const std::string ROT_Z_LABEL = "RotZ";

const std::string COLOR_LABEL = "Color";
const std::string ROUGHNESS_LABEL = "Roughness";
const std::string SPECULAR_LABEL = "Specular";
const std::string METALLIC_LABEL = "Metallic";

const std::string ALBEDO_PATH_LABEL = "Albedo path";
const std::string ROUGHNESS_PATH_LABEL = "Roughness path";
const std::string METALLIC_PATH_LABEL = "Metallic path";
const std::string NORMAL_MAP_PATH_LABEL = "Normal map path";

const std::string REMOVE_LABEL = "Remove";
const std::string EDIT_NAME_LABEL = "Edit name";
const std::string INTENSITY_LABEL = "Intensity";
const std::string FIT_TO_SCREEN_LABEL = "Fit To Screen";

struct BarItem {
	TwBar* bar;
	std::string name;
};

// FOR UNDO/REDO
enum UndoRedoType { MODEL, LIGHT };

struct ModelInfo {
	std::string name;
	osg::Matrix matrix;
};

struct LightInfo {
	std::string name;
	osg::Vec3 posDir;
	osg::Vec3 color;
	float radius;
	bool shadow;
	float intensity;
};

union UndoRedoValue {
	struct {
		ModelInfo model;
	};
	struct {
		LightInfo light;
	};
};

struct UndoRedoNode {
	UndoRedoType type;
	UndoRedoValue value;
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
	static void addPlainMaterialToGUI(TwBar* bar, Material* mat, std::string group, int& index);
	static void addTexturedMaterialToGUI(TwBar* bar, Material* mat, std::string group, int& index);

	static UndoRedoNode* makeUndoRedoNode(GeometryObject* geom);
	static UndoRedoNode* makeUndoRedoNode(Light* light);
	static void addGeomToUndo(GeometryObject* geom);
	static void addLightToUndo(Light* light);
	static void clearRedoList();

	static UndoRedoNode* _currChange;

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
		g_plainMaterialBar = NULL;
		g_texturedMaterialBar = NULL;

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
	void initPlainMaterialBar();
	void initTexturedMaterialBar();
	void initLightBar();

	static void fitObjectToScreen(osg::MatrixTransform *mt);
	static void fitPointLightToScreen(PointLight *l);
	static void doUndoRedo(std::vector<UndoRedoNode*> &from, std::vector<UndoRedoNode*> &dest);

	std::queue< osg::ref_ptr<const osgGA::GUIEventAdapter> > _eventsToHandle;
	TwBar *g_twBar;
	TwBar *g_manipulatorSelectorBar;
	TwBar *g_addBar;
	TwBar *g_plainMaterialBar;
	TwBar *g_texturedMaterialBar;
	TwBar *g_lightBar;
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

	static void exportWorldXML(std::string &path);
	static void exportMaterialXML(std::string &path);

	LightManager *_lm;
	GeometryObjectManager *_gm;
	MaterialManager *_mm;

	bool _freezeGUI;
	bool _cameraManipulatorActive;
	int _manipulatorBits;

	std::string nameToCopy;
	static int _index;

	static std::vector<UndoRedoNode*> _undos;
	static std::vector<UndoRedoNode*> _redos;

	bool _isMouseOver;
	int _scrollPos;
};