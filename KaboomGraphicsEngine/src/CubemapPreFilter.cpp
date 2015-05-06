#include "CubeMapPreFilter.h"
#include <osgDB/ReadFile>
#include <osg/Program>
#include <osg/Depth>
#include <osg/CullFace>
#include <osgDB/WriteFile>


CubeMapPreFilter::CubeMapPreFilter()
	: _isInit(false)
{
}

CubeMapPreFilter::CubeMapPreFilter(osg::Camera *passCam)
	: _isInit(false)
{
	init();
	
	_passCam = passCam;
	// assume cubemap is in the passcam attachment 

	if (_passCam == NULL || _passCam->getBufferAttachmentMap().size() == 0)
	{
		OSG_WARN << "CubemapPreFilter: Buffer attachment is empty" << std::endl;
		return;
	}

	osg::Texture *tex = _passCam->getBufferAttachmentMap().begin()->second._texture;
	_generatedSpecularCubemap = static_cast<osg::TextureCubeMap *>(tex);

	_isInit = true;
}

void CubeMapPreFilter::init()
{
	if (_sphere == NULL)
	{
		_sphere = osgDB::readNodeFile("DefaultAssets/GeometryObject/sphere.dae");
		_sphere->setCullingActive(false);
	}
	_cameraGroup = new osg::Group;
	_cameraGroup->setNodeMask(0x8);
	_cameraGroup->setCullingActive(false);

	_diffuseCamGroup = new osg::Group;
	_specularCamGroup = new osg::Group;

	osg::ref_ptr<osg::Depth> depth(new osg::Depth());
	depth->setWriteMask(false);
	depth->setFunction(osg::Depth::ALWAYS);
	_cameraGroup->getOrCreateStateSet()->setAttributeAndModes(depth);
	_cameraGroup->getOrCreateStateSet()->setMode(GL_CULL_FACE, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);

	_cameraGroup->addChild(_diffuseCamGroup);
	_cameraGroup->addChild(_specularCamGroup);

	_switch = new osg::Group;
	_specularPrefilterShader = new osg::Program;
	_specularPrefilterShader->addShader(osgDB::readShaderFile("Shaders/cubemapPreFilter.vert"));
	_specularPrefilterShader->addShader(osgDB::readShaderFile("Shaders/cubemapSpecularPreFilter.frag"));

	_diffusePrefilterShader = new osg::Program;
	_diffusePrefilterShader->addShader(osgDB::readShaderFile("Shaders/cubemapPreFilter.vert"));
	_diffusePrefilterShader->addShader(osgDB::readShaderFile("Shaders/cubemapDiffusePreFilter.frag"));

	_diffuseCamGroup->getOrCreateStateSet()->setAttributeAndModes(_diffusePrefilterShader, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	_specularCamGroup->getOrCreateStateSet()->setAttributeAndModes(_specularPrefilterShader, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
}

void CubeMapPreFilter::setupCameras(int texWidth, const osg::Vec3 &eyePos, int &o_maxLodLevel)
{
	int maxLodLevel = log2(texWidth);
	o_maxLodLevel = maxLodLevel;
	// const int mipmapCorrection = 1; // for keeping roughness resolution between 0.0 and 0.1
	int numMipToGenerate = maxLodLevel;

	// int mipMapTotalLevel = 6;
	if (_generatedSpecularCubemap == NULL) // allow for binding in the effectcompositor
	{
		_generatedSpecularCubemap = new osg::TextureCubeMap;
		_generatedSpecularCubemap->setTextureSize(texWidth, texWidth);
		_generatedSpecularCubemap->setInternalFormat(GL_RGBA16F_ARB);
		_generatedSpecularCubemap->setSourceFormat(GL_RGBA);
		_generatedSpecularCubemap->setSourceType(GL_FLOAT);
		_generatedSpecularCubemap->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
		_generatedSpecularCubemap->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
		_generatedSpecularCubemap->setWrap(osg::Texture::WRAP_R, osg::Texture::CLAMP_TO_EDGE);
		_generatedSpecularCubemap->setFilter(osg::TextureCubeMap::MIN_FILTER, osg::TextureCubeMap::LINEAR_MIPMAP_LINEAR);
		_generatedSpecularCubemap->setFilter(osg::TextureCubeMap::MAG_FILTER, osg::TextureCubeMap::LINEAR);
	}

	int currTexSize = texWidth;
	for (int mipmapLevel = 0; mipmapLevel < numMipToGenerate; mipmapLevel++)
	{
		for (int i = 0; i < 6; i++) // six faces
		{
			osg::Camera *cam = new osg::Camera;
			_cameraList.push_back(cam);

			osg::Matrix projMat;
			projMat.makePerspective(90, 1, 1, 1000);
			cam->setProjectionMatrix(projMat);
			cam->setViewMatrix(calcViewMatrix(osg::TextureCubeMap::POSITIVE_X + i, eyePos));

			cam->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			cam->setViewport(0, 0, currTexSize, currTexSize); // default
			cam->setRenderOrder(osg::Camera::PRE_RENDER);
			cam->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
			cam->setReferenceFrame(osg::Camera::ABSOLUTE_RF);

			// investigate true / false
			// debug
			/*_images.push_back(new osg::Image);
			_images.back()->setInternalTextureFormat(GL_RGBA32F_ARB);
			_images.back()->allocateImage(currTexSize, currTexSize, 1, GL_RGBA32F_ARB, GL_FLOAT);
			cam->attach(osg::Camera::COLOR_BUFFER, _images.back());*/

			cam->attach(osg::Camera::COLOR_BUFFER, _generatedSpecularCubemap, mipmapLevel, osg::TextureCubeMap::POSITIVE_X + i, false, 0, 0);
			cam->addChild(_switch);

			// lod specific uniform
			cam->getOrCreateStateSet()->addUniform(new osg::Uniform("u_lod", mipmapLevel));

			_specularCamGroup->addChild(cam);
		}
		printf("Bind %d level to resolution %d\n", mipmapLevel, currTexSize);
		currTexSize /= 2;
	}

	if (_generatedDiffuseCubemap == NULL)
	{
		_generatedDiffuseCubemap = new osg::TextureCubeMap;
		_generatedDiffuseCubemap->setTextureSize(texWidth, texWidth);
		_generatedDiffuseCubemap->setInternalFormat(GL_RGBA16F_ARB);
		_generatedDiffuseCubemap->setSourceFormat(GL_RGBA);
		_generatedDiffuseCubemap->setSourceType(GL_FLOAT);
		_generatedDiffuseCubemap->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
		_generatedDiffuseCubemap->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
		_generatedDiffuseCubemap->setWrap(osg::Texture::WRAP_R, osg::Texture::CLAMP_TO_EDGE);
		_generatedDiffuseCubemap->setFilter(osg::TextureCubeMap::MIN_FILTER, osg::TextureCubeMap::LINEAR_MIPMAP_LINEAR);
		_generatedDiffuseCubemap->setFilter(osg::TextureCubeMap::MAG_FILTER, osg::TextureCubeMap::LINEAR);
	}

	// diffuse convolution camera
	for (int i = 0; i < 6; i++)
	{
		osg::Camera *cam = new osg::Camera;
		osg::Matrix projMat;
		projMat.makePerspective(90, 1, 1, 1000);
		cam->setProjectionMatrix(projMat);
		cam->setViewMatrix(calcViewMatrix(osg::TextureCubeMap::POSITIVE_X + i, eyePos));

		cam->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		cam->setViewport(0, 0, 128, 128); // default
		cam->setRenderOrder(osg::Camera::PRE_RENDER);
		cam->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
		cam->setReferenceFrame(osg::Camera::ABSOLUTE_RF);

		// investigate true / false
		// debug
	/*	_images.push_back(new osg::Image);
		_images.back()->setInternalTextureFormat(GL_RGBA);
		_images.back()->allocateImage(128, 128, 1, GL_RGBA, GL_UNSIGNED_BYTE);
		cam->attach(osg::Camera::COLOR_BUFFER, _images.back());*/

		cam->attach(osg::Camera::COLOR_BUFFER, _generatedDiffuseCubemap, 0, osg::TextureCubeMap::POSITIVE_X + i, false, 0, 0);
		cam->addChild(_switch);

		_diffuseCamGroup->addChild(cam);
	}
}

void CubeMapPreFilter::initWithCubeMap(osg::TextureCubeMap *cubemap)
{
	if (_isInit == false)
	{
		init();
		_isInit = true;
	}

	_cubemap = cubemap;

	int texSize = 128;
	if (_generatedSpecularCubemap != NULL)
	{
		texSize = _generatedSpecularCubemap->getTextureWidth();
	}

	int maxLodLevel = -1;
	setupCameras(texSize, osg::Vec3(0, 0, 0), maxLodLevel);
	if (maxLodLevel < 0)
	{
		OSG_WARN << "CubemapPrefilter: maxlod level error" << std::endl;
	}

	_maxLodLevel = maxLodLevel;

	osg::StateSet *ss = _cameraGroup->getOrCreateStateSet();
	ss->addUniform(new osg::Uniform("u_maxLodLevel", maxLodLevel)); // TODO: test if 6 is appropriate
	ss->addUniform(new osg::Uniform("u_cubeMapTex", 0));
	ss->setTextureAttributeAndModes(0, cubemap);

	osg::ref_ptr<osg::MatrixTransform> _transform(new osg::MatrixTransform);
	osg::Matrix m;
	m.makeScale(osg::Vec3(10, 10, 10));
	_transform->setMatrix(m);

	_switch->addChild(_transform);
	_transform->addChild(_sphere);
}

void CubeMapPreFilter::changeCubeMap(osg::TextureCubeMap *cubemap)
{
	_cubemap = cubemap;

	osg::StateSet *ss = _cameraGroup->getOrCreateStateSet();
	ss->setTextureAttributeAndModes(0, cubemap);
}

osg::Matrix CubeMapPreFilter::calcViewMatrix(int face, const osg::Vec3 &eyePos)
{
	std::cout << face << std::endl;
	static const osg::Vec3 viewDirs[6] = { 
		osg::Vec3(1, 0, 0), 
		osg::Vec3(-1, 0, 0), 
		osg::Vec3(0, 1, 0), 
		osg::Vec3(0, -1, 0), 
		osg::Vec3(0, 0, 1), 
		osg::Vec3(0, 0, -1) 
	};

	static const osg::Vec3 upDirs[6] = {
		osg::Vec3(0, -1, 0), 
		osg::Vec3(0, -1, 0), 
		osg::Vec3(0, 0, 1), 
		osg::Vec3(0, 0, -1), 
		osg::Vec3(0, -1, 0), 
		osg::Vec3(0, -1, 0), 
	};

	if (face < (int)osg::TextureCubeMap::POSITIVE_X || face > (int)osg::TextureCubeMap::NEGATIVE_Z)
	{
		return osg::Matrix();
	}

	osg::Matrix cam;
	cam.makeLookAt(eyePos, eyePos + viewDirs[face], upDirs[face]);

	//if (face == 0)
	//{
	//	osg::Matrix m;
	//	m.makeRotate(osg::PI_2, osg::Vec3(1, 0, 0));
	//	cam = cam * m;
	//}
	
	return cam;
}

int CubeMapPreFilter::getCubemapMaxLodLevel()
{
	return _maxLodLevel;
}

void CubeMapPreFilter::enableCompute()
{
	if (!_isInit) return;
	if (_cameraGroup->getNodeMask() == 0x0)
	{
		_cameraGroup->setNodeMask(0x8);
	}
}

void CubeMapPreFilter::disableCompute()
{
	if (!_isInit) return;
	if (_cameraGroup->getNodeMask() == 0x8)
	{
		_cameraGroup->setNodeMask(0x0);
	}
}

void CubeMapPreFilter::saveImagesToFile(const std::string &path)
{
	for (int i = 0; i < _images.size(); i++)
	{
		std::string fileName = path + "\\" + std::to_string(i) + ".png";
		bool res = osgDB::writeImageFile(*_images[i], fileName);
		if (res)
		{
			std::cout << "Writing image " + fileName + " successfully" << std::endl;
		}
		else
		{
			std::cout << "Writing image " + fileName + " failed" << std::endl;
		}
	}
}

osg::ref_ptr<osg::Node> CubeMapPreFilter::_sphere = NULL;