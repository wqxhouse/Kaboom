#include "stdafx.h" 

#include "Scene.h"
#include "World.h"
#include "Core.h"
#include "GeometryObjectManager.h"
#include "LightManager.h"
#include "MaterialManager.h"

#include <osgDB/ReadFile>
#include <GeometryObject.h>
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

	Core::setEnvironmentMapVerticalCross("C:/Users/w3leung/desktop/Kaboom/Media/haha.png");

	//Core::setEnvironmentMap(
	//	"Cubemap_snow/posx.jpg", "Cubemap_snow/negx.jpg",
	//	"Cubemap_snow/posy.jpg", "Cubemap_snow/negy.jpg",
	//	"Cubemap_snow/posz.jpg", "Cubemap_snow/negz.jpg");
	Core::setEnvironmentMap(
		"Map/posx.png",
		"Map/negx.png",
		"Map/posy.png",
		"Map/negy.png",
		"Map/posz.png",
		"Map/negz.png");

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

	g->addGeometry("cow", loadedCow, osg::Vec3(0, 0, 0));
	//g->addGeometry("second", loadedCow, osg::Vec3(1, 0, 0));
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


	//osg::ref_ptr<osg::Box> floor = new osg::Box;
	//osg::ref_ptr<osg::ShapeDrawable> drawable = new osg::ShapeDrawable(floor);
	osg::ref_ptr<osg::Geode> floorGeo = new osg::Geode;
	//floorModel->addDrawable(drawable);
	
	//g->addGeometry("floor", floorModel, osg::Vec3(0,0,0));
	

	osg::ref_ptr<osg::Vec3Array> floorVertices = new osg::Vec3Array;
	floorVertices->push_back(osg::Vec3(-10, -10, 0)); //left front
	floorVertices->push_back(osg::Vec3(10, -10, 0));  //right front
	floorVertices->push_back(osg::Vec3(10, 10, 0));   //right back
	floorVertices->push_back(osg::Vec3(-10, 10, 0));  //left back
	
	osg::ref_ptr<osg::Geometry> floorGeometry = new osg::Geometry;
	floorGeometry->setVertexArray(floorVertices);

	osg::ref_ptr<osg::DrawElementsUInt> floorBase = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
	floorBase->push_back(0);
	floorBase->push_back(1);	
	floorBase->push_back(2);
	floorBase->push_back(3);
	floorGeometry->addPrimitiveSet(floorBase);

	floorGeo->addDrawable(floorGeometry);
	g->addGeometry("floor", floorGeo, osg::Vec3(0, 0, 0));
	g->setGeometryMaterial("floor", m->getMaterial("planeMat"));


	osg::ref_ptr<osg::Geode> wallBack = new osg::Geode(*floorGeo, osg::CopyOp::DEEP_COPY_ALL);
	g->addGeometry("wallBack", wallBack, osg::Vec3(0, -10, 10));
	g->setGeometryMaterial("wallBack", m->getMaterial("planeMat"));
	osg::Quat q0;
	q0.makeRotate(osg::DegreesToRadians(90.0), -1, 0, 0);
	g->getGeometryObject("wallBack")->setRotation(q0);



	osg::ref_ptr<osg::Geode> wallFront = new osg::Geode(*floorGeo, osg::CopyOp::DEEP_COPY_ALL);
	g->setGeometryMaterial("wallFront", m->getMaterial("planeMat"));
	g->addGeometry("wallFront", wallFront, osg::Vec3(0, 10, 10));
	g->setGeometryMaterial("wallFront", m->getMaterial("planeMat"));
	osg::Quat q1;
	q1.makeRotate(osg::DegreesToRadians(90.0), 1, 0, 0);
	g->getGeometryObject("wallFront")->setRotation(q1);


	osg::ref_ptr<osg::Geode> wallLeft = new osg::Geode(*floorGeo, osg::CopyOp::DEEP_COPY_ALL);
	g->setGeometryMaterial("wallLeft", m->getMaterial("planeMat"));
	g->addGeometry("wallLeft", wallLeft, osg::Vec3(-10, 0, 10));
	//g->setGeometryMaterial("wallLeft", m->getMaterial("planeMat"));
	osg::Quat q2;
	q2.makeRotate(osg::DegreesToRadians(90.0), 0, 1, 0);
	g->getGeometryObject("wallLeft")->setRotation(q2);

	osg::ref_ptr<osg::Geode> wallRight = new osg::Geode(*floorGeo, osg::CopyOp::DEEP_COPY_ALL);
	g->setGeometryMaterial("wallRight", m->getMaterial("planeMat"));
	g->addGeometry("wallRight", wallRight, osg::Vec3(10, 0, 10));
	//g->setGeometryMaterial("wallRight", m->getMaterial("planeMat"));
	osg::Quat q3;
	q3.makeRotate(osg::DegreesToRadians(90.0), 0, -1, 0);
	g->getGeometryObject("wallRight")->setRotation(q3);

	osg::ref_ptr<osg::Geode> ramp = new osg::Geode(*floorGeo, osg::CopyOp::DEEP_COPY_ALL);
	g->setGeometryMaterial("ramp", m->getMaterial("planeMat"));
	g->addGeometry("ramp", ramp, osg::Vec3(-5, 0, 0));
	g->setGeometryMaterial("ramp", m->getMaterial("planeMat"));
	osg::Quat q4;
	q4.makeRotate(osg::DegreesToRadians(30.0), 0, 1, 0);
	g->getGeometryObject("ramp")->setRotation(q4);

	//g->getGeometryObject("floor2")->setTranslate(q0);

}