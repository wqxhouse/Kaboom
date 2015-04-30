#pragma once
#include <vector>
#include <osg/TextureCubeMap>

class CubeMapPreFilter
{
public:
	CubeMapPreFilter();
	CubeMapPreFilter(osg::Camera *passCam);

	inline osg::ref_ptr<osg::Group> getRoot()
	{
		return _cameraGroup;
	}

	void initWithCubeMap(osg::TextureCubeMap *cubemap);
	void changeCubeMap(osg::TextureCubeMap *cubemap);
	void enableCompute();
	void disableCompute();

	// void initWithPosition

	inline osg::ref_ptr<osg::TextureCubeMap> getGeneratedCubemap()
	{
		return _generatedCubemap;
	}

	// only for debug use
	void saveImagesToFile(const std::string &path);

private:
	void init();
	osg::Matrix calcViewMatrix(int face, const osg::Vec3 &eyePos);

	void setupCameras(int texWidth, const osg::Vec3 &eyePos);

	static osg::ref_ptr<osg::Node> _sphere;
	osg::observer_ptr<osg::TextureCubeMap> _cubemap;
	osg::ref_ptr<osg::TextureCubeMap> _generatedCubemap;
	osg::ref_ptr<osg::Group> _cameraGroup;
	std::vector<osg::ref_ptr<osg::Camera> > _cameraList;
	osg::ref_ptr<osg::Group> _switch;
	osg::observer_ptr<osg::Camera> _passCam;

	std::vector<osg::ref_ptr<osg::Image> > _images;

	osg::ref_ptr<osg::Program> _shaders;

	bool _isInit;
};