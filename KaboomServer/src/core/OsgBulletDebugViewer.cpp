#include "OsgBulletDebugViewer.h"

#include <osgViewer/ViewerEventHandlers>

OsgBulletDebugViewer::OsgBulletDebugViewer(ConfigSettings * configSettings)
	: config(configSettings){

}

OsgBulletDebugViewer::~OsgBulletDebugViewer() {

}

void OsgBulletDebugViewer::init() {
	int screenPosXStr, screenPosYStr, screenWStr, screenHStr;

	config->getValue(ConfigSettings::str_screenPosX, screenPosXStr);
	config->getValue(ConfigSettings::str_screenPosY, screenPosYStr);
	config->getValue(ConfigSettings::str_screen_width, screenWStr);
	config->getValue(ConfigSettings::str_screen_height, screenHStr);


	viewer.setUpViewInWindow(screenPosXStr, screenPosYStr, screenWStr, screenHStr);

	tb = new osgGA::TrackballManipulator();
	tb->setHomePosition(osg::Vec3(5, -12, 12),
						osg::Vec3(-7, 0, -10),
						osg::Vec3(0, 0, 1));

	viewer.setCameraManipulator(tb);

	root = new osg::Group;

	viewer.setSceneData(root.get());
	viewer.addEventHandler(new osgViewer::StatsHandler);

	//dbgDraw = new osgbCollision::GLDebugDrawer();
	//dbgDraw->setDebugMode();
	//root->addChild(dbgDraw->getSceneGraph());
	dbgDraw = new osgbCollision::GLDebugDrawer();
	dbgDraw->setDebugMode(~btIDebugDraw::DBG_NoDebug);
	root->addChild(dbgDraw->getSceneGraph());

}

void OsgBulletDebugViewer::addNode(osg::Node* node) {
	root->addChild(node);
}

void OsgBulletDebugViewer::addNodeWireFrame(osg::Node * node) {

	osg::StateSet* state = node->getOrCreateStateSet();
	osg::PolygonMode* pm = new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
	state->setAttributeAndModes(pm);
	osg::PolygonOffset* po = new osg::PolygonOffset(-1, -1);
	state->setAttributeAndModes(po);
	state->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	root->addChild(node);
}

void OsgBulletDebugViewer::renderFrame() {
	viewer.frame();
}

osgbCollision::GLDebugDrawer* OsgBulletDebugViewer::getDbgDraw(){
	return dbgDraw;
}