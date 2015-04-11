#include "stdafx.h" 

#include "Scene.h"
#include "World.h"
#include "Core.h"
#include "GeometryObjectManager.h"
#include "LightManager.h"
#include "MaterialManager.h"

#include <osgDB/ReadFile>

void setupScene()
{
	World &w = Core::getWorldRef();
	GeometryObjectManager *g = w.getGeometryManager();
	MaterialManager *m = w.getMaterialManager();
	LightManager *l = w.getLightManager();

	osg::Node *loaded = osgDB::readNodeFile("cessna.osg");

	g->addGeometry("plane", loaded);
	m->createPlainMaterial("planeMat", osg::Vec3(0.7, 0.3, 0.3), 0.5, 0.7, 1.0);
	g->setGeometryMaterial("plane", m->getMaterial("planeMat"));
	l->addDirectionalLight("Sun", osg::Vec3(-1, -1, -1), osg::Vec3(0.3, 0.3, 0.7), false);

	osg::Vec3 colorRED = osg::Vec3(0.7, 0.3, 0.3);
	l->addPointLight("pt1", osg::Vec3(0, 0, 0), colorRED, 1, false);
	l->addPointLight("pt2", osg::Vec3(0, 0, 0), colorRED, 2, false);
}