#pragma once

#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <osgGA/TrackballManipulator>
#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/AnimationPath>
#include <osg/PolygonMode>
#include <osg/PolygonOffset>

#include <osgbCollision/GLDebugDrawer.h>
#include <util/ConfigSettings.h>

//#include <btBulletDynamicsCommon.h>
//
//#include <osg/io_utils>
//#include <iostream>
//
//#include <osgbDynamics/MotionState.h>
//#include <osgbCollision/CollisionShapes.h>
//#include <osgbCollision/RefBulletObject.h>
//#include <osgbDynamics/RigidBodyAnimation.h>


class OsgBulletDebugViewer {

public:
	OsgBulletDebugViewer(ConfigSettings *configSettings);
	~OsgBulletDebugViewer();

	void init();
	void renderFrame();

	void addNode(osg::Node* );

	void addNodeWireFrame(osg::Node* );

	osgbCollision::GLDebugDrawer* getDbgDraw();
private:
	ConfigSettings* config;

	osgViewer::Viewer viewer;
	osgGA::TrackballManipulator * tb;

	osg::ref_ptr<osg::Group> root;

	osgbCollision::GLDebugDrawer* dbgDraw;
};