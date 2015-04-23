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

	//std::string base = "Cerberus_by_Andrew_Maximov/Textures/";
	//osg::Node *loaded = osgDB::readNodeFile("Cerberus_by_Andrew_Maximov/Cerberus_LP.fbx");
	//g->addGeometry("gun", loaded);

	//m->createTextureMaterial("gunMaterial",
	//	base + "Cerberus_A.tga",
	//	base + "Cerberus_R.tga",
	//	base + "Cerberus_M.tga",
	//	base + "Cerberus_N.tga");

	//g->setGeometryMaterial("gun", m->getMaterial("gunMaterial"));

	osg::Vec3 skyColor = osg::Vec3(0.196078, 0.6, 0.8);
	osg::Vec3 sunsetColor = osg::Vec3(0.9, 0.8, 0.5);
	l->addDirectionalLight("Sun", osg::Vec3(-1, -1, -1), sunsetColor, false);

	Core::setEnvironmentMap(
		"Cubemap_snow/posx.jpg", "Cubemap_snow/negx.jpg",
		"Cubemap_snow/posy.jpg", "Cubemap_snow/negy.jpg",
		"Cubemap_snow/posz.jpg", "Cubemap_snow/negz.jpg");

	//Core::setEnvironmentMap(
	//	"cubemaps/uffizi0000.tif",
	//	"cubemaps/uffizi0001.tif",
	//	"cubemaps/uffizi0005.tif",
	//	"cubemaps/uffizi0004.tif",
	//	"cubemaps/uffizi0002.tif",
	//	"cubemaps/uffizi0003.tif"
	//	);
	//Core::setEnvironmentMap(
	//	"cubemaps/right.png", 
	//	"cubemaps/left.png", 
	//	"cubemaps/front.png", 
	//	"cubemaps/back.png", 
	//	"cubemaps/top.png", 
	//	"cubemaps/bottom.png"
	//	);

	osg::Node *loadedCow = osgDB::readNodeFile("Mesh/sphere.dae");

	g->addGeometry("cow", loadedCow, osg::Vec3(-1, 0, 0));
	g->addGeometry("second", loadedCow, osg::Vec3(1, 0, 0));
	m->createPlainMaterial("planeMat", osg::Vec3(0.8, 0.3, 0.3), 0.5, 0.5, 0.5);
	g->setGeometryMaterial("cow", m->getMaterial("planeMat"));
	g->setGeometryMaterial("second", m->getMaterial("planeMat"));

	//l->addDirectionalLight("Sun", osg::Vec3(-1, -1, -1), osg::Vec3(0.7, 0.3, 0.3), false);

	osg::Vec3 colorGREEN = osg::Vec3(0.3, 0.7, 0.3);
	l->addPointLight("pt2", osg::Vec3(-7, 0, 0), colorGREEN, 100, false);

	osg::Vec3 colorYELLO = osg::Vec3(0.7, 0.7, 0.3);
	l->addPointLight("pt3", osg::Vec3(0, 7, 0), colorYELLO, 100, false);

	osg::Vec3 colorBLUE = osg::Vec3(0.3, 0.3, 0.7);
	l->addPointLight("pt4", osg::Vec3(0, -7, 0), colorBLUE, 100, false);

}