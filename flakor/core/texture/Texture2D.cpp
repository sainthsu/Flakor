#include "targetMacros.h"
#include "core/opengl/GL.h"
#include "core/texture/Texture2D.h"
#include "base/element/Element.h"

#include 

FLAKOR_NS_BEGIN

Texture2D::Texture2D()
: _pixelFormat(Texture2D::PixelFormat::DEFAULT)
, _pixelsWidth(0)
, _pixelsHeight(0)
, _textureID(0)
, _maxS(0.0)
, _maxT(0.0)
, _hasPremultipliedAlpha(false)
, _hasMipmaps(false)
, _shaderProgram(nullptr)
, _antialiasEnabled(true)
{
	
}

Texture2D::~Texture2D()
{
	FK_FAKE_DELETE(_shaderProgram);
	if(_textureID)
	{
		glDeleteTextures(1,&_textureID);
	}
}

bool Texture2D::initWithData(const void *data,ssize_t dataLen, PixelFormat pixelFormat,int width,int height,Size size)
{
	return true;
}

bool Texture2D::initWithFile(const char *fileName)
{
	FILE *fp = fopen(fileName,"rb");
	
	return true;
}

/** Gets the pixel format of the texture */
PixelFormat Texture2D::getPixelFormat() const
{
	return _pixelFormat;
}

void Texture2D::setTexParams(const TexParams& texParams)
{
	/*FKAssert((_pixelsWide == ccNextPOT(_pixelsWide) || texParams.wrapS == GL_CLAMP_TO_EDGE) &&
        (_pixelsHigh == ccNextPOT(_pixelsHigh) || texParams.wrapT == GL_CLAMP_TO_EDGE),
        "GL_CLAMP_TO_EDGE should be used in NPOT dimensions");
	*/

    glBindTexture2D( GL_TEXTURE_2D,_textureID );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texParams.minFilter );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texParams.magFilter );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texParams.wrapS );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texParams.wrapT );
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
	if(_textureID != 0)
	{
		glDeleteTextures(1,&_textureID);
	}
	
	glGenTextures(1,&_textureID);
	glBindTextures(TEXTURE_2D,_textureID);

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _antialiasEnabled ? GL_LINEAR : GL_NEAREST);

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _antialiasEnabled ? GL_LINEAR : GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	glTexImage2D(GL_TEXTURE_2D, i, info.internalFormat, (GLsizei)width, (GLsizei)height, 0, info.format, info.type, data);
}

void Texture2D::unload()
{
	
}

FLAKOR_NS_END

