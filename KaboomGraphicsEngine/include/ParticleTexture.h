#pragma once

class ParticleTexture
{
public:
	ParticleTexture(osg::Image *img);

	inline int getGLTextureId()
	{
		return _glTextureId;
	}

	inline bool isCompiled()
	{
		return _isCompiled;
	}

	inline osg::ref_ptr<osg::Image> getImage()
	{
		return _image;
	}

private:
	friend class SparkDrawable;
	void setCompiledAndGLTextureId(int id);

	int _glTextureId;
	bool _isCompiled;
	osg::ref_ptr<osg::Image> _image;
};