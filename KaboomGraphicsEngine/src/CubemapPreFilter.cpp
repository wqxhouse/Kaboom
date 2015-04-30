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
	_generatedCubemap = static_cast<osg::TextureCubeMap *>(tex);

	_isInit = true;
}

void CubeMapPreFilter::init()
{
	if (_sphere == NULL)
	{
		_sphere = osgDB::readNodeFile("Mesh/sphere.dae");
		_sphere->setCullingActive(false);
	}
	_cameraGroup = new osg::Group;
	_cameraGroup->setNodeMask(0x8);
	_cameraGroup->setCullingActive(false);

	osg::ref_ptr<osg::Depth> depth(new osg::Depth());
	depth->setWriteMask(false);
	depth->setFunction(osg::Depth::ALWAYS);
	_cameraGroup->getOrCreateStateSet()->setAttributeAndModes(depth);
	_cameraGroup->getOrCreateStateSet()->setMode(GL_CULL_FACE, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);


	_switch = new osg::Group;
	_shaders = new osg::Program;
	_shaders->addShader(osgDB::readShaderFile("Shaders/cubemapPreFilter.vert"));
	_shaders->addShader(osgDB::readShaderFile("Shaders/cubemapPreFilter.frag"));
	_cameraGroup->getOrCreateStateSet()->setAttributeAndModes(_shaders, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
}

void CubeMapPreFilter::setupCameras(int texWidth, const osg::Vec3 &eyePos)
{
	//int mipMapTotalLevel = 1;
	// 512 256 128 64 32 16 8 4 2 1 = 10 levels
	// Since we need more precision at roughness 1
	// don't generate mipmap all the way down to 1x1
	//int calcMip = texWidth;
	//while (calcMip != 1)
	//{
	//	calcMip /= 2;
	//	mipMapTotalLevel++;
	//}

	int mipMapTotalLevel = 6;
	
	if (_generatedCubemap == NULL) // allow for binding in the effectcompositor
	{
		_generatedCubemap = new osg::TextureCubeMap;
		_generatedCubemap->setTextureSize(texWidth, texWidth);
		_generatedCubemap->setInternalFormat(GL_RGBA16F_ARB);
		_generatedCubemap->setSourceFormat(GL_RGBA);
		_generatedCubemap->setSourceType(GL_FLOAT);
		_generatedCubemap->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
		_generatedCubemap->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
		_generatedCubemap->setWrap(osg::Texture::WRAP_R, osg::Texture::CLAMP_TO_EDGE);
		_generatedCubemap->setFilter(osg::TextureCubeMap::MIN_FILTER, osg::TextureCubeMap::LINEAR_MIPMAP_LINEAR);
		_generatedCubemap->setFilter(osg::TextureCubeMap::MAG_FILTER, osg::TextureCubeMap::LINEAR);
	}

	// TODO: try hardware mipmapping first
	// _generatedCubemap->setUseHardwareMipMapGeneration(true);
	// _generatedCubemap->setNumMipmapLevels(mipMapTotalLevel);

	int currTexSize = texWidth;
	for (int mipmapLevel = 0; mipmapLevel < mipMapTotalLevel; mipmapLevel++)
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

			cam->attach(osg::Camera::COLOR_BUFFER, _generatedCubemap, mipmapLevel, osg::TextureCubeMap::POSITIVE_X + i, false, 0, 0);
			cam->addChild(_switch);

			// lod specific uniform
			cam->getOrCreateStateSet()->addUniform(new osg::Uniform("u_lod", mipmapLevel));

			_cameraGroup->addChild(cam);
		}
		printf("Bind %d level to resolution %d\n", mipmapLevel, currTexSize);
		currTexSize /= 2;
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

	int texSize = 512;
	if (_generatedCubemap != NULL)
	{
		texSize = _generatedCubemap->getTextureWidth();
	}

	setupCameras(texSize, osg::Vec3(0, 0, 0));

	osg::StateSet *ss = _cameraGroup->getOrCreateStateSet();
	ss->addUniform(new osg::Uniform("u_maxLod", 6)); // TODO: test if 6 is appropriate
	ss->addUniform(new osg::Uniform("u_size", texSize));
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
		std::string fileName = path + "\\" + std::to_string(i) + ".tga";
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