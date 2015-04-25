#pragma once

#include <osg/Image>
#include <string>
#include <vector>
#include <osg/TextureCubeMap>

class CubemapUtil
{
public:
	CubemapUtil();

	bool loadVerticalCross(const std::string &path);
	void saveImageToFile(const std::string &path);
	osg::ref_ptr<osg::Image> getImage(enum osg::TextureCubeMap::Face f);

private:
	void allocCubeImages(int singleImageDim);
	
	std::vector<osg::ref_ptr<osg::Image> > _cubeMapImages;
	osg::ref_ptr<osg::Image> createSubImage(osg::Image *sourceImage, int startX, int startY, int stopX, int stopY, bool rotate);
};