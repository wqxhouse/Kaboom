#include "ObjectGlowManager.h"
#include <osgDB/ReadFile>
#include <util/ConfigSettings.h>
#include <osg/Depth>
#include <osg/BlendFunc>

ObjectGlowManager::ObjectGlowManager()
{
	ConfigSettings *config = ConfigSettings::config;
	std::string mediaPath;
	config->getValue("MediaPath", mediaPath);

	_glowObjectGroup = new osg::Group;
	_glowObjectGroup->setNodeMask(0x40);

	// load shader
	_shader = new osg::Program;
	_shader->addShader(osgDB::readShaderFile(mediaPath + "Shaders\\objectGlow.vert"));
	_shader->addShader(osgDB::readShaderFile(mediaPath + "Shaders\\objectGlow.frag"));

	osg::StateSet *ss = _glowObjectGroup->getOrCreateStateSet();
	ss->setAttributeAndModes(_shader, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

	osg::Depth *depth = new osg::Depth;
	depth->setWriteMask(false);
	ss->setAttribute(depth);

	osg::BlendFunc *blendFunc = new osg::BlendFunc;
	blendFunc->setFunction(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	ss->setAttributeAndModes(blendFunc);
}

void ObjectGlowManager::addGlowGeometryObject(GeometryObject *geom)
{
	if (geom == NULL) return;
	// _glowObjects.push_back(geom);
	osg::MatrixTransform *mt = geom->getRoot();
	_glowObjectGroup->addChild(mt);
}

void ObjectGlowManager::removeGlowGeometryObject(GeometryObject *geomObj)
{
	osg::MatrixTransform *mt = geomObj->getRoot();
	_glowObjectGroup->removeChild(mt);
}