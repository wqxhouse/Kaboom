#pragma once
#include "Material.h"
#include <Core.h>

Material *createTestMaterial(TextureChangeCallback callback);
void testMaterialCallback(Material *material);