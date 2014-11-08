
#include "targetMacros.h"
#include "base/element/Color.h"

FLAKOR_NS_BEGIN

Color::Color(void)
:red(1.),green(1.),blue(1.0),alpha(1.0)
{
}

Color::Color(GLubyte red,GLubyte green,GLubyte blue)
:red(red) , green(green) ,blue(blue)
{
}

Color::Color(GLubyte red,GLubyte green,GLubyte blue,GLubyte alpha)
:red(red/255.f) , green(green/255.f) ,blue(blue/255.f),alpha(alpha/255.f)
{
}

Color::Color(GLfloat red,GLfloat green,GLfloat blue)
:red(red) , green(green) ,blue(blue)
{
}

Color::Color(GLfloat red,GLfloat green,GLfloat blue,GLfloat alpha)
:red(red) , green(green) ,blue(blue) ,alpha(alpha)
{
}

Color::Color(const Color& other)
:red(other.red) , green(other.green) ,blue(other.blue) ,alpha(other.alpha)
{
}

void Color::setColor(GLubyte red,GLubyte green,GLubyte blue)
{
	this->red = red/255.f;
	this->green = green/255.f;
	this->blue = blue/255.f;
}

void Color::setColor(GLubyte red,GLubyte green,GLubyte blue,GLubyte alpha)
{
	this->red = red/255.f;
	this->green = green/255.f;
	this->blue = blue/255.f;
	this->alpha = alpha/255.f;
}

void Color::setColor(GLfloat red,GLfloat green,GLfloat blue)
{
	this->red = red;
	this->green = green;
	this->blue = blue;
}

void Color::setColor(GLfloat red,GLfloat green,GLfloat blue,GLfloat alpha)
{
	this->red = red;
	this->green = green;
	this->blue = blue;
	this->alpha = alpha;
}

void Color::mix(const Color& ColorA, float percentageA, const Color& ColorB, float percentageB)
{
	GLfloat red = (ColorA.red * percentageA) + (ColorB.red * percentageB);
	GLfloat green = (ColorA.green * percentageA) + (ColorB.green * percentageB);
	GLfloat blue = (ColorA.blue * percentageA) + (ColorB.blue * percentageB);
	GLfloat alpha = (ColorA.alpha * percentageA) + (ColorB.alpha * percentageB);

	this->setColor(red, green, blue, alpha);
}

const float Color::COLOR_FACTOR_INT_TO_FLOAT = 255.0f;
const int32_t Color::ABGR_PACKED_RED_SHIFT = 0;
const int32_t Color::ABGR_PACKED_GREEN_SHIFT = 8;
const int32_t Color::ABGR_PACKED_BLUE_SHIFT = 16;
const int32_t Color::ABGR_PACKED_ALPHA_SHIFT = 24;

const int32_t Color::ABGR_PACKED_RED_CLEAR = 0XFFFFFF00;
const int32_t Color::ABGR_PACKED_GREEN_CLEAR = 0XFFFF00FF;
const int32_t Color::ABGR_PACKED_BLUE_CLEAR = 0XFF00FFFF;
const int32_t Color::ABGR_PACKED_ALPHA_CLEAR = 0X00FFFFFF;
	
const int32_t Color::RGBA_PACKED_ALPHA_SHIFT = 0;
const int32_t Color::RGBA_PACKED_BLUE_SHIFT = 8;
const int32_t Color::RGBA_PACKED_GREEN_SHIFT = 16;
const int32_t Color::RGBA_PACKED_RED_SHIFT = 24;
	
const int32_t Color::RGBA_PACKED_ALPHA_CLEAR = 0XFFFFFF00;
const int32_t Color::RGBA_PACKED_BLUE_CLEAR = 0XFFFF00FF;
const int32_t Color::RGBA_PACKED_GREEN_CLEAR = 0XFF00FFFF;
const int32_t Color::RGBA_PACKED_RED_CLEAR = 0X00FFFFFF;
	
const int32_t Color::ARGB_PACKED_BLUE_SHIFT = 0;
const int32_t Color::ARGB_PACKED_GREEN_SHIFT = 8;
const int32_t Color::ARGB_PACKED_RED_SHIFT = 16;
const int32_t Color::ARGB_PACKED_ALPHA_SHIFT = 24;

const int32_t Color::ARGB_PACKED_BLUE_CLEAR = 0XFFFFFF00;
const int32_t Color::ARGB_PACKED_GREEN_CLEAR = 0XFFFF00FF;
const int32_t Color::ARGB_PACKED_RED_CLEAR = 0XFF00FFFF;
const int32_t Color::ARGB_PACKED_ALPHA_CLEAR = 0X00FFFFFF;
	
const Color Color::WHITE = Color(1.f, 1.f, 1.f, 1.f);
const Color Color::BLACK = Color(0.f, 0.f, 0.f, 1.f);
const Color Color::RED = Color(1.f, 0.f, 0.f, 1.f);
const Color Color::YELLOW = Color(1.f, 1.f, 0.f, 1.f);
const Color Color::GREEN = Color(0.f, 1.f, 0.f, 1.f);
const Color Color::CYAN = Color(0.f, 1.f, 1.f, 1.f);
const Color Color::BLUE = Color(0.f, 0.f, 1.f, 1.f);
const Color Color::PINK = Color(1.f, 0.f, 1.f, 1.f);
const Color Color::TRANSPARENT = Color(0.f, 0.f, 0.f, 0.f);

FLAKOR_NS_END
