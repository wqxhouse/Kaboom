#include "stdafx.h" 

#include "GeometryObject.h"
#include <osgDB/ReadFile>
#include <osg/CullFace>

#include "Material.h"
#include "MaterialManager.h"
#include "Core.h"

GeometryObject::GeometryObject(const std::string &name, osg::Node *geomNode)
{
	getPlainShader();
	getTexturedShader();

	_name = name;
	_objRoot = new osg::MatrixTransform;
	_materialNode = new osg::Group;
	_materialNode->setName("MaterialNode_" + name);
	_objRoot->addChild(_materialNode);

	osg::ref_ptr<osg::CullFace> cullSettings(new osg::CullFace);
	
	// TODO: figure out how to deal with reflection for back faces
	cullSettings->setMode(osg::CullFace::BACK);
	geomNode->getOrCreateStateSet()->setAttributeAndModes(cullSettings, osg::StateAttribute::ON);
	_materialNode->addChild(geomNode);

	_objRoot->setUpdateCallback(new GeometryObjectNodeUpadateCallback(this));
	_objRoot->setName("Transform_" + name);

	setMaterial(Core::getWorldRef().getMaterialManager()->getBuiltInMaterial(DEFAULT));
}


GeometryObject::GeometryObject(const std::string &name, osg::Node *geomNode, std::string fileName)
: GeometryObject(name, geomNode)
{
	_fileName = fileName;
}

GeometryObject::~GeometryObject()
{
}

void GeometryObject::setMaterial(Material *material)
{
	_material = material;

	// reset
	setUpMaterialState();
}

void GeometryObject::setTransform(const osg::Matrixf &transform)
{
	// TODO : test
	_objRoot->setMatrix(transform);
}

osg::Vec3 GeometryObject::getTranslate()
{
	const osg::Matrix &mat = _objRoot->getMatrix();
	return mat.getTrans();
}

void GeometryObject::setTranslate(const osg::Vec3 &translate)
{
	osg::Matrix mat = _objRoot->getMatrix();
	mat.setTrans(translate);
	_objRoot->setMatrix(mat);
}

osg::Quat GeometryObject::getRotation()
{
	osg::Quat rot = _objRoot->getMatrix().getRotate();
	return rot;
}

void GeometryObject::setRotation(const osg::Quat &rot)
{
	osg::Matrix mat = _objRoot->getMatrix();
	mat.setRotate(rot);
	_objRoot->setMatrix(mat);
}

void GeometryObject::setUpMaterialState()
{
	osg::ref_ptr<osg::StateSet> ss = _materialNode->getOrCreateStateSet();

	// for changing materials
	ss->clear();
	
	if (_material->getUseTexture())
	{
		// TODO : implement
		ss->setAttributeAndModes(getTexturedShader(), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
		MaterialManager *mm = Core::getWorldRef().getMaterialManager();
		osg::Texture *albedoTex = mm->getAlbedoTexture(_material);
		ss->setTextureAttributeAndModes(0, albedoTex);
		osg::Texture *roughnessTex = mm->getRoughnessTexture(_material);
		ss->setTextureAttributeAndModes(1, roughnessTex);
		osg::Texture *metallicTex = mm->getMetallicTexture(_material);
		ss->setTextureAttributeAndModes(2, metallicTex);
		osg::Texture *normalMapTex = mm->getNormalMapTexture(_material);
		ss->setTextureAttributeAndModes(3, normalMapTex);

		ss->addUniform(new osg::Uniform("u_albedoTex", 0));
		ss->addUniform(new osg::Uniform("u_roughnessTex", 1));
		ss->addUniform(new osg::Uniform("u_metallicTex", 2));
		ss->addUniform(new osg::Uniform("u_normalMapTex", 3));
	}
	else
	{
		ss->setAttributeAndModes(getPlainShader(), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
		ss->addUniform(new osg::Uniform("u_albedo", osg::Vec3f(_material->getAlbedo())));
		ss->addUniform(new osg::Uniform("u_roughness", _material->getRoughness()));
		ss->addUniform(new osg::Uniform("u_metallic", _material->getMetallic()));
		ss->addUniform(new osg::Uniform("u_specular", _material->getSpecular()));
	}
}

void GeometryObject::updateMaterialState()
{
	// TODO: make sure pass (camera) is the correct node for forward shading
	osg::ref_ptr<osg::StateSet> ss = _materialNode->getOrCreateStateSet();

	if (_material->getUseTexture())
	{
		// TODO: profile to see if this impacts performance
		MaterialManager *mm = Core::getWorldRef().getMaterialManager();
		osg::Texture *albedoTex = mm->getAlbedoTexture(_material);
		ss->setTextureAttributeAndModes(0, albedoTex);
		osg::Texture *roughnessTex = mm->getRoughnessTexture(_material);
		ss->setTextureAttributeAndModes(1, roughnessTex);
		osg::Texture *metallicTex = mm->getMetallicTexture(_material);
		ss->setTextureAttributeAndModes(2, metallicTex);
		osg::Texture *normalMapTex = mm->getNormalMapTexture(_material);
		ss->setTextureAttributeAndModes(3, normalMapTex);
	}
	else
	{
		// TODO: change to UBO later // and translucent
		ss->getUniform("u_albedo")->set(osg::Vec3f(_material->getAlbedo()));
		ss->getUniform("u_roughness")->set(_material->getRoughness());
		ss->getUniform("u_metallic")->set(_material->getMetallic());
		ss->getUniform("u_specular")->set(_material->getSpecular());
	}

}

osg::ref_ptr<osg::Program> GeometryObject::getPlainShader()
{
	if (_plainShader == NULL)
	{
		_plainShader = new osg::Program();
		_plainShader->addShader(osgDB::readShaderFile("Shaders/gbuffer_plain.vert"));
		_plainShader->addShader(osgDB::readShaderFile("Shaders/gbuffer_plain.frag"));
	}

	return _plainShader;
}

osg::ref_ptr<osg::Program> GeometryObject::getTexturedShader()
{
	if (_texturedShader == NULL)
	{
		_texturedShader = new osg::Program();
		_texturedShader->addShader(osgDB::readShaderFile("Shaders/gbuffer_textured.vert"));
		_texturedShader->addShader(osgDB::readShaderFile("Shaders/gbuffer_textured.frag"));
	}

	return _texturedShader;
}

osg::ref_ptr<osg::Program> GeometryObject::_plainShader;
osg::ref_ptr<osg::Program> GeometryObject::_texturedShader;