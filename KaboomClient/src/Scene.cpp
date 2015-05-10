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
	quad_tcoords->push_back(osg::Vec2(500, 0));
	quad_tcoords->push_back(osg::Vec2(500, 500));
	quad_tcoords->push_back(osg::Vec2(0, 500));

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
	floorGeometry->setNormalArray(normal_coords, osg::Array::Binding::BIND_PER_VERTEX);

	osg::ref_ptr<osg::DrawElementsUInt> floorBase = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
	floorBase->push_back(0);
	floorBase->push_back(1);	
	floorBase->push_back(2);
	floorBase->push_back(3);
	floorGeometry->addPrimitiveSet(floorBase);

	floorGeo->addDrawable(floorGeometry);
	// Add * in front of the name to make it un-movable;
	manager->addGeometry("*__Huge_floor", floorGeo, osg::Vec3(0, 0, 0));

	// material
	ConfigSettings* config = ConfigSettings::config;
	std::string str_mediaPath = "";
	config->getValue(ConfigSettings::str_mediaFilePath, str_mediaPath);
	matManager->createTextureMaterial("__floorMat",
		str_mediaPath + "DefaultAssets\\GeometryObject\\__ground\\textures\\ground_baseColor.png",
		str_mediaPath + "DefaultAssets\\GeometryObject\\__ground\\textures\\ground_roughness.png",
		str_mediaPath + "DefaultAssets\\GeometryObject\\__ground\\textures\\ground_metallic.png",
		str_mediaPath + "DefaultAssets\\GeometryObject\\__ground\\textures\\ground_normal.tga", osg::Texture::REPEAT);

	manager->setGeometryMaterial("*__Huge_floor", matManager->getMaterial("__floorMat"));
}

void setupScene()
{
	World &w = Core::getWorldRef();
	GeometryObjectManager *g = w.getGeometryManager();
	MaterialManager *m = w.getMaterialManager();
	LightManager *l = w.getLightManager();

	// Load XML files
	ConfigSettings* config = ConfigSettings::config;
	std::string str_mediaPath = "";
	std::string str_material_xml = "";
	std::string str_world_xml = "";

	config->getValue(ConfigSettings::str_mediaFilePath, str_mediaPath);
	config->getValue(ConfigSettings::str_material_xml, str_material_xml);
	config->getValue(ConfigSettings::str_world_xml, str_world_xml);

	str_world_xml = str_mediaPath + str_world_xml;

	//Core::loadMaterialFile(str_material_xml);
	//Core::loadWorldFile(str_world_xml);

	createInfinitePlane(g, m);
	l->addDirectionalLight("Sun", osg::Vec3(0.1, -0.88, -0.46), osg::Vec3(0.7, 0.7, 0.7), false);

	osg::ref_ptr<osg::Sphere> sp = new osg::Sphere;
	osg::ref_ptr<osg::ShapeDrawable> sd = new osg::ShapeDrawable;
	sd->setShape(sp);
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(sd);
	g->addGeometry("sphere", geode, osg::Vec3(0, 20, 1));

	m->createPlainMaterial("SphereMat", osg::Vec3(0.3, 0.3, 0.3), 0.1, 0.5, 1.0);
	g->setGeometryMaterial("sphere", m->getMaterial("SphereMat"));

	l->addPointLight("debug", osg::Vec3(0, 0, 2), osg::Vec3(0, 1, 0), 100, false);
	l->addPointLight("debug1", osg::Vec3(0, 0, 2), osg::Vec3(0, 1, 0), 100, false);
	l->addPointLight("debug2", osg::Vec3(0, 0, 2), osg::Vec3(0, 1, 0), 100, false);

}