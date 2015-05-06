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
	int getCubemapMaxLodLevel();

	// void initWithPosition

	inline osg::ref_ptr<osg::TextureCubeMap> getGeneratedSpecularCubemap()
	{
		return _generatedSpecularCubemap;
	}

	inline osg::ref_ptr<osg::TextureCubeMap> getGeneratedDiffuseCubemap()
	{
		return _generatedDiffuseCubemap;
	}

	// only for debug use
	void saveImagesToFile(const std::string &path);

private:
	void init();
	osg::Matrix calcViewMatrix(int face, const osg::Vec3 &eyePos);
	void setupCameras(int texWidth, const osg::Vec3 &eyePos, int &o_maxLodLevel);

	static osg::ref_ptr<osg::Node> _sphere;
	osg::observer_ptr<osg::TextureCubeMap> _cubemap;
	osg::ref_ptr<osg::TextureCubeMap> _generatedSpecularCubemap;
	osg::ref_ptr<osg::TextureCubeMap> _generatedDiffuseCubemap;
	osg::ref_ptr<osg::Group> _cameraGroup;

	osg::ref_ptr<osg::Group> _diffuseCamGroup;
	osg::ref_ptr<osg::Group> _specularCamGroup;

	std::vector<osg::ref_ptr<osg::Camera> > _cameraList;
	osg::ref_ptr<osg::Group> _switch;
	osg::observer_ptr<osg::Camera> _passCam;

	std::vector<osg::ref_ptr<osg::Image> > _images;

	osg::ref_ptr<osg::Program> _specularPrefilterShader;
	osg::ref_ptr<osg::Program> _diffusePrefilterShader;
	int _maxLodLevel;

	bool _isInit;
};