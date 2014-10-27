
#ifndef _FK_TEXTURE2D_H_
#define _FK_TEXTURE2D_H_

FLAKOR_NS_START

/** @typedef Texture2DPixelFormat
  Possible texture pixel formats
 */
typedef enum {
	//! auto detect the type
	AUTO,
	//! 32-bit texture: BGRA8888
	BGRA8888,
	//! 32-bit texture: RGBA8888
	RGBA8888,
	//! 24-bit texture: RGBA888
	RGB888,
	//! 16-bit texture without Alpha channel
	RGB565,
	//! 8-bit textures used as masks
	A8,
	//! 8-bit intensity texture
	I8,
	//! 16-bit textures used as masks
	AI88,
	//! 16-bit textures: RGBA4444
	RGBA4444,
	//! 16-bit textures: RGB5A1
	RGB5A1,
	//! 4-bit PVRTC-compressed texture: PVRTC4
	PVRTC4,
	//! 4-bit PVRTC-compressed texture: PVRTC4 (has alpha channel)
	PVRTC4A,
	//! 2-bit PVRTC-compressed texture: PVRTC2
	PVRTC2,
	//! 2-bit PVRTC-compressed texture: PVRTC2 (has alpha channel)
	PVRTC2A,
	//! ETC-compressed texture: ETC
	ETC,
	//! S3TC-compressed texture: S3TC_Dxt1
	S3TC_DXT1,
	//! S3TC-compressed texture: S3TC_Dxt3
	S3TC_DXT3,
	//! S3TC-compressed texture: S3TC_Dxt5
	S3TC_DXT5,
	//! ATITC-compressed texture: ATC_RGB
	ATC_RGB,
	//! ATITC-compressed texture: ATC_EXPLICIT_ALPHA
	ATC_EXPLICIT_ALPHA,
	//! ATITC-compresed texture: ATC_INTERPOLATED_ALPHA
	ATC_INTERPOLATED_ALPHA,
	//! Default texture format: AUTO
	DEFAULT = AUTO,

	NONE = -1
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
		bool initWithData(const void *data,int width,int height,Size size);
		bool initWithFile(const char *fileName);
		void setContentSize(const Size *size);
		Size* getContentSize();
		void load();
		void unload();

protexted:

		Texture2DPixelFormat pixelFormat;
		/** width in pixels */
		int _pixelsWidth;

		/** height in pixels */
		int _pixelsHeight;

		/** texture id in gpu */
		GLuint _textureID

		/** texture max S */
		GLfloat _maxS;

		/** texture max T */
		GLfloat _maxT;

		/** content size */
		Size _contentSize;

		/** whether or not the texture has their Alpha premultiplied */
		bool _hasPremultipliedAlpha;

		bool _hasMipmaps;

		/** shader program used by drawAtPoint and drawInRect */
		GLProgram* _shaderProgram;
}

FLAKOR_NS_END

#endif
