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
		virtual void setColor(const Color& color);
		virtual void setColor(float red,float green,float blue);
		virtual void setColor(float red,float green,float blue,float alpha);
		virtual void setRed(float red);
		virtual void setGreen(float green);
		virtual void setBlue(float blue);
		virtual void setAlpha(float alpha);
		virtual Color& getColor();
		virtual float getRed();
		virtual float getGreen();
		virtual float getBlue();
		virtual float getAlpha();
};

FLAKOR_NS_END

#endif
