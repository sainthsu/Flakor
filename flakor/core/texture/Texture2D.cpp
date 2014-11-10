#include "targetMacros.h"
#include "core/opengl/GL.h"
#include "core/texture/Texture2D.h"
#include "base/element/Element.h"

FLAKOR_NS_BEGIN

Texture2D::Texture2D()
{
}

Texture2D::~Texture2D()
{

}

bool Texture2D::initWithData(const void *data,ssize_t dataLen, PixelFormat pixelFormat,int width,int height,Size size)
{
	return true;
}

bool Texture2D::initWithFile(const char *fileName)
{
	return true;
}

/** Gets the pixel format of the texture */
PixelFormat Texture2D::getPixelFormat() const
{
	return _pixelFormat;
}

void Texture2D::setTexParams(const TexParams& texParams)
{
	
}

void Texture2D::setContentSize(Size *size)
{
	_contentSize = size;
}

Size* Texture2D::getContentSize()
{
	return _contentSize;
}
	
/** Gets the width of the texture in pixels */
int Texture2D::getPixelsWidth() const
{
	return _pixelsWidth;
}
    
/** Gets the height of the texture in pixels */
int Texture2D::getPixelsHeight() const
{
	return _pixelsHeight;
}
    
/** Gets the texture id*/
GLuint Texture2D::getTextureID() const
{
	return _textureID;
}
    
/** Gets max S */
GLfloat Texture2D::getMaxS() const
{
	return _maxS;
}

/** Sets max S */
void Texture2D::setMaxS(GLfloat maxS)
{
	_maxS = maxS;
}
    
/** Gets max T */
GLfloat Texture2D::getMaxT() const
{
	return _maxT;
}

/** Sets max T */
void Texture2D::setMaxT(GLfloat maxT)
{
	_maxT = maxT;
}

void Texture2D::load()
{
	
}

void Texture2D::unload()
{
	
}

FLAKOR_NS_END

