
#include "Texture2D.h"

FLAKOR_NS_BEGIN

Textrue2D::Texture2D()
{
}

Textrue2D::~Texture2D();
bool Textrue2D::initWithData(const void *data,ssize_t dataLen, PixelFormat pixelFormat,int width,int height,Size size);

bool Textrue2D::initWithFile(const char *fileName)
{}

/** Gets the pixel format of the texture */
PixelFormat Textrue2D::getPixelFormat() const;

void Textrue2D::setTexParams(const TexParams& texParams);

void Textrue2D::setContentSize(const Size *size);

Size* Textrue2D::getContentSize();
	
/** Gets the width of the texture in pixels */
int Textrue2D::getPixelsWidth() const;
    
/** Gets the height of the texture in pixels */
int Textrue2D::getPixelsHeight() const;
    
/** Gets the texture id*/
GLuint Textrue2D::getTextureID() const;
    
/** Gets max S */
GLfloat Textrue2D::getMaxS() const;
/** Sets max S */
void Textrue2D::setMaxS(GLfloat maxS);
    
/** Gets max T */
GLfloat Textrue2D::getMaxT() const;
/** Sets max T */
void Textrue2D::setMaxT(GLfloat maxT);

void Textrue2D::load();
void Textrue2D::unload();


FLAKOR_NS_END

#endif
