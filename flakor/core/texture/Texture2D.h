
#ifndef _FK_TEXTURE2D_H_
#define _FK_TEXTURE2D_H_

FLAKOR_NS_START

/** @typedef CCTexture2DPixelFormat
Possible texture pixel formats
*/
typedef enum {

    //! 32-bit texture: RGBA8888
    Texture2DPixelFormat_RGBA8888,
    //! 24-bit texture: RGBA888
    Texture2DPixelFormat_RGB888,
    //! 16-bit texture without Alpha channel
    Texture2DPixelFormat_RGB565,
    //! 8-bit textures used as masks
    Texture2DPixelFormat_A8,
    //! 8-bit intensity texture
    Texture2DPixelFormat_I8,
    //! 16-bit textures used as masks
    Texture2DPixelFormat_AI88,
    //! 16-bit textures: RGBA4444
    Texture2DPixelFormat_RGBA4444,
    //! 16-bit textures: RGB5A1
    Texture2DPixelFormat_RGB5A1,    
    //! 4-bit PVRTC-compressed texture: PVRTC4
    Texture2DPixelFormat_PVRTC4,
    //! 2-bit PVRTC-compressed texture: PVRTC2
    Texture2DPixelFormat_PVRTC2,


    //! Default texture format: RGBA8888
    Texture2DPixelFormat_Default = Texture2DPixelFormat_RGBA8888,

} Texture2DPixelFormat;

typedef struct _TexParams {
		    GLuint    minFilter;
		    GLuint    magFilter;
		    GLuint    wrapS;
			GLuint    wrapT;
} TexParams;

class Texture2D : public Object,ITexture2D
{
    public:
		Texture2D();
		virtual ~Texture2D();
	
	protexted:
		unsigned int textureID;
		Texture2DPixelFormat pixelFormat;
}

FLAKOR_NS_END

#endif
