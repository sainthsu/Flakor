#ifndef _FK_ICOLORABLE_H_
#define _FK_ICOLORABLE_H_

FLAKOR_NS_BEGIN

class Color;

/**
 * Color interface that affects CCNode's color and opacity
 */
class IColorable
{
	public:
		virtual void setColor(const Color& color) = 0;
		virtual void setColor(float red,float green,float blue) = 0;
		virtual void setColor(float red,float green,float blue,float alpha) = 0;
		virtual void setRed(float red) = 0;
		virtual void setGreen(float green) = 0;
		virtual void setBlue(float blue) = 0;
		virtual void setAlpha(float alpha) = 0;
		virtual Color& getColor() = 0;
		virtual float getRed() = 0;
		virtual float getGreen() = 0;
		virtual float getBlue() = 0;
		virtual float getAlpha() = 0;
};

FLAKOR_NS_END

#endif
