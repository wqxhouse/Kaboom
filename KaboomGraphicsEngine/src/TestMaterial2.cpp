#include "TestMaterial.h"
#include "MaterialManager.h"
#include <util/ConfigSettings.h>

std::string createTestMaterial2(TextureChangeCallback callback, MaterialManager *manager)
{
	Material *mat = new Material("____TestMaterial2", callback);
	ConfigSettings *settings = ConfigSettings::config;
	std::string str_mediaPath;
	settings->getValue("MediaPath", str_mediaPath);
	manager->createTexturedMaterial("____TestMaterial2",
		str_mediaPath + "DefaultAssets\\GeometryObject\\__ground\\textures\\ground_baseColor.png",
		str_mediaPath + "DefaultAssets\\GeometryObject\\__ground\\textures\\ground_roughness.png",
		str_mediaPath + "DefaultAssets\\GeometryObject\\__ground\\textures\\ground_metallic.png",
		str_mediaPath + "DefaultAssets\\GeometryObject\\__ground\\textures\\ground_normal.tga", osg::Texture::REPEAT);

	Material *m = manager->getMaterial("____TestMaterial2");
	osg::Vec3 albedo = osg::Vec3(0.0, 0.4375, 0.808);
	m->setAlbedo(albedo);
	m->setRoughness(0.29);
	m->setMetallic(0.09);
	m->setAlbedoTexLerp(0.0);
	m->setRoughnessTexLerp(0.0);
	m->setMetallicTexLerp(0.55);
	m->setNormalMapLerp(0.55);
	m->enableMaterialUpdate();

	return "____TestMaterial2";
}

void testMaterial2Callback(Material *material)
{
	float time = Core::getTimeElaspedSec();
	osg::Vec2 texOffset;
	texOffset.y() += time * 0.1;
	material->setTextureOffset(texOffset);

	float wave = ((cos(time * 1.5) * 0.5 + 0.5) + 1) * 0.5;
	material->setNormalMapLerp(wave);
}

