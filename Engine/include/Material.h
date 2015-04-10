// Zonglin Wu (Robin) 4/2/2015
// Material - manage material properties

#pragma once
#include <string>
#include <osg/Vec3>

typedef void(*TextureChangeCallback)(const std::string &texturePath);

class Material
{
public:
	Material(const std::string &name, TextureChangeCallback onTextureChange);
	~Material();

	inline bool getUseTexture()
	{
		return _useTexture;
	}

	inline void setUseTexture(bool tf)
	{
		_useTexture = tf;
	}

	inline void setPlainParameter(const osg::Vec3 &albedo, float roughness, float metallic, float specular)
	{
		_albedo = albedo;
		_roughness = roughness;
		_metallic = metallic;
		_specular = specular;
	}

	inline const osg::Vec3 &getAlbedo()
	{
		return _albedo;
	}

	inline void setAlbedo(const osg::Vec3 &albedo)
	{
		_albedo = albedo;
	}

	inline float getRoughness()
	{
		return _roughness;
	}

	inline void setRoughness(float roughness)
	{
		_roughness = roughness;
	}

	inline float getMetallic()
	{
		return _metallic;
	}

	inline void setMetallic(float metallic)
	{
		_metallic = metallic;
	}

	inline float getSpecular()
	{
		return _specular;
	}

	inline void setSpecular(float specular)
	{
		_specular = specular;
	}

	inline void setAlbedoTexturePath(std::string &albedoTexPath)
	{
		_albedoMapPath = albedoTexPath;
		_onTextureChangeFunc(albedoTexPath);
	}

	inline void setRoughnessMapPath(std::string &roughnessTexPath)
	{
		_roughnessMapPath = roughnessTexPath;
		_onTextureChangeFunc(roughnessTexPath);
	}

	inline void setSpecularMapPath(std::string &specularTexpath)
	{
		_specularMapPath = specularTexpath;
		_onTextureChangeFunc(specularTexpath);
	}

	inline void setMetallicMapPath(std::string &metallicTexPath)
	{
		_metallicMapPath = metallicTexPath;
		_onTextureChangeFunc(metallicTexPath);
	}

private:

	TextureChangeCallback _onTextureChangeFunc;

	std::string _name;

	// TODO: support mixed mode later
	// since it is hard to support uber shader now
	bool _useTexture;

	// 6 * 4 = 24 bytes
	float _roughness;
	float _metallic;
	float _specular;
	osg::Vec3 _albedo;

	std::string _roughnessMapPath;
	std::string _metallicMapPath;
	std::string _specularMapPath;
	std::string _albedoMapPath;
};

