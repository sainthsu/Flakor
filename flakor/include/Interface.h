#ifndef _FK_INTERFACE_H_
#define _FK_INTERFACE_H_

#include <string>

FLAKOR_NS_BEGIN

/**
 * Color interface that affects CCNode's color and opacity
 */
class IColorable
{
public:
	virtual void setRed(float red);
	virtual void setGreen(float green);
	virtual void setBlue(float blue);
	virtual void setAlpha(float alpha);

	virtual void setColor(float red,float green, float blue);
	virtual void setColor(float red,float green, float blue ,float alpha);
	virtual void setColor(const Color& color) = 0;

	virtual float getRed(void);
	virtual float getGreen(void);
	virtual float getBlue(void);
	virtual float getAlpha(void);

	virtual const Color& getColor(void);
};

class IBlendFunc
{
	public:
    /**
     * Sets the source blending function.
     *
     * @param blendFunc A structure with source and destination factor to specify pixel arithmetic, 
     *                  e.g. {GL_ONE, GL_ONE}, {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA}.
     *
     */
    virtual void setBlendFunc(BlendFunc blendFunc) = 0;

    /**
     * Returns the blending function that is currently being used.
     * 
     * @return A ccBlendFunc structure with source and destination factor which specified pixel arithmetic.
     */
    virtual BlendFunc getBlendFunc(void) = 0;
}

class ITexture :: public IBlendFunc
{
	public:
    /**
     * Returns the currently used texture
     *
     * @return  The texture that is currenlty being used.
     */
    virtual Texture2D* getTexture(void) = 0;

    /**
     * Sets a new texuture. It will be retained.
     *
     * @param   texture A valid Texture2D object, which will be applied to this sprite object.
     * 
     */
    virtual void setTexture(Texture2D *texture) = 0;
}

FLKAOR_NS_END

#endif
