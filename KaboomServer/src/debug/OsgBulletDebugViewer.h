#pragma once

#include <osgbCollision/GLDebugDrawer.h>
#include <osgGA/TrackballManipulator>
#include <osgViewer/Viewer>

class ConfigSettings;

class OsgBulletDebugViewer {

public:
    OsgBulletDebugViewer(ConfigSettings *configSettings);
    ~OsgBulletDebugViewer();

    void init();
    void renderFrame();

    void addNode(osg::Node*);

    void addNodeWireFrame(osg::Node*);

    osgbCollision::GLDebugDrawer* getDbgDraw();

private:
    ConfigSettings* config;

    osgViewer::Viewer viewer;
    osgGA::TrackballManipulator * tb;

    osg::ref_ptr<osg::Group> root;

    osgbCollision::GLDebugDrawer* dbgDraw;
};