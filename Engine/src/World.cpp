#include "World.h"

#include <osgDB/ReadFile>
#include "stdafx.h" 

#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>
#include <osgDB/Options>
#include <osgDB/XmlParser>

#include "GeometryObjectManager.h"
#include "LightManager.h"
#include "MaterialManager.h"

World::World()
{
	_geomManager = new GeometryObjectManager;
	_materialManager = new MaterialManager;
	_lightManager = new LightManager;
}

World::~World()
{
	delete _geomManager;
	delete _materialManager;
	delete _lightManager;
}


void World::loadWorldXMLFile(const std::string &worldFilePath)
{
	osg::ref_ptr<osgDB::XmlNode> xmlRoot = osgDB::readXmlFile(worldFilePath);
	if (xmlRoot.valid())
	{
		osgDB::FilePathList& filePaths = osgDB::getDataFilePathList();
		filePaths.push_back(osgDB::getFilePath(worldFilePath));
		loadXMLNode(xmlRoot.get());
		filePaths.pop_back();
	}
	else
	{
		// TODO : output to console bad xml
	}
}

void World::loadXMLNode(osgDB::XmlNode *xmlRoot)
{

}