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

	struct pixelStructRGB {
		unsigned char r, g, b;
	};

	struct pixelStructRGB16 {
		short r, g, b;
	};

	struct pixelStructRGB32 {
		float r, g, b;
	};

	struct pixelStructRGBA {
		unsigned char r, g, b, a;
	};

	struct pixelStructRGBA16 {
		short r, g, b, a;
	};

	struct pixelStructRGBA32 {
		float r, g, b, a;
	};
	
	std::vector<osg::ref_ptr<osg::Image> > _cubeMapImages;
	osg::ref_ptr<osg::Image> createSubImage(osg::Image *sourceImage, int startX, int startY, int stopX, int stopY, bool rotate);
	void copyPixel(GLenum internalFormat, unsigned char *srcBuffer, unsigned char *dstBuffer,
		int currX, int currY, int subX, int subY, int subImgWidth, int subImgHeight, int srcImgWidth, bool rotate);
	unsigned char *createImageBuffer(GLenum type, int subImageWidth, int subImageHeight);
};