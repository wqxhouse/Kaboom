#include "stdafx.h" 

#include <util/ConfigSettings.h>
#include <osgDB/ReadFile>
#include <GeometryObject.h>

#include "Scene.h"
#include "World.h"
#include "Core.h"
#include "GeometryObjectManager.h"
#include "LightManager.h"
#include "MaterialManager.h"

void createInfinitePlane(GeometryObjectManager *manager, MaterialManager *matManager)
{
	// TODO: tesslllate it here or using tess shader (height map)
	osg::ref_ptr<osg::Geode> floorGeo = new osg::Geode;
	osg::ref_ptr<osg::Vec3Array> floorVertices = new osg::Vec3Array;
	floorVertices->push_back(osg::Vec3(-1000, -1000, 0)); //left front
	floorVertices->push_back(osg::Vec3(1000, -1000, 0));  //right front
	floorVertices->push_back(osg::Vec3(1000, 1000, 0));   //right back
	floorVertices->push_back(osg::Vec3(-1000, 1000, 0));  //left back

	osg::ref_ptr<osg::Vec2Array> quad_tcoords = new osg::Vec2Array; // texture coords
	quad_tcoords->push_back(osg::Vec2(0, 0));
	quad_tcoords->push_back(osg::Vec2(30, 0));
	quad_tcoords->push_back(osg::Vec2(30, 30));
	quad_tcoords->push_back(osg::Vec2(0, 30));

	osg::ref_ptr<osg::Vec3Array> normal_coords = new osg::Vec3Array;
	normal_coords->push_back(osg::Vec3(0, 0, 1));
	normal_coords->push_back(osg::Vec3(0, 0, 1));
	normal_coords->push_back(osg::Vec3(0, 0, 1));
	normal_coords->push_back(osg::Vec3(0, 0, 1));

	osg::ref_ptr<osg::Geometry> quad_geom = new osg::Geometry;
	osg::ref_ptr<osg::DrawArrays> quad_da = new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4);

	osg::ref_ptr<osg::Geometry> floorGeometry = new osg::Geometry;
	floorGeometry->setVertexArray(floorVertices);
	floorGeometry->setTexCoordArray(0, quad_tcoords);


	osg::ref_ptr<osg::DrawElementsUInt> floorBase = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
	floorBase->push_back(0);
	floorBase->push_back(1);	
	floorBase->push_back(2);
	floorBase->push_back(3);
	floorGeometry->addPrimitiveSet(floorBase);

	floorGeo->addDrawable(floorGeometry);
	manager->addGeometry("__Huge_floor", floorGeo, osg::Vec3(0, 0, -1));

	// material
	ConfigSettings* config = ConfigSettings::config;
	std::string str_mediaPath = "";
	config->getValue(ConfigSettings::str_mediaFilePath, str_mediaPath);
	matManager->createTextureMaterial("__floorMat",
		str_mediaPath + "DefaultAssets\\GeometryObject\\__ground\\textures\\ground_baseColor.png",
		str_mediaPath + "DefaultAssets\\GeometryObject\\__ground\\textures\\ground_roughness.png",
		str_mediaPath + "DefaultAssets\\GeometryObject\\__ground\\textures\\ground_metallic.png",
		str_mediaPath + "DefaultAssets\\GeometryObject\\__ground\\textures\\ground_normal.tga", osg::Texture::REPEAT);

	manager->setGeometryMaterial("__Huge_floor", matManager->getMaterial("__floorMat"));
}

void setupScene()
{
	World &w = Core::getWorldRef();
	GeometryObjectManager *g = w.getGeometryManager();
	MaterialManager *m = w.getMaterialManager();
	LightManager *l = w.getLightManager();

	createInfinitePlane(g, m);
	l->addDirectionalLight("Sun", osg::Vec3(0.1, -0.88, -0.46), osg::Vec3(0.7, 0.7, 0.7), false);

	osg::ref_ptr<osg::Sphere> sp = new osg::Sphere;
	osg::ref_ptr<osg::ShapeDrawable> sd = new osg::ShapeDrawable;
	sd->setShape(sp);
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(sd);
	g->addGeometry("sphere", geode);
}