#include <android/asset_manager.h>
#include "macros.h"
#include "core/opengl/GL.h"
#include "core/opengl/GLProgram.h"
#include "core/texture/Texture2D.h"
#include "core/texture/Image.h"

FLAKOR_NS_BEGIN

namespace {
    typedef PixelFormatInfoMap::value_type PixelFormatInfoMapValue;
    static const PixelFormatInfoMapValue TexturePixelFormatInfoTablesValue[] =
    {
        PixelFormatInfoMapValue(PixelFormat::BGRA8888, PixelFormatInfo(GL_BGRA, GL_BGRA, GL_UNSIGNED_BYTE, 32, false, true)),
        PixelFormatInfoMapValue(PixelFormat::RGBA8888, PixelFormatInfo(GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, 32, false, true)),
        PixelFormatInfoMapValue(PixelFormat::RGBA4444, PixelFormatInfo(GL_RGBA, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, 16, false, true)),
        PixelFormatInfoMapValue(PixelFormat::RGB5A1, PixelFormatInfo(GL_RGBA, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, 16, false, true)),
        PixelFormatInfoMapValue(PixelFormat::RGB565, PixelFormatInfo(GL_RGB, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, 16, false, false)),
        PixelFormatInfoMapValue(PixelFormat::RGB888, PixelFormatInfo(GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, 24, false, false)),
        PixelFormatInfoMapValue(PixelFormat::A8, PixelFormatInfo(GL_ALPHA, GL_ALPHA, GL_UNSIGNED_BYTE, 8, false, false)),
        PixelFormatInfoMapValue(PixelFormat::I8, PixelFormatInfo(GL_LUMINANCE, GL_LUMINANCE, GL_UNSIGNED_BYTE, 8, false, false)),
        PixelFormatInfoMapValue(PixelFormat::AI88, PixelFormatInfo(GL_LUMINANCE_ALPHA, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, 16, false, true)),
        
#ifdef GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG
        PixelFormatInfoMapValue(PixelFormat::PVRTC2, PixelFormatInfo(GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG, 0xFFFFFFFF, 0xFFFFFFFF, 2, true, false)),
        PixelFormatInfoMapValue(PixelFormat::PVRTC2A, PixelFormatInfo(GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG, 0xFFFFFFFF, 0xFFFFFFFF, 2, true, true)),
        PixelFormatInfoMapValue(PixelFormat::PVRTC4, PixelFormatInfo(GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG, 0xFFFFFFFF, 0xFFFFFFFF, 4, true, false)),
        PixelFormatInfoMapValue(PixelFormat::PVRTC4A, PixelFormatInfo(GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG, 0xFFFFFFFF, 0xFFFFFFFF, 4, true, true)),
#endif
        
#ifdef GL_ETC1_RGB8_OES
        PixelFormatInfoMapValue(PixelFormat::ETC, PixelFormatInfo(GL_ETC1_RGB8_OES, 0xFFFFFFFF, 0xFFFFFFFF, 4, true, false)),
#endif
        
#ifdef GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
        PixelFormatInfoMapValue(PixelFormat::S3TC_DXT1, PixelFormatInfo(GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, 0xFFFFFFFF, 0xFFFFFFFF, 4, true, false)),
#endif
        
#ifdef GL_COMPRESSED_RGBA_S3TC_DXT3_EXT
        PixelFormatInfoMapValue(PixelFormat::S3TC_DXT3, PixelFormatInfo(GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, 0xFFFFFFFF, 0xFFFFFFFF, 8, true, false)),
#endif
        
#ifdef GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
        PixelFormatInfoMapValue(PixelFormat::S3TC_DXT5, PixelFormatInfo(GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, 0xFFFFFFFF, 0xFFFFFFFF, 8, true, false)),
#endif
        
#ifdef GL_ATC_RGB_AMD
        PixelFormatInfoMapValue(PixelFormat::ATC_RGB, PixelFormatInfo(GL_ATC_RGB_AMD,
            0xFFFFFFFF, 0xFFFFFFFF, 4, true, false)),
#endif
        
#ifdef GL_ATC_RGBA_EXPLICIT_ALPHA_AMD
        PixelFormatInfoMapValue(PixelFormat::ATC_EXPLICIT_ALPHA, PixelFormatInfo(GL_ATC_RGBA_EXPLICIT_ALPHA_AMD,
            0xFFFFFFFF, 0xFFFFFFFF, 8, true, false)),
#endif
        
#ifdef GL_ATC_RGBA_INTERPOLATED_ALPHA_AMD
        PixelFormatInfoMapValue(PixelFormat::ATC_INTERPOLATED_ALPHA, PixelFormatInfo(GL_ATC_RGBA_INTERPOLATED_ALPHA_AMD,
            0xFFFFFFFF, 0xFFFFFFFF, 8, true, false)),
#endif
    };
}

//The PixpelFormat corresponding information
const PixelFormatInfoMap Texture2D::_pixelFormatInfoTables(TexturePixelFormatInfoTablesValue,
                                                                     TexturePixelFormatInfoTablesValue + sizeof(TexturePixelFormatInfoTablesValue) / sizeof(TexturePixelFormatInfoTablesValue[0]));

// If the image has alpha, you can create RGBA8 (32-bit) or RGBA4 (16-bit) or RGB5A1 (16-bit)
// Default is: RGBA8888 (32-bit textures)
static PixelFormat g_defaultAlphaPixelFormat = PixelFormat::DEFAULT;

Texture2D::Texture2D()
: _pixelFormat(PixelFormat::DEFAULT)
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
	FK_SAFE_DELETE(_shaderProgram);
	if(_textureID)
	{
		glDeleteTextures(1,&_textureID);
	}
}

bool Texture2D::initWithData(const void *data,ssize_t dataLen, PixelFormat pixelFormat,int width,int height,Size size)
{
	return true;
}

bool Texture2D::initWithAsset(const char *fileName)
{
	AAssetManager* assetManager;
	_filename = fileName;
	return true;
}


bool Texture2D::initWithFile(const char *fileName)
{
	_filename = fileName;
	this->load();
	
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

    glBindTexture( GL_TEXTURE_2D,_textureID );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texParams.minFilter );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texParams.magFilter );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texParams.wrapS );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texParams.wrapT );
}

void Texture2D::setContentSize(Size& size)
{
	_contentSize = size;
}

Size Texture2D::getContentSize()
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

bool Texture2D::hasPremultipliedAlpha()
{
	return _hasPremultipliedAlpha;
}

void Texture2D::load()
{
	if(_textureID != 0)
	{
		glDeleteTextures(1,&_textureID);
	}
	
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1,&_textureID);
	glBindTexture(GL_TEXTURE_2D,_textureID);

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _antialiasEnabled ? GL_LINEAR : GL_NEAREST);

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _antialiasEnabled ? GL_LINEAR : GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	Image *image = this->loadData();
	PixelFormat pixelFormat = image->getRenderFormat();

	if(_pixelFormatInfoTables.find(pixelFormat) == _pixelFormatInfoTables.end())
    {
        FKLOG("cocos2d: WARNING: unsupported pixelformat: %lx", (unsigned long)pixelFormat );
        return ;
    }

    const PixelFormatInfo& info = _pixelFormatInfoTables.at(pixelFormat);

	
	glTexImage2D(GL_TEXTURE_2D, 0, info.internalFormat, (GLsizei)image->getWidth(), (GLsizei)image->getHeight(), 0, info.format, info.type, image->getData());
}

Image* Texture2D::loadData()
{
	unsigned char* buffer = nullptr;
    ssize_t size = 0;
    size_t readsize;
    const char* mode = "rb";
    
    do
    {
        // Read the file from hardware
        //std::string fullPath = FileUtils::getInstance()->fullPathForFilename(filename);
        FILE *fp = fopen(_filename, mode);
        FK_BREAK_IF(!fp);
        fseek(fp,0,SEEK_END);
        size = ftell(fp);
        fseek(fp,0,SEEK_SET);
        
        buffer = (unsigned char*)malloc(sizeof(unsigned char) * size);
        
        readsize = fread(buffer, sizeof(unsigned char), size, fp);
        fclose(fp);

    } while (0);
    
    if (nullptr == buffer || 0 == readsize)
    {
        std::string msg = "Get data from file(";
        msg.append(_filename).append(") failed!");
        FKLOG("%s", msg.c_str());
		return nullptr;
    }

	Image *image = new Image();
	image->initWithImageData(buffer, readsize);
	
	return image;

}

void Texture2D::unload()
{
	
}

void Texture2D::bind()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,_textureID);
}

const PixelFormatInfoMap& Texture2D::getPixelFormatInfoMap()
{
    return _pixelFormatInfoTables;
}


FLAKOR_NS_END

