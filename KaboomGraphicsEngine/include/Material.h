// Zonglin Wu (Robin) 4/2/2015
// Material - manage material properties

#pragma once
#include <string>
#include <osg/Vec3>

class Material;
typedef void(*TextureChangeCallback)(const std::string &texturePath, osg::Texture::WrapMode mode);
typedef void(*MaterialUpdateCallback)(Material *material);

class Material
{
public:
	Material(const std::string &name, TextureChangeCallback onTextureChange);
	~Material();

	inline std::string getName()
	{
		return _name;
	}

	inline void setName(std::string name)
	{
		_name = name;
	}

	inline bool getUseTexture()
	{
		return _useTexture;
	}

	inline void setUseTexture(bool tf)
	{
		_useTexture = tf;
	}

	inline osg::Texture::WrapMode getMode()
	{
		return _mode;
	}

	inline void setMode(osg::Texture::WrapMode mode)
	{
		_mode = mode;
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

	inline void setAlbedoTexturePath(const std::string &albedoTexPath, osg::Texture::WrapMode mode)
	{
		_albedoMapPath = albedoTexPath;
		_onTextureChangeFunc(albedoTexPath, mode);
	}

	inline void setRoughnessMapPath(const std::string &roughnessTexPath, osg::Texture::WrapMode mode)
	{
		_roughnessMapPath = roughnessTexPath;
		_onTextureChangeFunc(roughnessTexPath, mode);
	}

	inline void setMetallicMapPath(const std::string &metallicTexPath, osg::Texture::WrapMode mode)
	{
		_metallicMapPath = metallicTexPath;
		_onTextureChangeFunc(metallicTexPath, mode);
	}

	inline void setNormalMapPath(const std::string &normalMapTexPath, osg::Texture::WrapMode mode)
	{
		_normalMapPath = normalMapTexPath;
		_onTextureChangeFunc(normalMapTexPath, mode);
	}

	inline std::string getAlbedoTexturePath()
	{
		return _albedoMapPath;
	}

	inline std::string getRoughnessTexturePath()
	{
		return _roughnessMapPath;
	}

	inline std::string getMetallicTexturePath()

	{
		return _metallicMapPath;
	}

	inline std::string getNormalMapTexturePath()
	{
		return _normalMapPath;
	}

	inline float getAlbedoTexLerp()
	{
		return _albedoTexLerp;
	}

	inline void setAlbedoTexLerp(float alpha)
	{
		_albedoTexLerp = alpha;
	}

	inline float getRoughnessTexLerp()
	{
		return _roughnessTexLerp;
	}

	inline void setRoughnessTexLerp(float alpha)
	{
		_roughnessTexLerp = alpha;
	}

	inline float getMetallicTexLerp()
	{
		return _metallicTexLerp;
	}

	inline void setMetallicTexLerp(float alpha)
	{
		_metallicTexLerp = alpha;
	}

	inline float getNormalMapMapLerp()
	{
		return _normalMapLerp;
	}

	inline void setNormalMapLerp(float alpha)
	{
		_normalMapLerp = alpha;
	}

	inline void setTextureOffset(const osg::Vec2 &offset)
	{
		_textureOffset = offset;
	}

	inline void clearTextureOffset()
	{
		_textureOffset.x() = 0.0f;
		_textureOffset.y() = 0.0f;
	}

	inline const osg::Vec2 &getTextureOffset()
	{
		return _textureOffset;
	}

	inline void enableMaterialUpdate()
	{
		_materialUpdateEnabled = true;
	}

	inline void disableMaterialUpdate()
	{
		_materialUpdateEnabled = false;
	}

	inline bool isMaterialUpdateEnabled()
	{
		return _materialUpdateEnabled;
	}

	inline void *getUserData()
	{
		return _userData;
	}

	inline MaterialUpdateCallback getUpdateCallback()
	{
		return _updateCallback;
	}

	inline bool hasUpdateCallback()
	{
		return _updateCallback != NULL ? true : false;
	}

private:

	friend class MaterialManager;
	TextureChangeCallback _onTextureChangeFunc;
	MaterialUpdateCallback _updateCallback;

	std::string _name;

	// TODO: support mixed mode later
	// since it is hard to support uber shader now
	bool _useTexture;
	osg::Texture::WrapMode _mode;

	// 6 * 4 = 24 bytes
	float _roughness;
	float _metallic;
	float _specular;
	osg::Vec3 _albedo;

	std::string _roughnessMapPath;
	std::string _metallicMapPath;
	std::string _albedoMapPath;
	std::string _normalMapPath;

	float _albedoTexLerp;
	float _roughnessTexLerp;
	float _metallicTexLerp;
	float _normalMapLerp;

	osg::Vec2 _textureOffset;
	bool _materialUpdateEnabled;

	void *_userData;
};

