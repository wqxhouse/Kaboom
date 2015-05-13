#pragma once
#include <osgManipulator/TranslateAxisDragger>
#include <osgManipulator/TabBoxDragger>
#include <osgManipulator/TabBoxTrackballDragger>
#include <osgManipulator/TabPlaneDragger>
#include <osgManipulator/TabPlaneTrackballDragger>
#include <osgManipulator/Scale1DDragger>
#include <osgManipulator/Scale2DDragger>
#include <osgManipulator/TrackballDragger>
#include <osgManipulator/Translate1DDragger>
#include <osgManipulator/Translate2DDragger>
//#include <osgManipulator/TranslateAxisDragger>
#include "CustomTranslateAxisDragger.h"
#include <osgManipulator/TranslatePlaneDragger>
#include <osgManipulator/RotateCylinderDragger>

#include <osgManipulator/Selection>
#include <osg/MatrixTransform>
#include <osg/Depth>
#include "DraggerUpdateCallback.h"
#include "Light.h"

enum ManipulatorType
{
	TrackballDragger,
	TranslateAxisDragger,
	TabBoxDragger,
	NONE,
};

class GeometryObjectManipulator
{
public:
	static void initWithRootNode(osg::Group *root);
	static void assignManipulatorToGeometryTransformNode(osg::MatrixTransform *node, enum ManipulatorType type);
	static void changeCurrentManipulatorType(enum ManipulatorType type);
	static void changeCurrentNode(osg::MatrixTransform *node);

	static void attachTransformNode(osg::MatrixTransform *node);
	static void attachLight(Light *light);

	static void detachManipulator();
	static enum ManipulatorType getCurrentManipulatorType();

	static osg::ref_ptr<osg::MatrixTransform> getCurrNode();
	static void updateBoundingBox();

	static bool setVisible(bool tf);
	static bool isVisible();
	static bool isAttchingLight();
	static Light *getAttachedLight();

private:
	static osg::ref_ptr<osgManipulator::TrackballDragger> _trackBallDragger;
	static osg::ref_ptr<osgManipulator::CustomTranslateAxisDragger> _translateAxisDragger;
	static osg::ref_ptr<osgManipulator::TabBoxDragger> _tabBoxDragger;

	static osg::observer_ptr<osg::MatrixTransform> _currNode;
	static osg::observer_ptr<osgManipulator::Dragger> _dragger;
	static enum ManipulatorType _currType;

	static osg::observer_ptr<osg::Group> _rootNode;
	static osg::ref_ptr<osg::Depth> _depth;

	static DraggerUpdateCallback _draggerCB;

	static osg::ref_ptr<osg::MatrixTransform> _lightManipulatorGhostObject;
	static bool _attachingLight;
	static Light *_attachedLight;
};