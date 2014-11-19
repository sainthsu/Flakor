#ifndef _FK_TEXTURE2D_H_
#define _FK_TEXTURE2D_H_

#include <map>
#include "base/lang/Object.h"

FLAKOR_NS_BEGIN

class Size;
class GLProgram;
class Image;


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
} PixelFormat;

typedef struct _TexParams {
	GLuint    minFilter;
	GLuint    magFilter;
	GLuint    wrapS;
	GLuint    wrapT;
} TexParams;

struct PixelFormatInfo {

        PixelFormatInfo(GLenum anInternalFormat, GLenum aFormat, GLenum aType, int aBpp, bool aCompressed, bool anAlpha)
            : internalFormat(anInternalFormat)
            , format(aFormat)
            , type(aType)
            , bpp(aBpp)
            , compressed(aCompressed)
            , alpha(anAlpha)
        {}

        GLenum internalFormat;
        GLenum format;
        GLenum type;
        int bpp;
        bool compressed;
        bool alpha;
};

typedef std::map<PixelFormat, const PixelFormatInfo> PixelFormatInfoMap;

class Texture2D : public Object
{
	protected:
		const char* _filename;		
	
		PixelFormat _pixelFormat;
		/** width in pixels */
		int _pixelsWidth;

		/** height in pixels */
		int _pixelsHeight;

		/** texture id in gpu */
        GLuint _textureID;

		/** texture max S */
		GLfloat _maxS;

		/** texture max T */
		GLfloat _maxT;

		/** content size */
		Size* _contentSize;

		/** whether or not the texture has their Alpha premultiplied */
		bool _hasPremultipliedAlpha;

		bool _hasMipmaps;

		/** shader program used by drawAtPoint and drawInRect */
		GLProgram* _shaderProgram;

		static const PixelFormatInfoMap _pixelFormatInfoTables;		

		bool	_antialiasEnabled;

		//TODO need these attributes later
		float _scale;
		bool rotated;
		bool flipY;

	public:
		Texture2D();
		virtual ~Texture2D();
		
		bool initWithData(const void *data,ssize_t dataLen,PixelFormat pixelFormat,int width,int height,Size size);

		bool initWithAsset(const char *fileName);

		bool initWithFile(const char *fileName);

		/** Gets the pixel format of the texture */
	    PixelFormat getPixelFormat() const;

		void setTexParams(const TexParams& texParams);

		void setContentSize(Size *size);

		Size* getContentSize();
	
		/** Gets the width of the texture in pixels */
    	int getPixelsWidth() const;
    
    	/** Gets the height of the texture in pixels */
    	int getPixelsHeight() const;
    
    	/** Gets the texture id*/
    	GLuint getTextureID() const;
    
    	/** Gets max S */
    	GLfloat getMaxS() const;
    	/** Sets max S */
    	void setMaxS(GLfloat maxS);
    
    	/** Gets max T */
    	GLfloat getMaxT() const;
    	/** Sets max T */
    	void setMaxT(GLfloat maxT);

		void load();
		Image* loadData();
		void unload();
	public:
		static const PixelFormatInfoMap& getPixelFormatInfoMap();

};

FLAKOR_NS_END

#endif
