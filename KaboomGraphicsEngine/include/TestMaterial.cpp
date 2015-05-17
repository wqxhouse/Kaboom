#include "TestMaterial.h"
#include "MaterialManager.h"

Material *createTestMaterial(TextureChangeCallback callback)
{
	Material *mat = new Material("____TestMaterial", callback);
	mat->setAlbedo(osg::Vec3(0.8, 0.2, 0.2));

	return mat;
}

void testMaterialCallback(Material *material)
{
	double time = Core::getTimeElaspedSec();

	osg::Vec3 v = material->getAlbedo();
	v.y() = sin(time * 0.5) * 0.5 + 0.5;
	v.x() = cos(time * 0.5) * 0.5 + 0.5;
	material->setAlbedo(v);

	float roughness = cos(time * 0.5) * 0.5 + 0.5;
	material->setRoughness(roughness);

	float metallic = sin(time * 0.5) * 0.5 + 0.5;
	material->setMetallic(metallic);
}