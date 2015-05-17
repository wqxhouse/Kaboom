#include "TestMaterial.h"
#include "MaterialManager.h"

Material *createTestMaterial(TextureChangeCallback callback)
{
	Material *mat = new Material("____TestMaterial", callback);
	mat->setAlbedo(osg::Vec3(0.8, 0.2, 0.2));
	mat->enableMaterialUpdate();

	return mat;
}

void testMaterialCallback(Material *material)
{
	double time = Core::getTimeElaspedSec();

	osg::Vec3 v = material->getAlbedo();
	float wave1 = sin(time * 0.5) * 0.5 + 0.5;
	float wave2 = cos(time * 0.5) * 0.5 + 0.5;
	v.y() = wave1;
	v.x() = wave2;
	material->setAlbedo(v);

	float roughness = wave2;
	material->setRoughness(roughness);

	float metallic = wave1;
	material->setMetallic(metallic);
}