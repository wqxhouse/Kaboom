#pragma once
#include <unordered_map> 
#include <osg/Vec3>
#include <osg/Texture>

// TODO: built-in material
enum MaterialBuiltIn
{
	DEFAULT, 
};

class GeometryObject;
class Material;
class MaterialManager
{
public:
	MaterialManager();
	~MaterialManager();

	void createPlainMaterial(const std::string &name,
		const osg::Vec3 albedoColor,
		float roughness,
		float specular,
		float metallic);

	void createTextureMaterial(const std::string &name,
		const std::string &albedoPath,
		const std::string &roughnessPath,
		const std::string &specularPath,
		const std::string &metallicPath, 
		const std::string &normalMapPath);

	Material *getMaterial(const std::string &name);
	Material *getBuiltInMaterial(enum MaterialBuiltIn type);

	osg::ref_ptr<osg::Texture> getAlbedoTexture(Material *m);
	osg::ref_ptr<osg::Texture> getSpecularTexture(Material *m);
	osg::ref_ptr<osg::Texture> getRoughnessTexture(Material *m);
	osg::ref_ptr<osg::Texture> getMetallicTexture(Material *m);
	osg::ref_ptr<osg::Texture> getNormalMapTexture(Material *m);

private:
	void createBuiltInMaterials();

	// update texture cache	and associated geometry object binding
	static void onTexturePathChange(const std::string &texturePath);
	static MaterialManager *_weakMaterialManagerPtr;

	// TODO: separate textured material and plain material,
	// so that uniform iteration does not need to use if statement
	// to determine the material type
	// Might not be the case if UBER shader is supported
	// thus mixed material type. e.g: plain on specular, texture on albedo
	std::unordered_map<std::string, Material *> _materialMap;

	// cache texture to avoid duplicated loading
	// since not sure if osgDB already caches textures 
	std::unordered_map<std::string, osg::ref_ptr<osg::Texture> > _textureMap;

	// TODO: confirm if necessary, considering updating uniform every frame as an alternative
	// binded geometry objects for update 
	std::unordered_map<std::string, GeometryObject *> _materialGeomBinding;

	std::vector<Material *> _builtInMaterial;
};

