#include "ParticleTexture.h"


ParticleTexture::ParticleTexture(osg::Image *img)
	: _image(img), _isCompiled(false), _glTextureId(-1000)
{
}


void ParticleTexture::setCompiledAndGLTextureId(int id)
{
	_isCompiled = true;
	_glTextureId = id;
}