#ifndef _FK_TYPE_H_
#define _FK_TYPE_H_

#include "core/opengl/GL.h"

FLAKOR_NS_BEGIN

//! Blend Function used for textures
typedef struct _fkBlendFunc
{
    //! source blend function
    GLenum src;
    //! destination blend function
    GLenum dst;
} BlendFunc;

static const BlendFunc BlendFuncDisable = {GL_ONE, GL_ZERO};

FLAKOR_NS_END

#endif
