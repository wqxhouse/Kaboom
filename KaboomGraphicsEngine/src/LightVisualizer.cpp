#include "LightVisualizer.h"
#include <osg/Shape>
#include <osg/Depth>
#include <osg/AlphaFunc>
#include <osgDB/ReadFile>

#include <Core.h>
#include <util/ConfigSettings.h>

#include "PointLight.h"

LightVisualizer::LightVisualizer()
{
	init();
	_matTrans->addChild(_lightBillBoard);
}


osg::Geometry *LightVisualizer::createPointLightTexturedQuad()
{
	osg::ref_ptr<osg::Geometry> quad =
		osg::createTexturedQuadGeometry(
		osg::Vec3(-0.5f, 0.0f, -0.5f),
		osg::Vec3(1.0f, 0.0f, 0.0f),
		osg::Vec3(0.0f, 0.0f, 1.0f));
	osg::StateSet* ss = quad->getOrCreateStateSet();
	ss->setTextureAttributeAndModes(0, _pointLightTex.get());
	return quad.release();
}

void LightVisualizer::addLight(Light *light)
{
	LightType type = light->getLightType();
	switch (type)
	{
	case LightType::POINTLIGHT:
	{
		PointLight *pt = light->asPointLight();
		osg::Vec3 position = pt->getPosition();
		_lightBillBoard->addDrawable(createPointLightTexturedQuad(), position);
		_lightPtrs.push_back(pt);
		break;
	}
	default:
		OSG_WARN << "LightVisualizer:: To be implemented... " << std::endl;
	}
}

void LightVisualizer::removeLight(Light *light)
{
	// TODO: can optimize, like keep a reference to the lightmanager
	int index_removed = -1;
	for (int i = 0; i < _lightPtrs.size(); i++)
	{
		if (light == _lightPtrs[i]) // ptr equivalence
		{
			index_removed = i;
			_lightPtrs.erase(_lightPtrs.begin() + i);
			break;
		}
	}

	if (index_removed == -1)
	{
		OSG_WARN << "LightVisualizer:: light to be removed not found" << std::endl;
		return;
	}
	// osg billboard removeDrawable method is not reliable, since it is indexed based on
	// the ptr of drawable. However, children can contain many drawables with the same ptr (same object), 
	// making removeDrawable not correctly removing the actually drawable at desired insert location.
	// Thus, do that manually
	_lightBillBoard->removeDrawables(index_removed, 1);
	_lightBillBoard->getPositionList().erase(_lightBillBoard->getPositionList().begin() + index_removed);
}

void LightVisualizer::init()
{
	ConfigSettings *config = ConfigSettings::config;
	std::string mediaPath;
	config->getValue("MediaPath", mediaPath);
	
	std::string pointLightIconPath = mediaPath + "DefaultAssets\\Icon\\pointlight_icon.png";
	osg::Image *img = osgDB::readImageFile(pointLightIconPath);
	_pointLightTex = new osg::Texture2D;
	_pointLightTex->setImage(img);
	_pointLightTex->setUseHardwareMipMapGeneration(true);

	_lightBillBoard = new osg::Billboard;
	_lightBillBoard->setName("__LightVisualizerBillBoard");
	_lightBillBoard->setMode(osg::Billboard::POINT_ROT_EYE);
	osg::StateSet *bss = _lightBillBoard->getOrCreateStateSet();
	bss->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
	bss->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	osg::Depth *depth = new osg::Depth;
	depth->setWriteMask(false);
	bss->setAttribute(depth);
	bss->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	osg::AlphaFunc* alphaFunction = new osg::AlphaFunc;
	alphaFunction->setFunction(osg::AlphaFunc::GEQUAL, 0.05f);
	bss->setAttributeAndModes(alphaFunction, osg::StateAttribute::ON);

	_lightBillBoard->setUpdateCallback(new LightVisualizerCallback(_lightPtrs));

	
}

LightVisualizerCallback::LightVisualizerCallback(std::vector<Light *> &lightPtrs)
	: _lightPtrsRef(lightPtrs)
{
}

void LightVisualizerCallback::operator()(osg::Node* node, osg::NodeVisitor* nv)
{
	osg::Billboard *billboard = static_cast<osg::Billboard *>(node);
	for (int i = 0; i < _lightPtrsRef.size(); i++)
	{
		billboard->setPosition(i, _lightPtrsRef[i]->getPosition());
	}
}