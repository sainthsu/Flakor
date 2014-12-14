//////////////////////////////////////////////////////////////////////////
//struct and data for pvr structure

namespace
{
    static const int PVR_TEXTURE_FLAG_TYPE_MASK = 0xff;
    
    static bool _PVRHaveAlphaPremultiplied = false;
    
    // Values taken from PVRTexture.h from http://www.imgtec.com
    enum class PVR2TextureFlag
    {
        Mipmap         = (1<<8),        // has mip map levels
        Twiddle        = (1<<9),        // is twiddled
        Bumpmap        = (1<<10),       // has normals encoded for a bump map
        Tiling         = (1<<11),       // is bordered for tiled pvr
        Cubemap        = (1<<12),       // is a cubemap/skybox
        FalseMipCol    = (1<<13),       // are there false colored MIP levels
        Volume         = (1<<14),       // is this a volume texture
        Alpha          = (1<<15),       // v2.1 is there transparency info in the texture
        VerticalFlip   = (1<<16),       // v2.1 is the texture vertically flipped
    };
    
    enum class PVR3TextureFlag
    {
        PremultipliedAlpha	= (1<<1)	// has premultiplied alpha
    };
    
    static const char gPVRTexIdentifier[5] = "PVR!";
    
    // v2
    enum class PVR2TexturePixelFormat : unsigned char
    {
        RGBA4444 = 0x10,
        RGBA5551,
        RGBA8888,
        RGB565,
        RGB555,          // unsupported
        RGB888,
        I8,
        AI88,
        PVRTC2BPP_RGBA,
        PVRTC4BPP_RGBA,
        BGRA8888,
        A8,
    };
        
    // v3
    enum class PVR3TexturePixelFormat : uint64_t
    {
        PVRTC2BPP_RGB  = 0ULL,
        PVRTC2BPP_RGBA = 1ULL,
        PVRTC4BPP_RGB  = 2ULL,
        PVRTC4BPP_RGBA = 3ULL,
        PVRTC2_2BPP_RGBA = 4ULL,
        PVRTC2_4BPP_RGBA  = 5ULL,
        ETC1 = 6ULL,
        DXT1 = 7ULL,
        DXT2 = 8ULL,
        DXT3 = 9ULL,
        DXT4 = 10ULL,
        DXT5 = 11ULL,
        BC1 = 7ULL,
        BC2 = 9ULL,
        BC3 = 11ULL,
        BC4 = 12ULL,
        BC5 = 13ULL,
        BC6 = 14ULL,
        BC7 = 15ULL,
        UYVY = 16ULL,
        YUY2 = 17ULL,
        BW1bpp = 18ULL,
        R9G9B9E5 = 19ULL,
        RGBG8888 = 20ULL,
        GRGB8888 = 21ULL,
        ETC2_RGB = 22ULL,
        ETC2_RGBA = 23ULL,
        ETC2_RGBA1 = 24ULL,
        EAC_R11_Unsigned = 25ULL,
        EAC_R11_Signed = 26ULL,
        EAC_RG11_Unsigned = 27ULL,
        EAC_RG11_Signed = 28ULL,
            
        BGRA8888       = 0x0808080861726762ULL,
        RGBA8888       = 0x0808080861626772ULL,
        RGBA4444       = 0x0404040461626772ULL,
        RGBA5551       = 0x0105050561626772ULL,
        RGB565         = 0x0005060500626772ULL,
        RGB888         = 0x0008080800626772ULL,
        A8             = 0x0000000800000061ULL,
        L8             = 0x000000080000006cULL,
        LA88           = 0x000008080000616cULL,
    };
        
        
    // v2
    typedef const std::map<PVR2TexturePixelFormat, Texture2D::PixelFormat> _pixel2_formathash;
    
    static const _pixel2_formathash::value_type v2_pixel_formathash_value[] =
    {
        _pixel2_formathash::value_type(PVR2TexturePixelFormat::BGRA8888,	    Texture2D::PixelFormat::BGRA8888),
        _pixel2_formathash::value_type(PVR2TexturePixelFormat::RGBA8888,	    Texture2D::PixelFormat::RGBA8888),
        _pixel2_formathash::value_type(PVR2TexturePixelFormat::RGBA4444,	    Texture2D::PixelFormat::RGBA4444),
        _pixel2_formathash::value_type(PVR2TexturePixelFormat::RGBA5551,	    Texture2D::PixelFormat::RGB5A1),
        _pixel2_formathash::value_type(PVR2TexturePixelFormat::RGB565,	    Texture2D::PixelFormat::RGB565),
        _pixel2_formathash::value_type(PVR2TexturePixelFormat::RGB888,	    Texture2D::PixelFormat::RGB888),
        _pixel2_formathash::value_type(PVR2TexturePixelFormat::A8,	        Texture2D::PixelFormat::A8),
        _pixel2_formathash::value_type(PVR2TexturePixelFormat::I8,	        Texture2D::PixelFormat::I8),
        _pixel2_formathash::value_type(PVR2TexturePixelFormat::AI88,	        Texture2D::PixelFormat::AI88),
            
        _pixel2_formathash::value_type(PVR2TexturePixelFormat::PVRTC2BPP_RGBA,	    Texture2D::PixelFormat::PVRTC2A),
        _pixel2_formathash::value_type(PVR2TexturePixelFormat::PVRTC4BPP_RGBA,	    Texture2D::PixelFormat::PVRTC4A),
    };
        
    static const int PVR2_MAX_TABLE_ELEMENTS = sizeof(v2_pixel_formathash_value) / sizeof(v2_pixel_formathash_value[0]);
    static const _pixel2_formathash v2_pixel_formathash(v2_pixel_formathash_value, v2_pixel_formathash_value + PVR2_MAX_TABLE_ELEMENTS);
        
    // v3
    typedef const std::map<PVR3TexturePixelFormat, Texture2D::PixelFormat> _pixel3_formathash;
    static _pixel3_formathash::value_type v3_pixel_formathash_value[] =
    {
        _pixel3_formathash::value_type(PVR3TexturePixelFormat::BGRA8888,	Texture2D::PixelFormat::BGRA8888),
        _pixel3_formathash::value_type(PVR3TexturePixelFormat::RGBA8888,	Texture2D::PixelFormat::RGBA8888),
        _pixel3_formathash::value_type(PVR3TexturePixelFormat::RGBA4444,	Texture2D::PixelFormat::RGBA4444),
        _pixel3_formathash::value_type(PVR3TexturePixelFormat::RGBA5551,	Texture2D::PixelFormat::RGB5A1),
        _pixel3_formathash::value_type(PVR3TexturePixelFormat::RGB565,	    Texture2D::PixelFormat::RGB565),
        _pixel3_formathash::value_type(PVR3TexturePixelFormat::RGB888,	    Texture2D::PixelFormat::RGB888),
        _pixel3_formathash::value_type(PVR3TexturePixelFormat::A8,	        Texture2D::PixelFormat::A8),
        _pixel3_formathash::value_type(PVR3TexturePixelFormat::L8,	        Texture2D::PixelFormat::I8),
        _pixel3_formathash::value_type(PVR3TexturePixelFormat::LA88,	    Texture2D::PixelFormat::AI88),
            
        _pixel3_formathash::value_type(PVR3TexturePixelFormat::PVRTC2BPP_RGB,	    Texture2D::PixelFormat::PVRTC2),
        _pixel3_formathash::value_type(PVR3TexturePixelFormat::PVRTC2BPP_RGBA,	    Texture2D::PixelFormat::PVRTC2A),
        _pixel3_formathash::value_type(PVR3TexturePixelFormat::PVRTC4BPP_RGB,	    Texture2D::PixelFormat::PVRTC4),
        _pixel3_formathash::value_type(PVR3TexturePixelFormat::PVRTC4BPP_RGBA,	    Texture2D::PixelFormat::PVRTC4A),

        _pixel3_formathash::value_type(PVR3TexturePixelFormat::ETC1,	    Texture2D::PixelFormat::ETC),
    };
        
    static const int PVR3_MAX_TABLE_ELEMENTS = sizeof(v3_pixel_formathash_value) / sizeof(v3_pixel_formathash_value[0]);
        
    static const _pixel3_formathash v3_pixel_formathash(v3_pixel_formathash_value, v3_pixel_formathash_value + PVR3_MAX_TABLE_ELEMENTS);
        
    typedef struct _PVRTexHeader
    {
        unsigned int headerLength;
        unsigned int height;
        unsigned int width;
        unsigned int numMipmaps;
        unsigned int flags;
        unsigned int dataLength;
        unsigned int bpp;
        unsigned int bitmaskRed;
        unsigned int bitmaskGreen;
        unsigned int bitmaskBlue;
        unsigned int bitmaskAlpha;
        unsigned int pvrTag;
        unsigned int numSurfs;
    } PVRv2TexHeader;
        
#ifdef _MSC_VER
#pragma pack(push,1)
#endif
    typedef struct
    {
        uint32_t version;
        uint32_t flags;
        uint64_t pixelFormat;
        uint32_t colorSpace;
        uint32_t channelType;
        uint32_t height;
        uint32_t width;
        uint32_t depth;
        uint32_t numberOfSurfaces;
        uint32_t numberOfFaces;
        uint32_t numberOfMipmaps;
        uint32_t metadataLength;
#ifdef _MSC_VER
    } PVRv3TexHeader;
#pragma pack(pop)
#else
    } __attribute__((packed)) PVRv3TexHeader;
#endif
}
//pvr structure end

//////////////////////////////////////////////////////////////////////////

//struct and data for s3tc(dds) struct
namespace
{
    struct DDColorKey
    {
        uint32_t colorSpaceLowValue;
        uint32_t colorSpaceHighValue;
    };
    
    struct DDSCaps
    {
        uint32_t caps;
        uint32_t caps2;
        uint32_t caps3;
        uint32_t caps4;
    };
    
    struct DDPixelFormat
    {
        uint32_t size;
        uint32_t flags;
        uint32_t fourCC;
        uint32_t RGBBitCount;
        uint32_t RBitMask;
        uint32_t GBitMask;
        uint32_t BBitMask;
        uint32_t ABitMask;
    };
    
    
    struct DDSURFACEDESC2
    {
        uint32_t size;
        uint32_t flags;
        uint32_t height;
        uint32_t width;
        
        union
        {
            uint32_t pitch;
            uint32_t linearSize;
        } DUMMYUNIONNAMEN1;
        
        union
        {
            uint32_t backBufferCount;
            uint32_t depth;
        } DUMMYUNIONNAMEN5;
        
        union
        {
            uint32_t mipMapCount;
            uint32_t refreshRate;
            uint32_t srcVBHandle;
        } DUMMYUNIONNAMEN2;
        
        uint32_t alphaBitDepth;
        uint32_t reserved;
        uint32_t surface;
        
        union
        {
            DDColorKey ddckCKDestOverlay;
            uint32_t emptyFaceColor;
        } DUMMYUNIONNAMEN3;
        
        DDColorKey ddckCKDestBlt;
        DDColorKey ddckCKSrcOverlay;
        DDColorKey ddckCKSrcBlt;
        
        union
        {
            DDPixelFormat ddpfPixelFormat;
            uint32_t FVF;
        } DUMMYUNIONNAMEN4;
        
        DDSCaps ddsCaps;
        uint32_t textureStage;
    } ;
    
#pragma pack(push,1)
    
    struct S3TCTexHeader
    {
        char fileCode[4];
        DDSURFACEDESC2 ddsd;
    };
    
#pragma pack(pop)

}
//s3tc struct end

//////////////////////////////////////////////////////////////////////////

//struct and data for atitc(ktx) struct
namespace
{
    struct ATITCTexHeader
    {
        //HEADER
        char identifier[12];
        uint32_t endianness;
        uint32_t glType;
        uint32_t glTypeSize;
        uint32_t glFormat;
        uint32_t glInternalFormat;
        uint32_t glBaseInternalFormat;
        uint32_t pixelWidth;
        uint32_t pixelHeight;
        uint32_t pixelDepth;
        uint32_t numberOfArrayElements;
        uint32_t numberOfFaces;
        uint32_t numberOfMipmapLevels;
        uint32_t bytesOfKeyValueData;
    };
}
//atittc struct end

