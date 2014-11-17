#ifndef _FK_TYPE_H_
#define _FK_TYPE_H_

#include "core/opengl/GL.h"

FLAKOR_NS_BEGIN

//! Blend Function used for textures
struct BlendFunc
{
    //! source blend function
    GLenum src;
    //! destination blend function
    GLenum dst;

    //! Blending disabled. Uses {GL_ONE, GL_ZERO}
    static const BlendFunc DISABLE;
    //! Blending enabled for textures with Alpha premultiplied. Uses {GL_ONE, GL_ONE_MINUS_SRC_ALPHA}
    static const BlendFunc ALPHA_PREMULTIPLIED;
    //! Blending enabled for textures with Alpha NON premultiplied. Uses {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA}
    static const BlendFunc ALPHA_NON_PREMULTIPLIED;
    //! Enables Additive blending. Uses {GL_SRC_ALPHA, GL_ONE}
    static const BlendFunc ADDITIVE;
	
	

	bool operator==(const BlendFunc &a) const
    {
        return src == a.src && dst == a.dst;
    }

    bool operator!=(const BlendFunc &a) const
    {
        return src != a.src || dst != a.dst;
    }

    bool operator<(const BlendFunc &a) const
    {
        return src < a.src || (src == a.src && dst < a.dst);
    }

};

const BlendFunc BlendFunc::DISABLE = {GL_ONE, GL_ZERO};
const BlendFunc BlendFunc::ALPHA_PREMULTIPLIED = {GL_ONE, GL_ONE_MINUS_SRC_ALPHA};
const BlendFunc BlendFunc::ALPHA_NON_PREMULTIPLIED = {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA};
const BlendFunc BlendFunc::ADDITIVE = {GL_SRC_ALPHA, GL_ONE};

FLAKOR_NS_END

#endif
