#ifndef _FK_INTERFACE_H_
#define _FK_INTERFACE_H_

#include <string>
#include "base/element/Blendfunc.h"

FLAKOR_NS_BEGIN

class Texture2D;

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
    virtual void setBlendFunc(const BlendFunc &blendFunc) = 0;

    /**
     * Returns the blending function that is currently being used.
     * 
     * @return A ccBlendFunc structure with source and destination factor which specified pixel arithmetic.
     */
    virtual const BlendFunc& getBlendFunc(void) const = 0;
};

class ITexture : public IBlendFunc
{
	public:
    /**
     * Returns the currently used texture
     *
     * @return  The texture that is currenlty being used.
     */
    virtual Texture2D* getTexture(void) const = 0;

    /**
     * Sets a new texuture. It will be retained.
     *
     * @param   texture A valid Texture2D object, which will be applied to this sprite object.
     * 
     */
    virtual void setTexture(Texture2D *texture) = 0;
};

class ITextureListener {
	public:
		virtual void onBeginLoad(ITexture *tex) const = 0;
		virtual void onFinishLoad(ITexture *tex) const = 0;
		virtual void onBeginUnload(ITexture *tex) const = 0;
		virtual void onFinishUnLoad(ITexture *tex) const = 0;
};

FLAKOR_NS_END

#endif
