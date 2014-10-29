#ifndef _FK_TYPE_H_
#define _FK_TYPE_H_

//! Blend Function used for textures
typedef struct _fkBlendFunc
{
    //! source blend function
    GLenum src;
    //! destination blend function
    GLenum dst;
} BlendFunc;

static const BlendFunc BlendFuncDisable = {GL_ONE, GL_ZERO};

#endif