#include "macros.h"
#include "core/opengl/GL.h"
#include "core/opengl/GPUInfo.h"
#include "core/opengl/GLProgram.h"
#include "core/opengl/texture/Texture2D.h"
#include "core/resource/Image.h"
#include "tool/utility/TexUtils.h"

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
, _antialiasEnabled(true)
, _info(NULL)
, _mipmapsNum(1)
, _paramDirty(false)
, _dataDirty(false)
, _clearDataAfterLoad(false)
{
	_texParams.minFilter = GL_LINEAR;
    _texParams.magFilter = GL_LINEAR;
    _texParams.wrapS = GL_CLAMP_TO_EDGE;
    _texParams.wrapT = GL_CLAMP_TO_EDGE;
}

Texture2D::~Texture2D()
{
	if(_textureID)
	{
		glDeleteTextures(1,&_textureID);
	}
}

bool Texture2D::initWithData(const void *data,ssize_t dataLen, PixelFormat pixelFormat,int width,int height,const Size& size)
{
	FKAssert(dataLen>0 && width>0 && height>0, "Invalid size");

    //if data has no mipmaps, we will consider it has only one mipmap
    _info = new MipmapInfo();
    _info->address = (unsigned char*)data;
    _info->len = static_cast<int>(dataLen);
    _mipmapsNum = 1;
    _pixelFormat = pixelFormat;
    _pixelsWidth = width;
    _pixelsHeight = height;
    _contentSize = size;
    _dataDirty = true;
    
    return true;
}

// implementation Texture2D (Image)
bool Texture2D::initWithImage(Image *image)
{
    return initWithImage(image, g_defaultAlphaPixelFormat);
}

bool Texture2D::initWithImage(Image *image, PixelFormat format)
{
    if (image == NULL)
    {
        FKLOG("Flakor: Texture2D. Can't create Texture. Image is nil");
        return false;
    }
    
    int imageWidth = image->getWidth();
    int imageHeight = image->getHeight();
    
    GPUInfo *info = GPUInfo::getInstance();
    
    int maxTextureSize = info->getMaxTextureSize();
    if (imageWidth > maxTextureSize || imageHeight > maxTextureSize)
    {
        FKLOG("Flakor: WARNING: Image (%u x %u) is bigger than the supported %u x %u", imageWidth, imageHeight, maxTextureSize, maxTextureSize);
        return false;
    }
    
    unsigned char*   tempData = image->getData();
    Size             imageSize = Size((float)imageWidth, (float)imageHeight);
    PixelFormat      pixelFormat = ((PixelFormat::NONE == format) || (PixelFormat::AUTO == format)) ? image->getRenderFormat() : format;
    PixelFormat      renderFormat = image->getRenderFormat();
    size_t	         tempDataLen = image->getDataLen();
    _mipmapsNum = image->getNumberOfMipmaps();
    
    if (_mipmapsNum > 1)
    {
        if (pixelFormat != image->getRenderFormat())
        {
            FKLOG("Flakor: WARNING: This image has more than 1 mipmaps and we will not convert the data format");
        }
        
        _pixelFormat = image->getRenderFormat();
        _info = image->getMipmaps();
        _pixelsWidth = imageWidth;
        _pixelsHeight = imageHeight;
        _dataDirty = true;
        
        return true;
    }
    else if (image->isCompressed())
    {
        if (pixelFormat != renderFormat)
        {
            FKLOG("Flakor: WARNING: This image is compressed and we can't convert it for now");
        }
        
        initWithData(tempData, tempDataLen, renderFormat, imageWidth, imageHeight, imageSize);
        return true;
    }
    else
    {
        unsigned char* outTempData = nullptr;
        ssize_t outTempDataLen = 0;
        
        pixelFormat = TexUtils::convertDataToFormat(tempData, tempDataLen, renderFormat, pixelFormat, &outTempData, &outTempDataLen);
        
        initWithData(outTempData, outTempDataLen, pixelFormat, imageWidth, imageHeight, imageSize);
        
        // set the premultiplied tag
        _hasPremultipliedAlpha = image->hasPremultipliedAlpha();
        
        return true;
    }
}


/** Gets the pixel format of the texture */
PixelFormat Texture2D::getPixelFormat() const
{
    return _pixelFormat;
}

void Texture2D::setTexParams(const TexParams& texParams)
{
    FKAssert((_pixelsWidth == FK_NextPOT(_pixelsWidth) || texParams.wrapS == GL_CLAMP_TO_EDGE) &&
             (_pixelsHeight == FK_NextPOT(_pixelsHeight) || texParams.wrapT == GL_CLAMP_TO_EDGE),
             "GL_CLAMP_TO_EDGE should be used in NPOT dimensions");
    
    _texParams.minFilter = texParams.minFilter;
    _texParams.magFilter = texParams.magFilter;
    _texParams.wrapS = texParams.wrapS;
    _texParams.wrapT = texParams.wrapT;
    
    _paramDirty = true;
    
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

bool Texture2D::hasMipmaps() const
{
    return _mipmapsNum != 1;
}

/*********************************
 * GL METHOD
 ********************************/

bool Texture2D::loadWithMipmapsGL(MipmapInfo* mipmaps, int mipmapsNum, PixelFormat pixelFormat, int pixelsWidth, int pixelsHeight)
{

    //the pixelFormat must be a certain value 
    FKAssert(pixelFormat != PixelFormat::NONE && pixelFormat != PixelFormat::AUTO, "the \"pixelFormat\" param must be a certain value!");
    FKAssert(pixelsWidth>0 && pixelsHeight>0, "Invalid size");

    if (mipmapsNum <= 0)
    {
        FKLOG("Flakor: WARNING: mipmap number is less than 1");
        return false;
    }
    

    if(_pixelFormatInfoTables.find(pixelFormat) == _pixelFormatInfoTables.end())
    {
        FKLOG("Flakor:: WARNING: unsupported pixelformat: %lx", (unsigned long)pixelFormat );
        return false;
    }

    const PixelFormatInfo& info = _pixelFormatInfoTables.at(pixelFormat);

    if (info.compressed && !GPUInfo::getInstance()->supportsPVRTC()
                        && !GPUInfo::getInstance()->supportsETC()
                        && !GPUInfo::getInstance()->supportsS3TC()
                        && !GPUInfo::getInstance()->supportsATITC())
    {
        FKLOG("Flakor: WARNING: PVRTC/ETC images are not supported");
        return false;
    }

    //Set the row align only when mipmapsNum == 1 and the data is uncompressed
    if (mipmapsNum == 1 && !info.compressed)
    {
        unsigned int bytesPerRow = pixelsWidth * info.bpp / 8;

        if(bytesPerRow % 8 == 0)
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 8);
        }
        else if(bytesPerRow % 4 == 0)
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        }
        else if(bytesPerRow % 2 == 0)
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
        }
        else
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        }
    }
	else
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }

    if(_textureID != 0)
    {
        glDeleteTextures(1,&_textureID);
        _textureID = 0;
    }

    glGenTextures(1, &_textureID);
    glBindTexture(GL_TEXTURE_2D, _textureID);

    if (mipmapsNum == 1)
    {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _antialiasEnabled ? GL_LINEAR : GL_NEAREST);
    }else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _antialiasEnabled ? GL_LINEAR_MIPMAP_NEAREST : GL_NEAREST_MIPMAP_NEAREST);
    }
    
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _antialiasEnabled ? GL_LINEAR : GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

#if FK_ENABLE_CACHE_TEXTURE_DATA
    if (_antialiasEnabled)
    {
        TexParams texParams = {(GLuint)(_hasMipmaps?GL_LINEAR_MIPMAP_NEAREST:GL_LINEAR),GL_LINEAR,GL_NONE,GL_NONE};
        
    } 
    else
    {
        TexParams texParams = {(GLuint)(_hasMipmaps?GL_NEAREST_MIPMAP_NEAREST:GL_NEAREST),GL_NEAREST,GL_NONE,GL_NONE};
        //VolatileTextureMgr::setTexParameters(this, texParams);
    }
#endif

    CHECK_GL_ERROR_DEBUG(); // clean possible GL error
    
    // Specify OpenGL texture image
    int width = pixelsWidth;
    int height = pixelsHeight;
    
    for (int i = 0; i < mipmapsNum; ++i)
    {
        unsigned char *data = mipmaps[i].address;
        GLsizei datalen = mipmaps[i].len;

        if (info.compressed)
        {
            glCompressedTexImage2D(GL_TEXTURE_2D, i, info.internalFormat, (GLsizei)width, (GLsizei)height, 0, datalen, data);
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, i, info.internalFormat, (GLsizei)width, (GLsizei)height, 0, info.format, info.type, data);
        }

        if (i > 0 && (width != height || FK_NextPOT(width) != width ))
        {
            FKLOG("Flakor: Texture2D. WARNING. Mipmap level %u is not squared. Texture won't render correctly. width=%d != height=%d", i, width, height);
        }

        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
        {
            FKLOG("Flakor: Texture2D: Error uploading compressed texture level: %u . glError: 0x%04X", i, err);
            return false;
        }

        width = MAX(width >> 1, 1);
        height = MAX(height >> 1, 1);
    }

    _contentSize = Size((float)pixelsWidth, (float)pixelsHeight);
    
    _maxS = 1;
    _maxT = 1;

    _hasPremultipliedAlpha = false;

    // shader
    //setGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE);
    return true;
}

bool Texture2D::updateWithDataGL(const void *data,int offsetX,int offsetY,int width,int height)
{
    if (_textureID)
    {
        glBindTexture(GL_TEXTURE_2D,_textureID);
        const PixelFormatInfo& info = _pixelFormatInfoTables.at(_pixelFormat);
        glTexSubImage2D(GL_TEXTURE_2D,0,offsetX,offsetY,width,height,info.format, info.type,data);

        return true;
    }
    return false;
}

void Texture2D::generateMipmapGL()
{
    FKAssert(_pixelsWidth == FK_NextPOT(_pixelsWidth) && _pixelsHeight == FK_NextPOT(_pixelsHeight), "Mipmap texture only works in POT textures");
    glBindTexture(GL_TEXTURE_2D,_textureID);
    glGenerateMipmap(GL_TEXTURE_2D);
    if(_mipmapsNum == 1)
    {
        _mipmapsNum = -1;
    }
}

void Texture2D::loadGL()
{
    if(_paramDirty)
    {
        glBindTexture( GL_TEXTURE_2D,_textureID );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _texParams.minFilter );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _texParams.magFilter );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _texParams.wrapS );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _texParams.wrapT );
    }
    
    if (_dataDirty) {
        loadWithMipmapsGL(_info, _mipmapsNum, _pixelFormat, _pixelsWidth, _pixelsHeight);
    }
}

void Texture2D::deleteGL()
{
	if(_textureID)
	{
		glDeleteTextures(1,&_textureID);
	}
	
	_textureID = 0;
}

void Texture2D::bindGL()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,_textureID);
}

const PixelFormatInfoMap& Texture2D::getPixelFormatInfoMap()
{
    return _pixelFormatInfoTables;
}


FLAKOR_NS_END

