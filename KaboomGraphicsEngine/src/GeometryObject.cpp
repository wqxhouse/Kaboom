#include "stdafx.h" 

#include "GeometryObject.h"
#include <osgDB/ReadFile>
#include <osg/CullFace>
#include <cmath>

#include "Material.h"
#include "MaterialManager.h"
#include "Core.h"

GeometryObject::GeometryObject(const std::string &name, osg::Node *geomNode)
{
	if (geomNode == NULL)
	{
		OSG_WARN << "Geometry object node is NULL " << std::endl;
	}

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
	osg::Vec3 pos, scale;
	osg::Quat rot, so;

	_objRoot->getMatrix().decompose(pos, rot, scale, so);
	return pos;
}

void GeometryObject::setTranslate(const osg::Vec3 &translate)
{
	osg::Matrix mat = _objRoot->getMatrix();
	mat.setTrans(translate);
	_objRoot->setMatrix(mat);
}

osg::Vec3 GeometryObject::getEulerRotation()
{
	osg::Vec4 quat = getRotation().asVec4();

	float q0 = quat.x();
	float q1 = quat.y();
	float q2 = quat.z();
	float q3 = quat.w();

	float xRoll, yPitch, zYaw;


	// Roll and yaw are actually swapped
	xRoll = atan2f(2 * (q0 * q3 + q1 * q2), 1 - 2 * (q2 * q2 + q3 * q3));
	yPitch = asinf(2 * (q0 * q2 - q3 * q1));
	zYaw = atan2f(2 * (q0 * q1 + q2 * q3), 1 - 2 * (q1 * q1 + q2 * q2));
	
	return osg::Vec3(xRoll, yPitch, zYaw);

}

osg::Quat GeometryObject::getRotation()
{
	osg::Vec3 pos, scale;
	osg::Quat rot, so;

	_objRoot->getMatrix().decompose(pos, rot, scale, so);
	return rot;
}

void GeometryObject::setRotation(const osg::Vec3 &rot)
{
	float xRoll, yPitch, zYaw;

	// Roll and yaw are actually swapped
	xRoll = rot.z();
	yPitch = rot.y();
	zYaw = rot.x();

	float tmp1, tmp2, tmp3, tmp4;
	float cx, sx, cy, sy, cz, sz;

	cx = cosf(xRoll / 2.0f);
	sx = sinf(xRoll / 2.0f);
	cy = cosf(yPitch / 2.0f);
	sy = sinf(yPitch / 2.0f);
	cz = cosf(zYaw / 2.0f);
	sz = sinf(zYaw / 2.0f);

	/*cx = cosf(xRoll / 2.0f);
	sx = sinf(xRoll / 2.0f);
	cy = cosf(yPitch / 2.0f);
	sy = sinf(yPitch / 2.0f);
	cz = cosf(zYaw / 2.0f);
	sz = sinf(zYaw / 2.0f);*/

	tmp1 = cx * cy * cz + sx * sy * sz;
	tmp2 = sx * cy * cz - cx * sy * sz;
	tmp3 = cx * sy * cz + sx * cy * sz;
	tmp4 = cx * cy * sz - sx * sy * cz;
	
	osg::Quat quat = osg::Quat(osg::Vec4(tmp1, tmp2, tmp3, tmp4));
	/*osg::Vec4 tmp1 = osg::Vec4(cosf(zYaw / 2.0f), 0.0f, 0.0f, sinf(zYaw / 2.0f));
	osg::Vec4 tmp2 = osg::Vec4(cosf(yPitch / 2.0f), 0.0f, sinf(yPitch / 2.0f), 0.0f);
	osg::Vec4 tmp3 = osg::Vec4(cosf(xRoll / 2.0f), sinf(xRoll / 2.0f), 0.0f, 0.0f);

	osg::Quat quat = osg::Quat(tmp1 * tmp2 * tmp3);
*/
	setRotation(quat);
}

void GeometryObject::setRotation(const osg::Quat &rot)
{
	osg::Matrix mat = _objRoot->getMatrix();
	osg::Vec3 pos = getTranslate();
	osg::Vec3 scale = getScale();

	mat.makeTranslate(pos);
	mat.preMult(osg::Matrix::rotate(rot));
	mat.preMult(osg::Matrix::scale(scale));

	_objRoot->setMatrix(mat);
}

osg::Vec3 GeometryObject::getScale()
{
	osg::Vec3 pos, scale;
	osg::Quat rot, so;

	_objRoot->getMatrix().decompose(pos, rot, scale, so);
	return scale;
}

void GeometryObject::setScale(const osg::Vec3 &scale)
{
	osg::Matrix mat = _objRoot->getMatrix();
	osg::Vec3 pos = getTranslate();
	osg::Quat rot = getRotation();

	mat.makeTranslate(pos);
	mat.preMult(osg::Matrix::rotate(rot));

	//std::cout << mat << std::endl;
	mat.preMult(osg::Matrix::scale(scale));
	//std::cout << mat << std::endl;

	_objRoot->setMatrix(mat);
}

osg::Matrix GeometryObject::getMatrix()
{
	return _objRoot->getMatrix();
}

void GeometryObject::setMatrix(const osg::Matrix &matrix)
{
	_objRoot->setMatrix(matrix);
}

void GeometryObject::decompose(osg::Vec3 &translate, osg::Quat &rot, osg::Vec3 &scale, osg::Quat &so)
{
	_objRoot->getMatrix().decompose(translate, rot, scale, so);
}

void GeometryObject::rename(const std::string& newName)
{
	this->setName(newName);

	// Have to rename some children of GeometryObject's root node
	osg::ref_ptr<osg::MatrixTransform> geomRoot = _objRoot;
	renameHelper(geomRoot, std::string("Transform_"), newName);

	for (unsigned int i = 0; i < geomRoot->getNumChildren(); i++) {
		osg::ref_ptr<osg::Node> child = geomRoot->getChild(i);
		renameHelper(child, std::string("MaterialNode_"), newName);		
	}
}

void GeometryObject::renameHelper(osg::Node *node, const std::string &prefix, const std::string &newName)
{
	std::string nodeName = node->getName();
	auto res = std::mismatch(prefix.begin(), prefix.end(), nodeName.begin());

	if (res.first == prefix.end())
	{
		node->setName(prefix + newName);
	}
}

GeometryObject* GeometryObject::copy(const std::string &newName)
{
	GeometryObject* copy = new GeometryObject(newName, _materialNode->getChild(0), _fileName);

	copy->setMaterial(_material);
	copy->setMatrix(this->getMatrix());
	//copy->_materialNode = this->_materialNode;

	return copy;
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

		ss->addUniform(new osg::Uniform("u_albedoTexLerp", 0.0f));
		ss->addUniform(new osg::Uniform("u_roughnessTexLerp", 0.0f));
		ss->addUniform(new osg::Uniform("u_metallicTexLerp", 0.0f));
		ss->addUniform(new osg::Uniform("u_normalMapLerp", 0.0f));
		ss->addUniform(new osg::Uniform("u_textureOffset", osg::Vec2()));
	}
	else
	{
		ss->setAttributeAndModes(getPlainShader(), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	}

	ss->addUniform(new osg::Uniform("u_albedo", osg::Vec3f(_material->getAlbedo())));
	ss->addUniform(new osg::Uniform("u_roughness", _material->getRoughness()));
	ss->addUniform(new osg::Uniform("u_metallic", _material->getMetallic()));
	ss->addUniform(new osg::Uniform("u_specular", _material->getSpecular()));
}

void GeometryObject::updateMaterialState()
{
	// Before uploading uniforms, run custom update callback to update properties
	if (_material->hasUpdateCallback() && _material->isMaterialUpdateEnabled())
	{
		_material->getUpdateCallback()(_material);
	}

	// TODO: make sure pass (camera) is the correct node for forward shading
	osg::ref_ptr<osg::StateSet> ss = _materialNode->getOrCreateStateSet();

	// TODO: change to UBO later // and translucent
	ss->getUniform("u_albedo")->set(osg::Vec3f(_material->getAlbedo()));
	ss->getUniform("u_roughness")->set(_material->getRoughness());
	ss->getUniform("u_metallic")->set(_material->getMetallic());
	ss->getUniform("u_specular")->set(_material->getSpecular());

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

		ss->getUniform("u_albedoTexLerp")->set(_material->getAlbedoTexLerp());
		ss->getUniform("u_roughnessTexLerp")->set(_material->getRoughnessTexLerp());
		ss->getUniform("u_metallicTexLerp")->set(_material->getMetallicTexLerp());
		ss->getUniform("u_normalMapLerp")->set(_material->getNormalMapMapLerp());
		ss->getUniform("u_textureOffset")->set(_material->getTextureOffset());
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