#include "core/texture/Image.h"

#include <string>
#include <ctype.h>
#include <jpeglib.h>
#include <png.h>

#define FK_GL_ATC_RGB_AMD                                          0x8C92
#define FK_GL_ATC_RGBA_EXPLICIT_ALPHA_AMD                          0x8C93
#define FK_GL_ATC_RGBA_INTERPOLATED_ALPHA_AMD                      0x87EE

FLAKOR_NS_BEGIN

namespace
{
    typedef struct 
    {
        const unsigned char * data;
        ssize_t size;
        int offset;
    }tImageSource;
    
    static void pngReadCallback(png_structp png_ptr, png_bytep data, png_size_t length)
    {
        tImageSource* isource = (tImageSource*)png_get_io_ptr(png_ptr);
        
        if((int)(isource->offset + length) <= isource->size)
        {
            memcpy(data, isource->data+isource->offset, length);
            isource->offset += length;
        }
        else
        {
            png_error(png_ptr, "pngReaderCallback failed!");
        }
    }
}

//////////////////////////////////////////////////////////////////////////
// Implement Image
//////////////////////////////////////////////////////////////////////////

Image::Image()
: _data(nullptr)
, _dataLen(0)
, _width(0)
, _height(0)
, _unpack(false)
, _fileType(Format::UNKOWN)
, _renderFormat(Texture2D::PixelFormat::NONE)
, _numberOfMipmaps(0)
, _hasPremultipliedAlpha(true)
{

}

Image::~Image()
{
    if(_unpack)
    {
        for (unsigned int i = 0; i < _numberOfMipmaps; ++i)
            FK_SAFE_DELETE_ARRAY(_mipmaps[i].address);
    }
    else
        FK_SAFE_FREE(_data);
}

bool Image::initWithImageFile(const std::string& path)
{
    bool ret = false;
    _filePath = FileUtils::getInstance()->fullPathForFilename(path);
    Data data = FileUtils::getInstance()->getDataFromFile(_filePath);

    if (!data.isNull())
    {
        ret = initWithImageData(data.getBytes(), data.getSize());
    }

    return ret;
}

bool Image::initWithImageFileThreadSafe(const std::string& fullpath)
{
    bool ret = false;
    _filePath = fullpath;

    return ret;
}

bool Image::initWithImageData(const unsigned char * data, ssize_t dataLen)
{
    bool ret = false;
    
    do
    {
        FK_BREAK_IF(! data || dataLen <= 0);
        
        unsigned char* unpackedData = nullptr;
        ssize_t unpackedLen = 0;
        
        /**detect and unzip the compress file
        if (ZipUtils::isCCZBuffer(data, dataLen))
        {
            unpackedLen = ZipUtils::inflateCCZBuffer(data, dataLen, &unpackedData);
        }
        else if (ZipUtils::isGZipBuffer(data, dataLen))
        {
            unpackedLen = ZipUtils::inflateMemory(const_cast<unsigned char*>(data), dataLen, &unpackedData);
        }
        else
        {*/
            unpackedData = const_cast<unsigned char*>(data);
            unpackedLen = dataLen;
        //}

        _fileType = detectFormat(unpackedData, unpackedLen);

        switch (_fileType)
        {
        case Format::PNG:
            ret = initWithPngData(unpackedData, unpackedLen);
            break;
        case Format::JPG:
            ret = initWithJpgData(unpackedData, unpackedLen);
            break;
        case Format::TIFF:
            ret = initWithTiffData(unpackedData, unpackedLen);
            break;
        case Format::WEBP:
            ret = initWithWebpData(unpackedData, unpackedLen);
            break;
        case Format::PVR:
            ret = initWithPVRData(unpackedData, unpackedLen);
            break;
        case Format::ETC:
            ret = initWithETCData(unpackedData, unpackedLen);
            break;
        case Format::S3TC:
            ret = initWithS3TCData(unpackedData, unpackedLen);
            break;
        case Format::ATITC:
            ret = initWithATITCData(unpackedData, unpackedLen);
            break;
        default:
            {
                // load and detect image format
                tImageTGA* tgaData = tgaLoadBuffer(unpackedData, unpackedLen);
                
                if (tgaData != nullptr && tgaData->status == TGA_OK)
                {
                    ret = initWithTGAData(tgaData);
                }
                else
                {
                    FKAssert(false, "unsupport image format!");
                }
                
                free(tgaData);
                break;
            }
        }
        
        if(unpackedData != data)
        {
            free(unpackedData);
        }
    } while (0);
    
    return ret;
}

bool Image::isPng(const unsigned char * data, ssize_t dataLen)
{
    if (dataLen <= 8)
    {
        return false;
    }

    static const unsigned char PNG_SIGNATURE[] = {0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a};

    return memcmp(PNG_SIGNATURE, data, sizeof(PNG_SIGNATURE)) == 0;
}


bool Image::isEtc(const unsigned char * data, ssize_t dataLen)
{
    return etc1_pkm_is_valid((etc1_byte*)data) ? true : false;
}


bool Image::isS3TC(const unsigned char * data, ssize_t dataLen)
{

    S3TCTexHeader *header = (S3TCTexHeader *)data;
    
    if (strncmp(header->fileCode, "DDS", 3) != 0)
    {
        FKLOG("flakor: the file is not a dds file!");
        return false;
    }
    return true;
}

bool Image::isATITC(const unsigned char *data, ssize_t dataLen)
{
    ATITCTexHeader *header = (ATITCTexHeader *)data;
    
    if (strncmp(&header->identifier[1], "KTX", 3) != 0)
    {
        FKLOG("flakor: the file is not a ktx file!");
        return false;
    }
    return true;
}

bool Image::isJpg(const unsigned char * data, ssize_t dataLen)
{
    if (dataLen <= 4)
    {
        return false;
    }

    static const unsigned char JPG_SOI[] = {0xFF, 0xD8};

    return memcmp(data, JPG_SOI, 2) == 0;
}

bool Image::isTiff(const unsigned char * data, ssize_t dataLen)
{
    if (dataLen <= 4)
    {
        return false;
    }

    static const char* TIFF_II = "II";
    static const char* TIFF_MM = "MM";

    return (memcmp(data, TIFF_II, 2) == 0 && *(static_cast<const unsigned char*>(data) + 2) == 42 && *(static_cast<const unsigned char*>(data) + 3) == 0) ||
        (memcmp(data, TIFF_MM, 2) == 0 && *(static_cast<const unsigned char*>(data) + 2) == 0 && *(static_cast<const unsigned char*>(data) + 3) == 42);
}

bool Image::isWebp(const unsigned char * data, ssize_t dataLen)
{
    if (dataLen <= 12)
    {
        return false;
    }

    static const char* WEBP_RIFF = "RIFF";
    static const char* WEBP_WEBP = "WEBP";

    return memcmp(data, WEBP_RIFF, 4) == 0 
        && memcmp(static_cast<const unsigned char*>(data) + 8, WEBP_WEBP, 4) == 0;
}

bool Image::isPvr(const unsigned char * data, ssize_t dataLen)
{
    if (static_cast<size_t>(dataLen) < sizeof(PVRv2TexHeader) || static_cast<size_t>(dataLen) < sizeof(PVRv3TexHeader))
    {
        return false;
    }
    
    const PVRv2TexHeader* headerv2 = static_cast<const PVRv2TexHeader*>(static_cast<const void*>(data));
    const PVRv3TexHeader* headerv3 = static_cast<const PVRv3TexHeader*>(static_cast<const void*>(data));
    
    return memcmp(&headerv2->pvrTag, gPVRTexIdentifier, strlen(gPVRTexIdentifier)) == 0 || CC_SWAP_INT32_BIG_TO_HOST(headerv3->version) == 0x50565203;
}

Image::Format Image::detectFormat(const unsigned char * data, ssize_t dataLen)
{
    if (isPng(data, dataLen))
    {
        return Format::PNG;
    }
    else if (isJpg(data, dataLen))
    {
        return Format::JPG;
    }
    else if (isTiff(data, dataLen))
    {
        return Format::TIFF;
    }
    else if (isWebp(data, dataLen))
    {
        return Format::WEBP;
    }
    else if (isPvr(data, dataLen))
    {
        return Format::PVR;
    }
    else if (isEtc(data, dataLen))
    {
        return Format::ETC;
    }
    else if (isS3TC(data, dataLen))
    {
        return Format::S3TC;
    }
    else if (isATITC(data, dataLen))
    {
        return Format::ATITC;
    }
    else
    {
        return Format::UNKOWN;
    }
}

int Image::getBitPerPixel()
{
    return Texture2D::getPixelFormatInfoMap().at(_renderFormat).bpp;
}

bool Image::hasAlpha()
{
    return Texture2D::getPixelFormatInfoMap().at(_renderFormat).alpha;
}

bool Image::isCompressed()
{
    return Texture2D::getPixelFormatInfoMap().at(_renderFormat).compressed;
}

namespace
{
/*
 * ERROR HANDLING:
 *
 * The JPEG library's standard error handler (jerror.c) is divided into
 * several "methods" which you can override individually.  This lets you
 * adjust the behavior without duplicating a lot of code, which you might
 * have to update with each future release.
 *
 * We override the "error_exit" method so that control is returned to the
 * library's caller when a fatal error occurs, rather than calling exit()
 * as the standard error_exit method does.
 *
 * We use C's setjmp/longjmp facility to return control.  This means that the
 * routine which calls the JPEG library must first execute a setjmp() call to
 * establish the return point.  We want the replacement error_exit to do a
 * longjmp().  But we need to make the setjmp buffer accessible to the
 * error_exit routine.  To do this, we make a private extension of the
 * standard JPEG error handler object.  (If we were using C++, we'd say we
 * were making a subclass of the regular error handler.)
 *
 * Here's the extended error handler struct:
 */
#if FK_USE_JPEG
    struct MyErrorMgr
    {
        struct jpeg_error_mgr pub;	/* "public" fields */
        jmp_buf setjmp_buffer;	/* for return to caller */
    };
    
    typedef struct MyErrorMgr * MyErrorPtr;
    
    /*
     * Here's the routine that will replace the standard error_exit method:
     */
    
    METHODDEF(void)
    myErrorExit(j_common_ptr cinfo)
    {
        /* cinfo->err really points to a MyErrorMgr struct, so coerce pointer */
        MyErrorPtr myerr = (MyErrorPtr) cinfo->err;
        
        /* Always display the message. */
        /* We could postpone this until after returning, if we chose. */
        /* internal message function cann't show error message in some platforms, so we rewrite it here.
         * edit it if has version confilict.
         */
        //(*cinfo->err->output_message) (cinfo);
        char buffer[JMSG_LENGTH_MAX];
        (*cinfo->err->format_message) (cinfo, buffer);
        FKLOG("jpeg error: %s", buffer);
        
        /* Return control to the setjmp point */
        longjmp(myerr->setjmp_buffer, 1);
    }
#endif // CC_USE_JPEG
}

bool Image::initWithJpgData(const unsigned char * data, ssize_t dataLen)
{
#if FK_USE_JPEG
    /* these are standard libjpeg structures for reading(decompression) */
    struct jpeg_decompress_struct cinfo;
    /* We use our private extension JPEG error handler.
	 * Note that this struct must live as long as the main JPEG parameter
	 * struct, to avoid dangling-pointer problems.
	 */
	struct MyErrorMgr jerr;
    /* libjpeg data structure for storing one row, that is, scanline of an image */
    JSAMPROW row_pointer[1] = {0};
    unsigned long location = 0;

    bool ret = false;
    do 
    {
        /* We set up the normal JPEG error routines, then override error_exit. */
		cinfo.err = jpeg_std_error(&jerr.pub);
		jerr.pub.error_exit = myErrorExit;
		/* Establish the setjmp return context for MyErrorExit to use. */
		if (setjmp(jerr.setjmp_buffer))
        {
			/* If we get here, the JPEG code has signaled an error.
			 * We need to clean up the JPEG object, close the input file, and return.
			 */
			jpeg_destroy_decompress(&cinfo);
			break;
		}

        /* setup decompression process and source, then read JPEG header */
        jpeg_create_decompress( &cinfo );

#ifndef FK_TARGET_QT5
        jpeg_mem_src(&cinfo, const_cast<unsigned char*>(data), dataLen);
#endif /* FK_TARGET_QT5 */

        /* reading the image header which contains image information */
#if (JPEG_LIB_VERSION >= 90)
        // libjpeg 0.9 adds stricter types.
        jpeg_read_header(&cinfo, TRUE);
#else
        jpeg_read_header(&cinfo, TRUE);
#endif

        // we only support RGB or grayscale
        if (cinfo.jpeg_color_space == JCS_GRAYSCALE)
        {
            _renderFormat = Texture2D::PixelFormat::I8;
        }else
        {
            cinfo.out_color_space = JCS_RGB;
            _renderFormat = Texture2D::PixelFormat::RGB888;
        }

        /* Start decompression jpeg here */
        jpeg_start_decompress( &cinfo );

        /* init image info */
        _width  = cinfo.output_width;
        _height = cinfo.output_height;
        _hasPremultipliedAlpha = false;

        _dataLen = cinfo.output_width*cinfo.output_height*cinfo.output_components;
        _data = static_cast<unsigned char*>(malloc(_dataLen * sizeof(unsigned char)));
        FK_BREAK_IF(! _data);

        /* now actually read the jpeg into the raw buffer */
        /* read one scan line at a time */
        while (cinfo.output_scanline < cinfo.output_height)
        {
            row_pointer[0] = _data + location;
            location += cinfo.output_width*cinfo.output_components;
            jpeg_read_scanlines(&cinfo, row_pointer, 1);
        }

	/* When read image file with broken data, jpeg_finish_decompress() may cause error.
	 * Besides, jpeg_destroy_decompress() shall deallocate and release all memory associated
	 * with the decompression object.
	 * So it doesn't need to call jpeg_finish_decompress().
	 */
	//jpeg_finish_decompress( &cinfo );
        jpeg_destroy_decompress( &cinfo );
        /* wrap up decompression, destroy objects, free pointers and close open files */        
        ret = true;
    } while (0);

    return ret;
#else
    return false;
#endif // FK_USE_JPEG
}

bool Image::initWithPngData(const unsigned char * data, ssize_t dataLen)
{
    // length of bytes to check if it is a valid png file
#define PNGSIGSIZE  8
    bool ret = false;
    png_byte        header[PNGSIGSIZE]   = {0}; 
    png_structp     png_ptr     =   0;
    png_infop       info_ptr    = 0;

    do 
    {
        // png header len is 8 bytes
        FK_BREAK_IF(dataLen < PNGSIGSIZE);

        // check the data is png or not
        memcpy(header, data, PNGSIGSIZE);
        FK_BREAK_IF(png_sig_cmp(header, 0, PNGSIGSIZE));

        // init png_struct
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
        FK_BREAK_IF(! png_ptr);

        // init png_info
        info_ptr = png_create_info_struct(png_ptr);
        FK_BREAK_IF(!info_ptr);

#if (FK_TARGET_PLATFORM != FK_PLATFORM_BADA && FK_TARGET_PLATFORM != FK_PLATFORM_NACL)
        FK_BREAK_IF(setjmp(png_jmpbuf(png_ptr)));
#endif

        // set the read call back function
        tImageSource imageSource;
        imageSource.data    = (unsigned char*)data;
        imageSource.size    = dataLen;
        imageSource.offset  = 0;
        png_set_read_fn(png_ptr, &imageSource, pngReadCallback);

        // read png header info

        // read png file info
        png_read_info(png_ptr, info_ptr);

        _width = png_get_image_width(png_ptr, info_ptr);
        _height = png_get_image_height(png_ptr, info_ptr);
        png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
        png_uint_32 color_type = png_get_color_type(png_ptr, info_ptr);

        //FKLOG("color type %u", color_type);

        // force palette images to be expanded to 24-bit RGB
        // it may include alpha channel
        if (color_type == PNG_COLOR_TYPE_PALETTE)
        {
            png_set_palette_to_rgb(png_ptr);
        }
        // low-bit-depth grayscale images are to be expanded to 8 bits
        if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        {
            bit_depth = 8;
            png_set_expand_gray_1_2_4_to_8(png_ptr);
        }
        // expand any tRNS chunk data into a full alpha channel
        if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
        {
            png_set_tRNS_to_alpha(png_ptr);
        }  
        // reduce images with 16-bit samples to 8 bits
        if (bit_depth == 16)
        {
            png_set_strip_16(png_ptr);            
        } 

        // Expanded earlier for grayscale, now take care of palette and rgb
        if (bit_depth < 8)
        {
            png_set_packing(png_ptr);
        }
        // update info
        png_read_update_info(png_ptr, info_ptr);
        bit_depth = png_get_bit_depth(png_ptr, info_ptr);
        color_type = png_get_color_type(png_ptr, info_ptr);

        switch (color_type)
        {
        case PNG_COLOR_TYPE_GRAY:
            _renderFormat = Texture2D::PixelFormat::I8;
            break;
        case PNG_COLOR_TYPE_GRAY_ALPHA:
            _renderFormat = Texture2D::PixelFormat::AI88;
            break;
        case PNG_COLOR_TYPE_RGB:
            _renderFormat = Texture2D::PixelFormat::RGB888;
            break;
        case PNG_COLOR_TYPE_RGB_ALPHA:
            _renderFormat = Texture2D::PixelFormat::RGBA8888;
            break;
        default:
            break;
        }

        // read png data
        png_size_t rowbytes;
        png_bytep* row_pointers = (png_bytep*)malloc( sizeof(png_bytep) * _height );

        rowbytes = png_get_rowbytes(png_ptr, info_ptr);

        _dataLen = rowbytes * _height;
        _data = static_cast<unsigned char*>(malloc(_dataLen * sizeof(unsigned char)));
        if (!_data)
        {
            if (row_pointers != nullptr)
            {
                free(row_pointers);
            }
            break;
        }

        for (unsigned short i = 0; i < _height; ++i)
        {
            row_pointers[i] = _data + i*rowbytes;
        }
        png_read_image(png_ptr, row_pointers);

        png_read_end(png_ptr, nullptr);

        // premultiplied alpha for RGBA8888
        if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
        {
            premultipliedAlpha();
        }
        else
        {
            _hasPremultipliedAlpha = false;
        }

        if (row_pointers != nullptr)
        {
            free(row_pointers);
        }

        ret = true;
    } while (0);

    if (png_ptr)
    {
        png_destroy_read_struct(&png_ptr, (info_ptr) ? &info_ptr : 0, 0);
    }
    return ret;
}

#if FK_USE_TIFF
namespace
{
    static tmsize_t tiffReadProc(thandle_t fd, void* buf, tmsize_t size)
    {
        tImageSource* isource = (tImageSource*)fd;
        uint8* ma;
        uint64 mb;
        unsigned long n;
        unsigned long o;
        tmsize_t p;
        ma=(uint8*)buf;
        mb=size;
        p=0;
        while (mb>0)
        {
            n=0x80000000UL;
            if ((uint64)n>mb)
                n=(unsigned long)mb;
            
            
            if ((int)(isource->offset + n) <= isource->size)
            {
                memcpy(ma, isource->data+isource->offset, n);
                isource->offset += n;
                o = n;
            }
            else
            {
                return 0;
            }
            
            ma+=o;
            mb-=o;
            p+=o;
            if (o!=n)
            {
                break;
            }
        }
        return p;
    }
    
    static tmsize_t tiffWriteProc(thandle_t fd, void* buf, tmsize_t size)
    {
        FK_UNUSED_PARAM(fd);
        FK_UNUSED_PARAM(buf);
        FK_UNUSED_PARAM(size);
        return 0;
    }
    
    
    static uint64 tiffSeekProc(thandle_t fd, uint64 off, int whence)
    {
        tImageSource* isource = (tImageSource*)fd;
        uint64 ret = -1;
        do
        {
            if (whence == SEEK_SET)
            {
                FK_BREAK_IF(off >= (uint64)isource->size);
                ret = isource->offset = (uint32)off;
            }
            else if (whence == SEEK_CUR)
            {
                FK_BREAK_IF(isource->offset + off >= (uint64)isource->size);
                ret = isource->offset += (uint32)off;
            }
            else if (whence == SEEK_END)
            {
                FK_BREAK_IF(off >= (uint64)isource->size);
                ret = isource->offset = (uint32)(isource->size-1 - off);
            }
            else
            {
                FK_BREAK_IF(off >= (uint64)isource->size);
                ret = isource->offset = (uint32)off;
            }
        } while (0);
        
        return ret;
    }
    
    static uint64 tiffSizeProc(thandle_t fd)
    {
        tImageSource* imageSrc = (tImageSource*)fd;
        return imageSrc->size;
    }
    
    static int tiffCloseProc(thandle_t fd)
    {
        FK_UNUSED_PARAM(fd);
        return 0;
    }
    
    static int tiffMapProc(thandle_t fd, void** base, toff_t* size)
    {
        FK_UNUSED_PARAM(fd);
        FK_UNUSED_PARAM(base);
        FK_UNUSED_PARAM(size);
        return 0;
    }
    
    static void tiffUnmapProc(thandle_t fd, void* base, toff_t size)
    {
        FK_UNUSED_PARAM(fd);
        FK_UNUSED_PARAM(base);
        FK_UNUSED_PARAM(size);
    }
}
#endif // CC_USE_TIFF

bool Image::initWithTiffData(const unsigned char * data, ssize_t dataLen)
{
#if FK_USE_TIFF
    bool ret = false;
    do 
    {
        // set the read call back function
        tImageSource imageSource;
        imageSource.data    = data;
        imageSource.size    = dataLen;
        imageSource.offset  = 0;

        TIFF* tif = TIFFClientOpen("file.tif", "r", (thandle_t)&imageSource, 
            tiffReadProc, tiffWriteProc,
            tiffSeekProc, tiffCloseProc, tiffSizeProc,
            tiffMapProc,
            tiffUnmapProc);

        FK_BREAK_IF(nullptr == tif);

        uint32 w = 0, h = 0;
        uint16 bitsPerSample = 0, samplePerPixel = 0, planarConfig = 0;
        size_t npixels = 0;
        
        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);
        TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitsPerSample);
        TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplePerPixel);
        TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &planarConfig);

        npixels = w * h;
        
        _renderFormat = Texture2D::PixelFormat::RGBA8888;
        _width = w;
        _height = h;

        _dataLen = npixels * sizeof (uint32);
        _data = static_cast<unsigned char*>(malloc(_dataLen * sizeof(unsigned char)));

        uint32* raster = (uint32*) _TIFFmalloc(npixels * sizeof (uint32));
        if (raster != nullptr) 
        {
           if (TIFFReadRGBAImageOriented(tif, w, h, raster, ORIENTATION_TOPLEFT, 0))
           {
                /* the raster data is pre-multiplied by the alpha component 
                   after invoking TIFFReadRGBAImageOriented*/
                _hasPremultipliedAlpha = true;

               memcpy(_data, raster, npixels*sizeof (uint32));
           }

          _TIFFfree(raster);
        }
        

        TIFFClose(tif);

        ret = true;
    } while (0);
    return ret;
#else
    FKLOG("tiff is not enabled, please enalbe it in ccConfig.h");
    return false;
#endif
}

namespace
{
    bool testFormatForPvr2TCSupport(PVR2TexturePixelFormat format)
    {
        return true;
    }
    
    bool testFormatForPvr3TCSupport(PVR3TexturePixelFormat format)
    {
        switch (format) {
            case PVR3TexturePixelFormat::DXT1:
            case PVR3TexturePixelFormat::DXT3:
            case PVR3TexturePixelFormat::DXT5:
                return Configuration::getInstance()->supportsS3TC();
                
            case PVR3TexturePixelFormat::BGRA8888:
                return Configuration::getInstance()->supportsBGRA8888();
                
            case PVR3TexturePixelFormat::PVRTC2BPP_RGB:
            case PVR3TexturePixelFormat::PVRTC2BPP_RGBA:
            case PVR3TexturePixelFormat::PVRTC4BPP_RGB:
            case PVR3TexturePixelFormat::PVRTC4BPP_RGBA:
            case PVR3TexturePixelFormat::ETC1:
            case PVR3TexturePixelFormat::RGBA8888:
            case PVR3TexturePixelFormat::RGBA4444:
            case PVR3TexturePixelFormat::RGBA5551:
            case PVR3TexturePixelFormat::RGB565:
            case PVR3TexturePixelFormat::RGB888:
            case PVR3TexturePixelFormat::A8:
            case PVR3TexturePixelFormat::L8:
            case PVR3TexturePixelFormat::LA88:
                return true;
                
            default:
                return false;
        }
    }
}

bool Image::initWithPVRv2Data(const unsigned char * data, ssize_t dataLen)
{
    int dataLength = 0, dataOffset = 0, dataSize = 0;
    int blockSize = 0, widthBlocks = 0, heightBlocks = 0;
    int width = 0, height = 0;
    
    //Cast first sizeof(PVRTexHeader) bytes of data stream as PVRTexHeader
    const PVRv2TexHeader *header = static_cast<const PVRv2TexHeader *>(static_cast<const void*>(data));
    
    //Make sure that tag is in correct formatting
    if (memcmp(&header->pvrTag, gPVRTexIdentifier, strlen(gPVRTexIdentifier)) != 0)
    {
        return false;
    }
    
    Configuration *configuration = Configuration::getInstance();
    
    //can not detect the premultiplied alpha from pvr file, use _PVRHaveAlphaPremultiplied instead.
    _hasPremultipliedAlpha = _PVRHaveAlphaPremultiplied;
    
    unsigned int flags = CC_SWAP_INT32_LITTLE_TO_HOST(header->flags);
    PVR2TexturePixelFormat formatFlags = static_cast<PVR2TexturePixelFormat>(flags & PVR_TEXTURE_FLAG_TYPE_MASK);
    bool flipped = (flags & (unsigned int)PVR2TextureFlag::VerticalFlip) ? true : false;
    if (flipped)
    {
        FKLOG("cocos2d: WARNING: Image is flipped. Regenerate it using PVRTexTool");
    }
    
    if (! configuration->supportsNPOT() &&
        (static_cast<int>(header->width) != ccNextPOT(header->width)
            || static_cast<int>(header->height) != ccNextPOT(header->height)))
    {
        FKLOG("cocos2d: ERROR: Loading an NPOT texture (%dx%d) but is not supported on this device", header->width, header->height);
        return false;
    }
    
    if (!testFormatForPvr2TCSupport(formatFlags))
    {
        FKLOG("cocos2d: WARNING: Unsupported PVR Pixel Format: 0x%02X. Re-encode it with a OpenGL pixel format variant", (int)formatFlags);
        return false;
    }

    if (v2_pixel_formathash.find(formatFlags) == v2_pixel_formathash.end())
    {
        FKLOG("cocos2d: WARNING: Unsupported PVR Pixel Format: 0x%02X. Re-encode it with a OpenGL pixel format variant", (int)formatFlags);
        return false;
    }
    
    auto it = Texture2D::getPixelFormatInfoMap().find(getDevicePixelFormat(v2_pixel_formathash.at(formatFlags)));

    if (it == Texture2D::getPixelFormatInfoMap().end())
    {
        FKLOG("cocos2d: WARNING: Unsupported PVR Pixel Format: 0x%02X. Re-encode it with a OpenGL pixel format variant", (int)formatFlags);
        return false;
    }

    _renderFormat = it->first;
    int bpp = it->second.bpp;

    //Reset num of mipmaps
    _numberOfMipmaps = 0;

    //Get size of mipmap
    _width = width = CC_SWAP_INT32_LITTLE_TO_HOST(header->width);
    _height = height = CC_SWAP_INT32_LITTLE_TO_HOST(header->height);

    //Get ptr to where data starts..
    dataLength = CC_SWAP_INT32_LITTLE_TO_HOST(header->dataLength);

    //Move by size of header
    _dataLen = dataLen - sizeof(PVRv2TexHeader);
    _data = static_cast<unsigned char*>(malloc(_dataLen * sizeof(unsigned char)));
    memcpy(_data, (unsigned char*)data + sizeof(PVRv2TexHeader), _dataLen);

    // Calculate the data size for each texture level and respect the minimum number of blocks
    while (dataOffset < dataLength)
    {
        switch (formatFlags) {
            case PVR2TexturePixelFormat::PVRTC2BPP_RGBA:
                if (!Configuration::getInstance()->supportsPVRTC())
                {
                    FKLOG("cocos2d: Hardware PVR decoder not present. Using software decoder");
                    _unpack = true;
                    _mipmaps[_numberOfMipmaps].len = width*height*4;
                    _mipmaps[_numberOfMipmaps].address = new unsigned char[width*height*4];
                    PVRTDecompressPVRTC(_data+dataOffset,width,height,_mipmaps[_numberOfMipmaps].address, true);
                    bpp = 2;
                }
                blockSize = 8 * 4; // Pixel by pixel block size for 2bpp
                widthBlocks = width / 8;
                heightBlocks = height / 4;
                break;
            case PVR2TexturePixelFormat::PVRTC4BPP_RGBA:
                if (!Configuration::getInstance()->supportsPVRTC())
                {
                    CCLOG("cocos2d: Hardware PVR decoder not present. Using software decoder");
                    _unpack = true;
                    _mipmaps[_numberOfMipmaps].len = width*height*4;
                    _mipmaps[_numberOfMipmaps].address = new unsigned char[width*height*4];
                    PVRTDecompressPVRTC(_data+dataOffset,width,height,_mipmaps[_numberOfMipmaps].address, false);
                    bpp = 4;
                }
                blockSize = 4 * 4; // Pixel by pixel block size for 4bpp
                widthBlocks = width / 4;
                heightBlocks = height / 4;
                break;
            case PVR2TexturePixelFormat::BGRA8888:
                if (Configuration::getInstance()->supportsBGRA8888() == false)
                {
                    CCLOG("cocos2d: Image. BGRA8888 not supported on this device");
                    return false;
                }
            default:
                blockSize = 1;
                widthBlocks = width;
                heightBlocks = height;
                break;
        }
        
        // Clamp to minimum number of blocks
        if (widthBlocks < 2)
        {
            widthBlocks = 2;
        }
        if (heightBlocks < 2)
        {
            heightBlocks = 2;
        }
        
        dataSize = widthBlocks * heightBlocks * ((blockSize  * bpp) / 8);
        int packetLength = (dataLength - dataOffset);
        packetLength = packetLength > dataSize ? dataSize : packetLength;
        
        //Make record to the mipmaps array and increment counter
        if(!_unpack)
        {
            _mipmaps[_numberOfMipmaps].address = _data + dataOffset;
            _mipmaps[_numberOfMipmaps].len = packetLength;
        }
        _numberOfMipmaps++;
        
        dataOffset += packetLength;
        
        //Update width and height to the next lower power of two
        width = MAX(width >> 1, 1);
        height = MAX(height >> 1, 1);
    }
    
    if(_unpack)
    {
        _data = _mipmaps[0].address;
        _dataLen = _mipmaps[0].len;
    }

    return true;
}

bool Image::initWithPVRv3Data(const unsigned char * data, ssize_t dataLen)
{
    if (static_cast<size_t>(dataLen) < sizeof(PVRv3TexHeader))
    {
		return false;
	}
	
	const PVRv3TexHeader *header = static_cast<const PVRv3TexHeader *>(static_cast<const void*>(data));
	
	// validate version
	if (CC_SWAP_INT32_BIG_TO_HOST(header->version) != 0x50565203)
    {
		CCLOG("cocos2d: WARNING: pvr file version mismatch");
		return false;
	}
	
	// parse pixel format
	PVR3TexturePixelFormat pixelFormat = static_cast<PVR3TexturePixelFormat>(header->pixelFormat);
    
    if (!testFormatForPvr3TCSupport(pixelFormat))
    {
        CCLOG("cocos2d: WARNING: Unsupported PVR Pixel Format: 0x%016llX. Re-encode it with a OpenGL pixel format variant",
              static_cast<unsigned long long>(pixelFormat));
        return false;
    }


    if (v3_pixel_formathash.find(pixelFormat) == v3_pixel_formathash.end())
    {
        CCLOG("cocos2d: WARNING: Unsupported PVR Pixel Format: 0x%016llX. Re-encode it with a OpenGL pixel format variant",
              static_cast<unsigned long long>(pixelFormat));
        return false;
    }

    auto it = Texture2D::getPixelFormatInfoMap().find(getDevicePixelFormat(v3_pixel_formathash.at(pixelFormat)));

    if (it == Texture2D::getPixelFormatInfoMap().end())
    {
        CCLOG("cocos2d: WARNING: Unsupported PVR Pixel Format: 0x%016llX. Re-encode it with a OpenGL pixel format variant",
              static_cast<unsigned long long>(pixelFormat));
        return false;
    }

    _renderFormat = it->first;
    int bpp = it->second.bpp;
    
    // flags
	int flags = CC_SWAP_INT32_LITTLE_TO_HOST(header->flags);

    // PVRv3 specifies premultiply alpha in a flag -- should always respect this in PVRv3 files
    if (flags & (unsigned int)PVR3TextureFlag::PremultipliedAlpha)
    {
        _hasPremultipliedAlpha = true;
    }
    
	// sizing
	int width = CC_SWAP_INT32_LITTLE_TO_HOST(header->width);
	int height = CC_SWAP_INT32_LITTLE_TO_HOST(header->height);
	_width = width;
	_height = height;
	int dataOffset = 0, dataSize = 0;
	int blockSize = 0, widthBlocks = 0, heightBlocks = 0;
	
    _dataLen = dataLen - (sizeof(PVRv3TexHeader) + header->metadataLength);
    _data = static_cast<unsigned char*>(malloc(_dataLen * sizeof(unsigned char)));
    memcpy(_data, static_cast<const unsigned char*>(data) + sizeof(PVRv3TexHeader) + header->metadataLength, _dataLen);
	
	_numberOfMipmaps = header->numberOfMipmaps;
	CCAssert(_numberOfMipmaps < MIPMAP_MAX, "Image: Maximum number of mimpaps reached. Increate the CC_MIPMAP_MAX value");
    
	for (int i = 0; i < _numberOfMipmaps; i++)
    {
		switch ((PVR3TexturePixelFormat)pixelFormat)
        {
            case PVR3TexturePixelFormat::PVRTC2BPP_RGB :
            case PVR3TexturePixelFormat::PVRTC2BPP_RGBA :
                if (!Configuration::getInstance()->supportsPVRTC())
                {
                    CCLOG("cocos2d: Hardware PVR decoder not present. Using software decoder");
                    _unpack = true;
                    _mipmaps[i].len = width*height*4;
                    _mipmaps[i].address = new unsigned char[width*height*4];
                    PVRTDecompressPVRTC(_data+dataOffset,width,height,_mipmaps[i].address, true);
                    bpp = 2;
                }
                blockSize = 8 * 4; // Pixel by pixel block size for 2bpp
                widthBlocks = width / 8;
                heightBlocks = height / 4;
                break;
            case PVR3TexturePixelFormat::PVRTC4BPP_RGB :
            case PVR3TexturePixelFormat::PVRTC4BPP_RGBA :
                if (!Configuration::getInstance()->supportsPVRTC())
                {
                    CCLOG("cocos2d: Hardware PVR decoder not present. Using software decoder");
                    _unpack = true;
                    _mipmaps[i].len = width*height*4;
                    _mipmaps[i].address = new unsigned char[width*height*4];
                    PVRTDecompressPVRTC(_data+dataOffset,width,height,_mipmaps[i].address, false);
                    bpp = 4;
                }
                blockSize = 4 * 4; // Pixel by pixel block size for 4bpp
                widthBlocks = width / 4;
                heightBlocks = height / 4;
                break;
            case PVR3TexturePixelFormat::ETC1:
                if (!Configuration::getInstance()->supportsETC())
                {
                    CCLOG("cocos2d: Hardware ETC1 decoder not present. Using software decoder");
                    int bytePerPixel = 3;
                    unsigned int stride = width * bytePerPixel;
                    _unpack = true;
                    _mipmaps[i].len = width*height*bytePerPixel;
                    _mipmaps[i].address = new unsigned char[width*height*bytePerPixel];
                    if (etc1_decode_image(static_cast<const unsigned char*>(_data+dataOffset), static_cast<etc1_byte*>(_mipmaps[i].address), width, height, bytePerPixel, stride) != 0)
                    {
                        return false;
                    }
                }
                blockSize = 4 * 4; // Pixel by pixel block size for 4bpp
                widthBlocks = width / 4;
                heightBlocks = height / 4;
                break;
            case PVR3TexturePixelFormat::BGRA8888:
                if (! Configuration::getInstance()->supportsBGRA8888())
                {
                    CCLOG("cocos2d: Image. BGRA8888 not supported on this device");
                    return false;
                }
            default:
                blockSize = 1;
                widthBlocks = width;
                heightBlocks = height;
                break;
		}
        
		// Clamp to minimum number of blocks
		if (widthBlocks < 2)
        {
			widthBlocks = 2;
        }
		if (heightBlocks < 2)
        {
			heightBlocks = 2;
        }
		
		dataSize = widthBlocks * heightBlocks * ((blockSize  * bpp) / 8);
		auto packetLength = _dataLen - dataOffset;
		packetLength = packetLength > dataSize ? dataSize : packetLength;
		
		if(!_unpack)
		{
			_mipmaps[i].address = _data + dataOffset;
			_mipmaps[i].len = static_cast<int>(packetLength);
		}
		
		dataOffset += packetLength;
		CCAssert(dataOffset <= _dataLen, "CCTexurePVR: Invalid lenght");
		
		
		width = MAX(width >> 1, 1);
		height = MAX(height >> 1, 1);
	}
    
    if (_unpack)
    {
        _data = _mipmaps[0].address;
        _dataLen = _mipmaps[0].len;
    }
	
	return true;
}

bool Image::initWithETCData(const unsigned char * data, ssize_t dataLen)
{
    const etc1_byte* header = static_cast<const etc1_byte*>(data);
    
    //check the data
    if (! etc1_pkm_is_valid(header))
    {
        return  false;
    }

    _width = etc1_pkm_get_width(header);
    _height = etc1_pkm_get_height(header);

    if (0 == _width || 0 == _height)
    {
        return false;
    }

    if (Configuration::getInstance()->supportsETC())
    {
        //old opengl version has no define for GL_ETC1_RGB8_OES, add macro to make compiler happy. 
#ifdef GL_ETC1_RGB8_OES
        _renderFormat = Texture2D::PixelFormat::ETC;
        _dataLen = dataLen - ETC_PKM_HEADER_SIZE;
        _data = static_cast<unsigned char*>(malloc(_dataLen * sizeof(unsigned char)));
        memcpy(_data, static_cast<const unsigned char*>(data) + ETC_PKM_HEADER_SIZE, _dataLen);
        return true;
#endif
    }
    else
    {
        CCLOG("cocos2d: Hardware ETC1 decoder not present. Using software decoder");

         //if it is not gles or device do not support ETC, decode texture by software
        int bytePerPixel = 3;
        unsigned int stride = _width * bytePerPixel;
        _renderFormat = Texture2D::PixelFormat::RGB888;
        
        _dataLen =  _width * _height * bytePerPixel;
        _data = static_cast<unsigned char*>(malloc(_dataLen * sizeof(unsigned char)));
        
        if (etc1_decode_image(static_cast<const unsigned char*>(data) + ETC_PKM_HEADER_SIZE, static_cast<etc1_byte*>(_data), _width, _height, bytePerPixel, stride) != 0)
        {
            _dataLen = 0;
            if (_data != nullptr)
            {
                free(_data);
            }
            return false;
        }
        
        return true;
    }
    return false;
}

bool Image::initWithTGAData(tImageTGA* tgaData)
{
    bool ret = false;
    
    do
    {
        CC_BREAK_IF(tgaData == nullptr);
        
        // tgaLoadBuffer only support type 2, 3, 10
        if (2 == tgaData->type || 10 == tgaData->type)
        {
            // true color
            // unsupport RGB555
            if (tgaData->pixelDepth == 16)
            {
                _renderFormat = Texture2D::PixelFormat::RGB5A1;
            }
            else if(tgaData->pixelDepth == 24)
            {
                _renderFormat = Texture2D::PixelFormat::RGB888;
            }
            else if(tgaData->pixelDepth == 32)
            {
                _renderFormat = Texture2D::PixelFormat::RGBA8888;
            }
            else
            {
                CCLOG("Image WARNING: unsupport true color tga data pixel format. FILE: %s", _filePath.c_str());
                break;
            }
        }
        else if(3 == tgaData->type)
        {
            // gray
            if (8 == tgaData->pixelDepth)
            {
                _renderFormat = Texture2D::PixelFormat::I8;
            }
            else
            {
                // actually this won't happen, if it happens, maybe the image file is not a tga
                CCLOG("Image WARNING: unsupport gray tga data pixel format. FILE: %s", _filePath.c_str());
                break;
            }
        }
        
        _width = tgaData->width;
        _height = tgaData->height;
        _data = tgaData->imageData;
        _dataLen = _width * _height * tgaData->pixelDepth / 8;
        _fileType = Format::TGA;
        
        _hasPremultipliedAlpha = false;
        
        ret = true;
        
    }while(false);
    
    if (ret)
    {
        const unsigned char tgaSuffix[] = ".tga";
        for(int i = 0; i < 4; ++i)
        {
            if (tolower(_filePath[_filePath.length() - i - 1]) != tgaSuffix[3 - i])
            {
                CCLOG("Image WARNING: the image file suffix is not tga, but parsed as a tga image file. FILE: %s", _filePath.c_str());
                break;
            };
        }
    }
    else
    {
        if (tgaData && tgaData->imageData != nullptr)
        {
            free(tgaData->imageData);
            _data = nullptr;
        }
    }
    
    return ret;
}

namespace
{
    static const uint32_t makeFourCC(char ch0, char ch1, char ch2, char ch3)
    {
        const uint32_t fourCC = ((uint32_t)(char)(ch0) | ((uint32_t)(char)(ch1) << 8) | ((uint32_t)(char)(ch2) << 16) | ((uint32_t)(char)(ch3) << 24 ));
        return fourCC;
    }
}

bool Image::initWithS3TCData(const unsigned char * data, ssize_t dataLen)
{
    
    const uint32_t FOURCC_DXT1 = makeFourCC('D', 'X', 'T', '1');
    const uint32_t FOURCC_DXT3 = makeFourCC('D', 'X', 'T', '3');
    const uint32_t FOURCC_DXT5 = makeFourCC('D', 'X', 'T', '5');
    
    /* load the .dds file */
    
    S3TCTexHeader *header = (S3TCTexHeader *)data;
    unsigned char *pixelData = static_cast<unsigned char*>(malloc((dataLen - sizeof(S3TCTexHeader)) * sizeof(unsigned char)));
    memcpy((void *)pixelData, data + sizeof(S3TCTexHeader), dataLen - sizeof(S3TCTexHeader));
    
    _width = header->ddsd.width;
    _height = header->ddsd.height;
    _numberOfMipmaps = MAX(1, header->ddsd.DUMMYUNIONNAMEN2.mipMapCount); //if dds header reports 0 mipmaps, set to 1 to force correct software decoding (if needed).
    _dataLen = 0;
    int blockSize = (FOURCC_DXT1 == header->ddsd.DUMMYUNIONNAMEN4.ddpfPixelFormat.fourCC) ? 8 : 16;
    
    /* calculate the dataLen */
    
    int width = _width;
    int height = _height;
    
    if (Configuration::getInstance()->supportsS3TC())  //compressed data length
    {
        _dataLen = dataLen - sizeof(S3TCTexHeader);
        _data = static_cast<unsigned char*>(malloc(_dataLen * sizeof(unsigned char)));
        memcpy((void *)_data,(void *)pixelData , _dataLen);
    }
    else                                               //decompressed data length
    {
        for (int i = 0; i < _numberOfMipmaps && (width || height); ++i)
        {
            if (width == 0) width = 1;
            if (height == 0) height = 1;
            
            _dataLen += (height * width *4);

            width >>= 1;
            height >>= 1;
        }
        _data = static_cast<unsigned char*>(malloc(_dataLen * sizeof(unsigned char)));
    }
    
    /* if hardware supports s3tc, set pixelformat before loading mipmaps, to support non-mipmapped textures  */
    if (Configuration::getInstance()->supportsS3TC())
    {   //decode texture throught hardware
        
        if (FOURCC_DXT1 == header->ddsd.DUMMYUNIONNAMEN4.ddpfPixelFormat.fourCC)
        {
            _renderFormat = Texture2D::PixelFormat::S3TC_DXT1;
        }
        else if (FOURCC_DXT3 == header->ddsd.DUMMYUNIONNAMEN4.ddpfPixelFormat.fourCC)
        {
            _renderFormat = Texture2D::PixelFormat::S3TC_DXT3;
        }
        else if (FOURCC_DXT5 == header->ddsd.DUMMYUNIONNAMEN4.ddpfPixelFormat.fourCC)
        {
            _renderFormat = Texture2D::PixelFormat::S3TC_DXT5;
        }
    } else { //will software decode
        _renderFormat = Texture2D::PixelFormat::RGBA8888;
    }
    
    /* load the mipmaps */
    
    int encodeOffset = 0;
    int decodeOffset = 0;
    width = _width;  height = _height;
    
    for (int i = 0; i < _numberOfMipmaps && (width || height); ++i)  
    {
        if (width == 0) width = 1;
        if (height == 0) height = 1;
        
        int size = ((width+3)/4)*((height+3)/4)*blockSize;
                
        if (Configuration::getInstance()->supportsS3TC())
        {   //decode texture throught hardware
            _mipmaps[i].address = (unsigned char *)_data + encodeOffset;
            _mipmaps[i].len = size;
        }
        else
        {   //if it is not gles or device do not support S3TC, decode texture by software
            
            CCLOG("cocos2d: Hardware S3TC decoder not present. Using software decoder");

            int bytePerPixel = 4;
            unsigned int stride = width * bytePerPixel;

            std::vector<unsigned char> decodeImageData(stride * height);
            if (FOURCC_DXT1 == header->ddsd.DUMMYUNIONNAMEN4.ddpfPixelFormat.fourCC)
            {
                s3tc_decode(pixelData + encodeOffset, &decodeImageData[0], width, height, S3TCDecodeFlag::DXT1);
            }
            else if (FOURCC_DXT3 == header->ddsd.DUMMYUNIONNAMEN4.ddpfPixelFormat.fourCC)
            {
                s3tc_decode(pixelData + encodeOffset, &decodeImageData[0], width, height, S3TCDecodeFlag::DXT3);
            }
            else if (FOURCC_DXT5 == header->ddsd.DUMMYUNIONNAMEN4.ddpfPixelFormat.fourCC)
            {
                s3tc_decode(pixelData + encodeOffset, &decodeImageData[0], width, height, S3TCDecodeFlag::DXT5);
            }
            
            _mipmaps[i].address = (unsigned char *)_data + decodeOffset;
            _mipmaps[i].len = (stride * height);
            memcpy((void *)_mipmaps[i].address, (void *)&decodeImageData[0], _mipmaps[i].len);
            decodeOffset += stride * height;
        }
        
        encodeOffset += size;
        width >>= 1;
        height >>= 1;
    }
    
    /* end load the mipmaps */
    
    if (pixelData != nullptr)
    {
        free(pixelData);
    };
    
    return true;
}


bool Image::initWithATITCData(const unsigned char *data, ssize_t dataLen)
{
    /* load the .ktx file */
    ATITCTexHeader *header = (ATITCTexHeader *)data;
    _width =  header->pixelWidth;
    _height = header->pixelHeight;
    _numberOfMipmaps = header->numberOfMipmapLevels;
    
    int blockSize = 0;
    switch (header->glInternalFormat)
    {
        case CC_GL_ATC_RGB_AMD:
            blockSize = 8;
            break;
        case CC_GL_ATC_RGBA_EXPLICIT_ALPHA_AMD:
            blockSize = 16;
            break;
        case CC_GL_ATC_RGBA_INTERPOLATED_ALPHA_AMD:
            blockSize = 16;
            break;
        default:
            break;
    }
    
    /* pixelData point to the compressed data address */
    unsigned char *pixelData = (unsigned char *)data + sizeof(ATITCTexHeader) + header->bytesOfKeyValueData + 4;
    
    /* caculate the dataLen */
    int width = _width;
    int height = _height;
    
    if (Configuration::getInstance()->supportsATITC())  //compressed data length
    {
        _dataLen = dataLen - sizeof(ATITCTexHeader) - header->bytesOfKeyValueData - 4;
        _data = static_cast<unsigned char*>(malloc(_dataLen * sizeof(unsigned char)));
        memcpy((void *)_data,(void *)pixelData , _dataLen);
    }
    else                                               //decompressed data length
    {
        for (int i = 0; i < _numberOfMipmaps && (width || height); ++i)
        {
            if (width == 0) width = 1;
            if (height == 0) height = 1;
            
            _dataLen += (height * width *4);
            
            width >>= 1;
            height >>= 1;
        }
        _data = static_cast<unsigned char*>(malloc(_dataLen * sizeof(unsigned char)));
    }
    
    /* load the mipmaps */
    int encodeOffset = 0;
    int decodeOffset = 0;
    width = _width;  height = _height;
    
    for (int i = 0; i < _numberOfMipmaps && (width || height); ++i)
    {
        if (width == 0) width = 1;
        if (height == 0) height = 1;
        
        int size = ((width+3)/4)*((height+3)/4)*blockSize;
        
        if (Configuration::getInstance()->supportsATITC())
        {
            /* decode texture throught hardware */
            
            CCLOG("this is atitc H decode");
            
            switch (header->glInternalFormat)
            {
                case CC_GL_ATC_RGB_AMD:
                    _renderFormat = Texture2D::PixelFormat::ATC_RGB;
                    break;
                case CC_GL_ATC_RGBA_EXPLICIT_ALPHA_AMD:
                    _renderFormat = Texture2D::PixelFormat::ATC_EXPLICIT_ALPHA;
                    break;
                case CC_GL_ATC_RGBA_INTERPOLATED_ALPHA_AMD:
                    _renderFormat = Texture2D::PixelFormat::ATC_INTERPOLATED_ALPHA;
                    break;
                default:
                    break;
            }
            
            _mipmaps[i].address = (unsigned char *)_data + encodeOffset;
            _mipmaps[i].len = size;
        }
        else
        {
            /* if it is not gles or device do not support ATITC, decode texture by software */
            
            CCLOG("cocos2d: Hardware ATITC decoder not present. Using software decoder");
            
            int bytePerPixel = 4;
            unsigned int stride = width * bytePerPixel;
            _renderFormat = Texture2D::PixelFormat::RGBA8888;
            
            std::vector<unsigned char> decodeImageData(stride * height);
            switch (header->glInternalFormat)
            {
                case CC_GL_ATC_RGB_AMD:
                    atitc_decode(pixelData + encodeOffset, &decodeImageData[0], width, height, ATITCDecodeFlag::ATC_RGB);
                    break;
                case CC_GL_ATC_RGBA_EXPLICIT_ALPHA_AMD:
                    atitc_decode(pixelData + encodeOffset, &decodeImageData[0], width, height, ATITCDecodeFlag::ATC_EXPLICIT_ALPHA);
                    break;
                case CC_GL_ATC_RGBA_INTERPOLATED_ALPHA_AMD:
                    atitc_decode(pixelData + encodeOffset, &decodeImageData[0], width, height, ATITCDecodeFlag::ATC_INTERPOLATED_ALPHA);
                    break;
                default:
                    break;
            }

            _mipmaps[i].address = (unsigned char *)_data + decodeOffset;
            _mipmaps[i].len = (stride * height);
            memcpy((void *)_mipmaps[i].address, (void *)&decodeImageData[0], _mipmaps[i].len);
            decodeOffset += stride * height;
        }

        encodeOffset += (size + 4);
        width >>= 1;
        height >>= 1;
    }
    /* end load the mipmaps */
    
    return true;
}

bool Image::initWithPVRData(const unsigned char * data, ssize_t dataLen)
{
    return initWithPVRv2Data(data, dataLen) || initWithPVRv3Data(data, dataLen);
}

bool Image::initWithWebpData(const unsigned char * data, ssize_t dataLen)
{
#if CC_USE_WEBP
	bool ret = false;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    FKLOG("WEBP image format not supported on WinRT or WP8");
#else
	do
	{
        WebPDecoderConfig config;
        if (WebPInitDecoderConfig(&config) == 0) break;
        if (WebPGetFeatures(static_cast<const uint8_t*>(data), dataLen, &config.input) != VP8_STATUS_OK) break;
        if (config.input.width == 0 || config.input.height == 0) break;
        
        config.output.colorspace = MODE_RGBA;
        _renderFormat = Texture2D::PixelFormat::RGBA8888;
        _width    = config.input.width;
        _height   = config.input.height;
        
        _dataLen = _width * _height * 4;
        _data = static_cast<unsigned char*>(malloc(_dataLen * sizeof(unsigned char)));
        
        config.output.u.RGBA.rgba = static_cast<uint8_t*>(_data);
        config.output.u.RGBA.stride = _width * 4;
        config.output.u.RGBA.size = _dataLen;
        config.output.is_external_memory = 1;
        
        if (WebPDecode(static_cast<const uint8_t*>(data), dataLen, &config) != VP8_STATUS_OK)
        {
            free(_data);
            _data = nullptr;
            break;
        }
        
        ret = true;
	} while (0);
#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	return ret;
#else 
    FKLOG("webp is not enabled, please enable it in ccConfig.h");
    return false;
#endif // CC_USE_WEBP
}


bool Image::initWithRawData(const unsigned char * data, ssize_t dataLen, int width, int height, int bitsPerComponent, bool preMulti)
{
    bool ret = false;
    do 
    {
        CC_BREAK_IF(0 == width || 0 == height);

        _height   = height;
        _width    = width;
        _hasPremultipliedAlpha = preMulti;
        _renderFormat = Texture2D::PixelFormat::RGBA8888;

        // only RGBA8888 supported
        int bytesPerComponent = 4;
        _dataLen = height * width * bytesPerComponent;
        _data = static_cast<unsigned char*>(malloc(_dataLen * sizeof(unsigned char)));
        CC_BREAK_IF(! _data);
        memcpy(_data, data, _dataLen);

        ret = true;
    } while (0);

    return ret;
}


#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
bool Image::saveToFile(const std::string& filename, bool isToRGB)
{
    //only support for Texture2D::PixelFormat::RGB888 or Texture2D::PixelFormat::RGBA8888 uncompressed data
    if (isCompressed() || (_renderFormat != Texture2D::PixelFormat::RGB888 && _renderFormat != Texture2D::PixelFormat::RGBA8888))
    {
        CCLOG("cocos2d: Image: saveToFile is only support for Texture2D::PixelFormat::RGB888 or Texture2D::PixelFormat::RGBA8888 uncompressed data for now");
        return false;
    }

    bool ret = false;

    do 
    {

        CC_BREAK_IF(filename.size() <= 4);

        std::string strLowerCasePath(filename);
        for (unsigned int i = 0; i < strLowerCasePath.length(); ++i)
        {
            strLowerCasePath[i] = tolower(filename[i]);
        }

        if (std::string::npos != strLowerCasePath.find(".png"))
        {
            CC_BREAK_IF(!saveImageToPNG(filename, isToRGB));
        }
        else if (std::string::npos != strLowerCasePath.find(".jpg"))
        {
            CC_BREAK_IF(!saveImageToJPG(filename));
        }
        else
        {
            break;
        }

        ret = true;
    } while (0);

    return ret;
}
#endif

bool Image::saveImageToPNG(const std::string& filePath, bool isToRGB)
{
    bool ret = false;
    do 
    {
        FILE *fp;
        png_structp png_ptr;
        png_infop info_ptr;
        png_colorp palette;
        png_bytep *row_pointers;

        fp = fopen(filePath.c_str(), "wb");
        CC_BREAK_IF(nullptr == fp);

        png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);

        if (nullptr == png_ptr)
        {
            fclose(fp);
            break;
        }

        info_ptr = png_create_info_struct(png_ptr);
        if (nullptr == info_ptr)
        {
            fclose(fp);
            png_destroy_write_struct(&png_ptr, nullptr);
            break;
        }
#if (CC_TARGET_PLATFORM != CC_PLATFORM_BADA && CC_TARGET_PLATFORM != CC_PLATFORM_NACL)
        if (setjmp(png_jmpbuf(png_ptr)))
        {
            fclose(fp);
            png_destroy_write_struct(&png_ptr, &info_ptr);
            break;
        }
#endif
        png_init_io(png_ptr, fp);

        if (!isToRGB && hasAlpha())
        {
            png_set_IHDR(png_ptr, info_ptr, _width, _height, 8, PNG_COLOR_TYPE_RGB_ALPHA,
                PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
        } 
        else
        {
            png_set_IHDR(png_ptr, info_ptr, _width, _height, 8, PNG_COLOR_TYPE_RGB,
                PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
        }

        palette = (png_colorp)png_malloc(png_ptr, PNG_MAX_PALETTE_LENGTH * sizeof (png_color));
        png_set_PLTE(png_ptr, info_ptr, palette, PNG_MAX_PALETTE_LENGTH);

        png_write_info(png_ptr, info_ptr);

        png_set_packing(png_ptr);

        row_pointers = (png_bytep *)malloc(_height * sizeof(png_bytep));
        if(row_pointers == nullptr)
        {
            fclose(fp);
            png_destroy_write_struct(&png_ptr, &info_ptr);
            break;
        }

        if (!hasAlpha())
        {
            for (int i = 0; i < (int)_height; i++)
            {
                row_pointers[i] = (png_bytep)_data + i * _width * 3;
            }

            png_write_image(png_ptr, row_pointers);

            free(row_pointers);
            row_pointers = nullptr;
        }
        else
        {
            if (isToRGB)
            {
                unsigned char *tempData = static_cast<unsigned char*>(malloc(_width * _height * 3 * sizeof(unsigned char)));
                if (nullptr == tempData)
                {
                    fclose(fp);
                    png_destroy_write_struct(&png_ptr, &info_ptr);
                    
                    free(row_pointers);
                    row_pointers = nullptr;
                    break;
                }

                for (int i = 0; i < _height; ++i)
                {
                    for (int j = 0; j < _width; ++j)
                    {
                        tempData[(i * _width + j) * 3] = _data[(i * _width + j) * 4];
                        tempData[(i * _width + j) * 3 + 1] = _data[(i * _width + j) * 4 + 1];
                        tempData[(i * _width + j) * 3 + 2] = _data[(i * _width + j) * 4 + 2];
                    }
                }

                for (int i = 0; i < (int)_height; i++)
                {
                    row_pointers[i] = (png_bytep)tempData + i * _width * 3;
                }

                png_write_image(png_ptr, row_pointers);

                free(row_pointers);
                row_pointers = nullptr;

                if (tempData != nullptr)
                {
                    free(tempData);
                }
            } 
            else
            {
                for (int i = 0; i < (int)_height; i++)
                {
                    row_pointers[i] = (png_bytep)_data + i * _width * 4;
                }

                png_write_image(png_ptr, row_pointers);

                free(row_pointers);
                row_pointers = nullptr;
            }
        }

        png_write_end(png_ptr, info_ptr);

        png_free(png_ptr, palette);
        palette = nullptr;

        png_destroy_write_struct(&png_ptr, &info_ptr);

        fclose(fp);

        ret = true;
    } while (0);
    return ret;
}
bool Image::saveImageToJPG(const std::string& filePath)
{
#if CC_USE_JPEG
    bool ret = false;
    do 
    {
        struct jpeg_compress_struct cinfo;
        struct jpeg_error_mgr jerr;
        FILE * outfile;                 /* target file */
        JSAMPROW row_pointer[1];        /* pointer to JSAMPLE row[s] */
        int     row_stride;          /* physical row width in image buffer */

        cinfo.err = jpeg_std_error(&jerr);
        /* Now we can initialize the JPEG compression object. */
        jpeg_create_compress(&cinfo);

        CC_BREAK_IF((outfile = fopen(filePath.c_str(), "wb")) == nullptr);
        
        jpeg_stdio_dest(&cinfo, outfile);

        cinfo.image_width = _width;    /* image width and height, in pixels */
        cinfo.image_height = _height;
        cinfo.input_components = 3;       /* # of color components per pixel */
        cinfo.in_color_space = JCS_RGB;       /* colorspace of input image */

        jpeg_set_defaults(&cinfo);
        jpeg_set_quality(&cinfo, 90, TRUE);
        
        jpeg_start_compress(&cinfo, TRUE);

        row_stride = _width * 3; /* JSAMPLEs per row in image_buffer */

        if (hasAlpha())
        {
            unsigned char *tempData = static_cast<unsigned char*>(malloc(_width * _height * 3 * sizeof(unsigned char)));
            if (nullptr == tempData)
            {
                jpeg_finish_compress(&cinfo);
                jpeg_destroy_compress(&cinfo);
                fclose(outfile);
                break;
            }

            for (int i = 0; i < _height; ++i)
            {
                for (int j = 0; j < _width; ++j)

                {
                    tempData[(i * _width + j) * 3] = _data[(i * _width + j) * 4];
                    tempData[(i * _width + j) * 3 + 1] = _data[(i * _width + j) * 4 + 1];
                    tempData[(i * _width + j) * 3 + 2] = _data[(i * _width + j) * 4 + 2];
                }
            }

            while (cinfo.next_scanline < cinfo.image_height)
            {
                row_pointer[0] = & tempData[cinfo.next_scanline * row_stride];
                (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
            }

            if (tempData != nullptr)
            {
                free(tempData);
            }
        } 
        else
        {
            while (cinfo.next_scanline < cinfo.image_height) {
                row_pointer[0] = & _data[cinfo.next_scanline * row_stride];
                (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
            }
        }

        jpeg_finish_compress(&cinfo);
        fclose(outfile);
        jpeg_destroy_compress(&cinfo);
        
        ret = true;
    } while (0);
    return ret;
#else
    CCLOG("jpeg is not enabled, please enable it in ccConfig.h");
    return false;
#endif // CC_USE_JPEG
}

void Image::premultipliedAlpha()
{
    CCASSERT(_renderFormat == Texture2D::PixelFormat::RGBA8888, "The pixel format should be RGBA8888!");
    
    unsigned int* fourBytes = (unsigned int*)_data;
    for(int i = 0; i < _width * _height; i++)
    {
        unsigned char* p = _data + i * 4;
        fourBytes[i] = CC_RGB_PREMULTIPLY_ALPHA(p[0], p[1], p[2], p[3]);
    }
    
    _hasPremultipliedAlpha = true;
}


void Image::setPVRImagesHavePremultipliedAlpha(bool haveAlphaPremultiplied)
{
    _PVRHaveAlphaPremultiplied = haveAlphaPremultiplied;
}



bool Banner::loadImageJPEG(FILE *fd)
{
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    JSAMPROW row_pointer[1];
    unsigned long location = 0;

    cinfo.err = jpeg_std_error(&jerr);

    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, fd);
    jpeg_read_header(&cinfo, TRUE);
    //cinfo.scale_num = 1;
    //cinfo.scale_denom = SCALE;
    jpeg_start_decompress(&cinfo);
    int width = cinfo.output_width;
    int height = cinfo.output_height;
    int depth = cinfo.num_components; //should always be 3

	LOGI("width:%d;height:%d;depth:%d",width,height,depth);
    image = (unsigned char *) malloc(width * height * depth);
    row_pointer[0] = (unsigned char *) malloc(width * depth);
    /* read one scan line at a time */
    int i;
    while( cinfo.output_scanline < cinfo.output_height )
    {
        jpeg_read_scanlines( &cinfo, row_pointer, 1 );
        for( i=0; i< (width * depth); i++)
            image[location++] = row_pointer[0][i];
    }
    fclose(fd);
    free(row_pointer[0]);

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    //gen vbo
    glGenBuffers(1, &mVBO);
	checkGlError("glGenBuffers");
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	checkGlError("glBindBuffer");
    glBufferData(GL_ARRAY_BUFFER, sizeof(struct BANNER_VERTEX)*4, data, GL_STATIC_DRAW);
	checkGlError("glBufferData");

	LOGE("mVBO:%d",mVBO);
    glActiveTexture(GL_TEXTURE0);
    checkGlError("glActiveTexture");
    glGenTextures(1, &mTextureID);
    checkGlError("glGenTextures");
    glBindTexture(GL_TEXTURE_2D, mTextureID);
    checkGlError("glBindTexture");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)image);
    checkGlError("glTexImage2D");
	LOGE("Textureid:%d",mTextureID);
    LOGE("LOADING DONE!!!!");

	return true;
}


bool Banner::loadImagePNG(FILE *fp)
{
     png_structp png_ptr;
     png_infop info_ptr;
     unsigned int sig_read = 0;
     int color_type, interlace_type;

     /* Create and initialize the png_struct
     * with the desired error handler
     * functions.  If you want to use the
     * default stderr and longjump method,
     * you can supply NULL for the last
     * three parameters.  We also supply the
     * the compiler header file version, so
     * that we know if the application
     * was compiled with a compatible version
     * of the library.  REQUIRED
     */
     png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                 NULL, NULL, NULL);

     if (png_ptr == NULL) {
             fclose(fp);
             return false;
     }

     /* Allocate/initialize the memory
     * for image information.  REQUIRED. */
     info_ptr = png_create_info_struct(png_ptr);
     if (info_ptr == NULL) {
             fclose(fp);
             png_destroy_read_struct(&png_ptr, NULL, NULL);
             return false;
     }

     /* Set error handling if you are
      * using the setjmp/longjmp method
      * (this is the normal method of
      * doing things with libpng).
      * REQUIRED unless you  set up
      * your own error handlers in
      * the png_create_read_struct()
      * earlier.
     */
     if (setjmp(png_jmpbuf(png_ptr))) {
          /* Free all of the memory associated
           * with the png_ptr and info_ptr */
             png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
             fclose(fp);
             /* If we get here, we had a
              * problem reading the file */
             return false;
     }

     /* Set up the output control if
      * you are using standard C streams */
      png_init_io(png_ptr, fp);

     /* If we have already
      * read some of the signature */
      png_set_sig_bytes(png_ptr, sig_read);

     /*
      * If you have enough memory to read
      * in the entire image at once, and
      * you need to specify only
      * transforms that can be controlled
      * with one of the PNG_TRANSFORM_*
      * bits (this presently excludes
      * dithering, filling, setting
      * background, and doing gamma
      * adjustment), then you can read the
      * entire image (including pixels)
      * into the info structure with this
      * call
      *
      * PNG_TRANSFORM_STRIP_16 |
      * PNG_TRANSFORM_PACKING  forces 8 bit
      * PNG_TRANSFORM_EXPAND forces to
      *  expand a palette into RGB
      */
      png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);

      int outWidth = png_get_image_width(png_ptr,info_ptr);
      int outHeight = png_get_image_height(png_ptr,info_ptr);
      /*bool outHasAlpha = false;
      switch (info_ptr->color_type) {
         case PNG_COLOR_TYPE_RGBA:
             outHasAlpha = true;
             break;
         case PNG_COLOR_TYPE_RGB:
              outHasAlpha = false;
              break;
         default:
              LOGI("Color type %d not supported!\n",info_ptr->color_type);
              png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
              fclose(fp);
              return false;
       }
       */
       unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
       image = (unsigned char*) malloc(row_bytes * outHeight);

       png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);

       for (int i = 0; i < outHeight; i++){
             // note that png is ordered top to
             // bottom, but OpenGL expect it bottom to top
             // so the order or swapped
             memcpy(image+(row_bytes * (outHeight-1-i)), row_pointers[i], row_bytes);
       }

       /* Clean up after the read,
        * and free any memory allocated */
       png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

       /* Close the file */
       fclose(fp);

       //gen vbo
       glGenBuffers(1, &mVBO);
       glBindBuffer(GL_ARRAY_BUFFER, mVBO);
       GLfloat vv[]={0.0000f,
    		   0.0000f,
    		   0.0000f,
    		   0.0000f,
    		   0.0000,
    		   0.0000,
    		   256.0000,
    		   0.0000f,
    		   0.0000f,
    		   1.0000f,
    		   256.0000f,
    		   0.0000f,
    		   0.0000f,
    		   1.0000f,
    		   0.0000f,
    		   256.0000f,
    		   256.0000f,
    		   0.0000f,
    		   1.0000f,
    		   1.0000f,
       };
       glBufferData(GL_ARRAY_BUFFER, sizeof(struct BANNER_VERTEX)*4, vv, GL_STATIC_DRAW);

       glActiveTexture(GL_TEXTURE0);
       glGenTextures(1, &mTextureID);
       glBindTexture(GL_TEXTURE_2D, mTextureID);

       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
       glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, outWidth, outHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)image);
       /* That's it */
       return true;
}

FLAKOR_NS_END

