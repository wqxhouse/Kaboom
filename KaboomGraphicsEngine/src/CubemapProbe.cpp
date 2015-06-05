#include "CubemapProbe.h"

#define CAPTURE_RESOLUTION 1024

CubemapProbe::CubemapProbe(osgFX::EffectCompositor *passes, osg::Program *shader)
	: _passes(passes), _isInit(false), _shader(shader)
{
	_radius = 10.0f;
	_position = osg::Vec3();
	// _cam = new osg::Camera;

	_bsphere.set(_position, _radius);

	_camGroup = new osg::Group;
	_cameraList.resize(6);
	_sampleCube = new osg::TextureCubeMap;
	setupCubeMap();
	setupProbeCamera();

	// TODO: make sure is the first or second technique, though it doesn't matter
	osgFX::EffectCompositor::PassData passData;
	passes->getPassData("LightPass", passData);
	osg::Camera *cam = passData.pass;
	_shadingBuf = cam->getBufferAttachmentMap().begin()->second._texture;

	_isInit = true;
}

void CubemapProbe::setupCubeMap()
{
	_sampleCube->setTextureSize(CAPTURE_RESOLUTION, CAPTURE_RESOLUTION);

	//_generatedSpecularCubemap->setInternalFormat(GL_RGBA16F_ARB);
	_sampleCube->setInternalFormat(GL_RGB);
	_sampleCube->setSourceFormat(GL_RGB);
	_sampleCube->setSourceType(GL_UNSIGNED_BYTE);
	_sampleCube->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
	_sampleCube->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
	_sampleCube->setWrap(osg::Texture::WRAP_R, osg::Texture::CLAMP_TO_EDGE);
	_sampleCube->setFilter(osg::TextureCubeMap::MIN_FILTER, osg::TextureCubeMap::LINEAR_MIPMAP_LINEAR);
	_sampleCube->setFilter(osg::TextureCubeMap::MAG_FILTER, osg::TextureCubeMap::LINEAR);
}

void CubemapProbe::setPosition(const osg::Vec3 &position)
{
	_position = position;
	_bsphere.set(_position, _radius);
	updateProbeCamera();
}

void CubemapProbe::setRadius(float radius)
{
	_radius = radius;
	_bsphere.set(_position, _radius);
	updateProbeCamera();
}

void CubemapProbe::setupPasses()
{
	// 
	// _passes->
}

void CubemapProbe::disablePasses()
{

}

void CubemapProbe::enableCompute()
{
	if (!_isInit) return;

	//for (int i = 0; i < 6; i++)
	//{
	//	if (_cameraList[i]->getNodeMask() == 0x0)
	//	{
	//		_cameraList[i]->setNodeMask(0x40);
	//	}
	//}
	_camGroup->setNodeMask(~0x0);
}

void CubemapProbe::disableCompute()
{
	if(!_isInit) return;
	/*for (int i = 0; i < 6; i++)
	{
		if (_cameraList[i]->getNodeMask() == 0x40)
		{
			_cameraList[i]->setNodeMask(0x0);
		}
	}*/

	//if (_camGroup->getNodeMask() == 0x40)
	//{
	//	_camGroup->
	//}

	_camGroup->setNodeMask(0x0);
}

void CubemapProbe::setupProbeCamera()
{
	for (int i = 0; i < 6; i++)
	{
		//osg::Camera *cam = new osg::Camera;
		//_cameraList.push_back(cam);

		//osg::Matrix projMat;
		//projMat.makePerspective(90, 1, 1, _radius);
		//cam->setProjectionMatrix(projMat);
		//cam->setViewMatrix(calcViewMatrix(i, _position));

		////cam->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		////cam->setViewport(0, 0, CAPTURE_RESOLUTION, CAPTURE_RESOLUTION); // default
		//cam->setRenderOrder(osg::Camera::PRE_RENDER, -200);
		//// cam->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
		//cam->setReferenceFrame(osg::Camera::ABSOLUTE_RF);

		//// cam->attach
		////cam->attach(osg::Camera::COLOR_BUFFER0, _sampleCube, 0, osg::TextureCubeMap::POSITIVE_X + i, false, 0, 0);
		//cam->setImplicitBufferAttachmentMask(0x0, 0x0);

		//cam->addChild(_passes);
		//_camGroup->addChild(cam);

		// rtt to cubemap
		osg::Camera *rtt = new osg::Camera;
		rtt->getOrCreateStateSet()->setAttributeAndModes(_shader, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

		rtt->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		rtt->setViewport(0, 0, CAPTURE_RESOLUTION, CAPTURE_RESOLUTION); // default
		rtt->setClearColor(osg::Vec4(1, 1, 0, 1));
		rtt->setRenderOrder(osg::Camera::PRE_RENDER, -200);
		rtt->setAllowEventFocus(false);
		rtt->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
		rtt->setProjectionMatrix(osg::Matrix::ortho2D(0.0, 1.0, 0.0, 1.0));
		rtt->setViewMatrix(osg::Matrix::identity());
		rtt->addChild(_passes->createScreenQuad(1.0, 1.0));
		// rtt->getOrCreateStateSet()->setTextureAttributeAndModes(0, _shadingBuf.get());
		rtt->attach(osg::Camera::COLOR_BUFFER, _sampleCube, 0, osg::TextureCubeMap::POSITIVE_X + i, false, 0, 0);

		_camGroup->addChild(rtt);
	}
}

void CubemapProbe::updateProbeCamera()
{
	for (int i = 0; i < 6; i++)
	{
		_cameraList[i]->setViewMatrix(calcViewMatrix(i, _position));
		osg::Matrix projMat;
		projMat.makePerspective(90, 1, 1, _radius);
		_cameraList[i]->setProjectionMatrix(projMat);
	}
}

osg::Matrix CubemapProbe::calcViewMatrix(int axis, const osg::Vec3 &pos)
{
	// std::cout << face << std::endl;
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

	if (axis < 0 || axis > 5)
	{
		return osg::Matrix();
	}

	osg::Matrix cam;
	cam.makeLookAt(pos, pos + viewDirs[axis], upDirs[axis]);

	return cam;
}

