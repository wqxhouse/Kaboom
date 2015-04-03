#include <Windows.h>

#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>

#include "KeyboardEventHandler.h"

int main() {
    osgViewer::Viewer viewer;

	osg::ref_ptr<osg::Shape> player(new osg::Box);
	osg::ref_ptr<osg::ShapeDrawable> playerDrawable(new osg::ShapeDrawable(player));

	osg::ref_ptr<osg::Geode> playerNode(new osg::Geode);
	playerNode->addDrawable(playerDrawable);

	viewer.setSceneData(playerNode);
	viewer.setCameraManipulator(new osgGA::TrackballManipulator);
	viewer.addEventHandler(new KeyboardEventHandler);

    viewer.realize();

    while (!viewer.done()) {
        viewer.frame();
    }
}
