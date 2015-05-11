#include "CubemapUtil.h"
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

CubemapUtil::CubemapUtil()
{
	_cubeMapImages.resize(6);
}

void CubemapUtil::allocCubeImages(int singleImageDim)
{
	for (int i = 0; i < 6; i++)
	{
		_cubeMapImages[i] = new osg::Image;

		// HDR
		_cubeMapImages[i]->allocateImage(singleImageDim, singleImageDim, 1, GL_RGBA16F_ARB, GL_FLOAT);
	}
}


osg::ref_ptr<osg::Image> CubemapUtil::getImage(enum osg::TextureCubeMap::Face f)
{
	// TODO: hope osg don't change this cubemap enum
	return _cubeMapImages[(int)f];
}

bool CubemapUtil::loadVerticalCross(const std::string &path)
{
	// Method referenced from CubeMapGen
	// u, v offsets of each face image in the cross, 
	// in units of faces (cross is layed out in a 3x4 grid)
	//      ___
	//     |Z+ |
	//  ___|___|___
	// |X- |Y+ |X+ |
	// |___|___|___|
	//     |Z- |
	//     |___|
	//     |Y- |
	//     |___|

	osg::ref_ptr<osg::Image> cubemap = osgDB::readImageFile(path);
	if (cubemap == NULL)
	{
		OSG_WARN << "Cubemap " + path + " read failed..." << std::endl;
		return false;
	}

	//int crossOffsets[6][2] = {
	//	{ 2, 1 },  //X+ face
	//	{ 0, 1 },  //X- face
	//	{ 1, 1 },  //Y+ face
	//	{ 1, 3 },  //Y- face
	//	{ 1, 0 },  //Z+ face
	//	{ 1, 2 },  //Z- face	
	//};

	int crossOffsets[6][2] = {
		{ 2, 2 },  //X+ face
		{ 0, 2 },  //X- face
		{ 1, 2 },  //Y+ face
		{ 1, 0 },  //Y- face
		{ 1, 3 },  //Z+ face
		{ 1, 1 },  //Z- face	
	};

	//make sure cross image is 3:4 ratio
	if ((cubemap->s() / 3) != (cubemap->t() / 4))
	{
		//texture width/height ratio must be 3/4 for cube map cross
		fprintf(stderr, "Error: texture width:height ratio must be 3:4 for cube map cross." //no comma, string continues on next line
			" Image Contained in %s is %dx%d. \n", path, cubemap->s(), cubemap->t());
		return false;
	}

	int cubeMapSize = cubemap->s() / 3;

	for (int i = 0; i < 6; i++)
	{
		int left = crossOffsets[i][0] * cubeMapSize;
		int right = (crossOffsets[i][0] + 1) * cubeMapSize;
		int top = crossOffsets[i][1] * cubeMapSize;
		int bottom = (crossOffsets[i][1] + 1) * cubeMapSize;

		osg::ref_ptr<osg::Image> img = NULL;

		if (i == 0 || i == 1)
		{
			img = createSubImage(cubemap, left, top, right, bottom, true);
			if (img == NULL) return false;

			if (i == 0)
			{
				img->flipHorizontal();
			}
			else if (i == 1)
			{
				img->flipVertical();
			}
		}
		else
		{
			img = createSubImage(cubemap, left, top, right, bottom, false);
			if (i == 5)
			{
				img->flipVertical();
				img->flipHorizontal();
			}
		}

		if (img == NULL)
		{
			return false;
		}

		_cubeMapImages[i] = img;
	}

	return true;
}

void CubemapUtil::saveImageToFile(const std::string &path)
{
	for (int i = 0; i < 6; i++)
	{
		std::string filePath = path + "\\map_" + std::to_string(i) + ".tga";
		std::cout << "CubemapUtil: Writing file " + path << "...  ";
		bool res = osgDB::writeImageFile(*_cubeMapImages[i], filePath);
		if (res) std::cout << " successfully" << std::endl;
		else std::cout << " failed..." << std::endl;
	}
}

unsigned char *CubemapUtil::createImageBuffer(GLenum internalFormat, int subImageWidth, int subImageHeight)
{
	unsigned char *targetData = NULL;
	switch (internalFormat)
	{
	case GL_RGB:
		targetData = (unsigned char *)new struct pixelStructRGB[subImageWidth * subImageHeight];
		break;
	case GL_RGBA:
		targetData = (unsigned char *)new struct pixelStructRGBA[subImageWidth * subImageHeight];
		break;
	case GL_RGB16F_ARB:
		targetData = (unsigned char *)new struct pixelStructRGB16[subImageWidth * subImageHeight];
		break;
	case GL_RGB32F_ARB:
		targetData = (unsigned char *)new struct pixelStructRGB32[subImageWidth * subImageHeight];
		break;
	case GL_RGBA16F_ARB:
		targetData = (unsigned char *)new struct pixelStructRGBA16[subImageWidth * subImageHeight];
		break;
	case GL_RGBA32F_ARB:
		targetData = (unsigned char *)new struct pixelStructRGBA32[subImageWidth * subImageHeight];
		break;
	default:
		std::cout << internalFormat << " unsupported" << std::endl;
		return NULL;
	}

	return targetData;
}

void CubemapUtil::copyPixel(GLenum internalFormat , unsigned char *srcBuffer, unsigned char *dstBuffer,
	int currX, int currY, int subX, int subY, int subImgWidth, int subImgHeight, int srcImgWidth, bool rotate)
{
	switch (internalFormat)
	{
	case GL_RGB:
	{
		struct pixelStructRGB *srcBuf = (pixelStructRGB *)srcBuffer;
		struct pixelStructRGB *dstBuf = (pixelStructRGB *)dstBuffer;
		struct pixelStructRGB *psrc = &srcBuf[currY * srcImgWidth + currX];
		struct pixelStructRGB *pdst;
		if (rotate)
		{
			pdst = &dstBuf[subX * subImgHeight + subY];
		}
		else
		{
			pdst = &dstBuf[subY * subImgWidth + subX];
		}
		*pdst = *psrc;
		break;
	}
	case GL_RGBA:
	{
		struct pixelStructRGBA *srcBuf = (pixelStructRGBA *)srcBuffer;
		struct pixelStructRGBA *dstBuf = (pixelStructRGBA *)dstBuffer;
		struct pixelStructRGBA *psrc = &srcBuf[currY * srcImgWidth + currX];
		struct pixelStructRGBA *pdst;
		if (rotate)
		{
			pdst = &dstBuf[subX * subImgHeight + subY];
		}
		else
		{
			pdst = &dstBuf[subY * subImgWidth + subX];
		}
		*pdst = *psrc;
		break;
	}
	case GL_RGB16F_ARB:
	{
		struct pixelStructRGB16 *srcBuf = (pixelStructRGB16 *)srcBuffer;
		struct pixelStructRGB16 *dstBuf = (pixelStructRGB16 *)dstBuffer;
		struct pixelStructRGB16 *psrc = &srcBuf[currY * srcImgWidth + currX];
		struct pixelStructRGB16 *pdst;
		if (rotate)
		{
			pdst = &dstBuf[subX * subImgHeight + subY];
		}
		else
		{
			pdst = &dstBuf[subY * subImgWidth + subX];
		}
		*pdst = *psrc;
		break;
	}
	case GL_RGB32F_ARB:
	{
		struct pixelStructRGB32 *srcBuf = (pixelStructRGB32 *)srcBuffer;
		struct pixelStructRGB32 *dstBuf = (pixelStructRGB32 *)dstBuffer;
		struct pixelStructRGB32 *psrc = &srcBuf[currY * srcImgWidth + currX];
		struct pixelStructRGB32 *pdst;
		if (rotate)
		{
			pdst = &dstBuf[subX * subImgHeight + subY];
		}
		else
		{
			pdst = &dstBuf[subY * subImgWidth + subX];
		}
		*pdst = *psrc;
		break;
	}
	case GL_RGBA16F_ARB:
	{
		struct pixelStructRGBA16 *srcBuf = (pixelStructRGBA16 *)srcBuffer;
		struct pixelStructRGBA16 *dstBuf = (pixelStructRGBA16 *)dstBuffer;
		struct pixelStructRGBA16 *psrc = &srcBuf[currY * srcImgWidth + currX];
		struct pixelStructRGBA16 *pdst;
		if (rotate)
		{
			pdst = &dstBuf[subX * subImgHeight + subY];
		}
		else
		{
			pdst = &dstBuf[subY * subImgWidth + subX];
		}
		*pdst = *psrc;
		break;
	}
	case GL_RGBA32F_ARB:
	{
		struct pixelStructRGBA32 *srcBuf = (pixelStructRGBA32 *)srcBuffer;
		struct pixelStructRGBA32 *dstBuf = (pixelStructRGBA32 *)dstBuffer;
		struct pixelStructRGBA32 *psrc = &srcBuf[currY * srcImgWidth + currX];
		struct pixelStructRGBA32 *pdst;
		if (rotate)
		{
			pdst = &dstBuf[subX * subImgHeight + subY];
		}
		else
		{
			pdst = &dstBuf[subY * subImgWidth + subX];
		}
		*pdst = *psrc;
		break;
	}
	default:
		std::cout << internalFormat << " unsupported" << std::endl;
		break;
	}
}

osg::ref_ptr<osg::Image> CubemapUtil::createSubImage(osg::Image *sourceImage, int startX, int startY, int stopX, int stopY, bool rotate)
{
	int subImageWidth = stopX - startX;
	int subImageHeight = stopY - startY;
	if (subImageWidth <= 0 || subImageHeight <= 0)
	{
		osg::notify(osg::ALWAYS) << "Error the subimage size is not correct. Aborting.";
		return NULL;
	}

	std::cout << "Size: " << sourceImage->s() << "x" << sourceImage->s()
		<< std::endl;
	std::cout << "SubImageSize: " << subImageWidth << "x" << subImageHeight
		<< std::endl;

	GLenum internalFormat = sourceImage->getInternalTextureFormat();
	std::cout << "Image internal format " << sourceImage->getInternalTextureFormat() << std::endl;

	unsigned char *sourceData = sourceImage->data();

	osg::ref_ptr<osg::Image> subImage = new osg::Image();

	// allocate memory for image data
	unsigned char *targetData = createImageBuffer(internalFormat, subImageWidth, subImageHeight);
	if (targetData == NULL)
	{
		return NULL;
	}

	int subX = 0;
	int subY = 0;
	for (int x = startX; x < stopX; x++) {
		for (int y = startY; y < stopY; y++) {
			/*std::cout << "Source Pixel [" << x << "][" << y << "]" << std::endl;
			std::cout << "Target Pixel [" << subX << "][" << subY << "]"
			<< std::endl;*/

			//pCurrentPixelSource = &pPixelSource[y * sourceImage->s() + x];
			//if (rotate)
			//{
			//	pCurrentPixelTarget = &pPixelTarget[subX * subImageHeight + subY];
			//}
			//else
			//{
			//	pCurrentPixelTarget = &pPixelTarget[subY * subImageWidth + subX];
			//}
			//pCurrentPixelTarget->r = pCurrentPixelSource->r;
			//pCurrentPixelTarget->g = pCurrentPixelSource->g;
			//pCurrentPixelTarget->b = pCurrentPixelSource->b;
			copyPixel(internalFormat, sourceData, targetData, x, y, subX, subY, 
				subImageWidth, subImageHeight, sourceImage->s(), rotate);
			subY++;
		}
		subY = 0;
		subX++;
	}

	GLenum dataType = sourceImage->getDataType();
	subImage->setImage(subImageWidth, subImageHeight, sourceImage->r(),
		sourceImage->getInternalTextureFormat(),
		sourceImage->getPixelFormat(), dataType, (unsigned char*)targetData,
		osg::Image::NO_DELETE);

	return subImage;
}