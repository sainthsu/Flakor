/****************************************************************************
Copyright (c) 2013-2014 Saint.

http://www.flakor.org
****************************************************************************/

#include "core/opengl/shader/Shaders.h"

#define _STRINGIFY(A)  #A
#define STRINGIFY(A)  _STRINGIFY(A)

FLAKOR_NS_BEGIN
//
#include "ccShader_Position_uColor.frag"
#include "ccShader_Position_uColor.vert"

//
#include "ccShader_PositionColor.frag"
#include "ccShader_PositionColor.vert"

//
#include "ccShader_PositionTexture.frag"
#include "ccShader_PositionTexture.vert"

//
#include "ccShader_PositionTextureA8Color.frag"
#include "ccShader_PositionTextureA8Color.vert"

//
#include "ccShader_PositionTextureColor.frag"
#include "ccShader_PositionTextureColor.vert"

//
#include "ccShader_PositionTextureColorAlphaTest.frag"

//
#include "ccShader_PositionTexture_uColor.frag"
#include "ccShader_PositionTexture_uColor.vert"

#include "ccShader_PositionColorLengthTexture.frag"
#include "ccShader_PositionColorLengthTexture.vert"

//
#include "ccShader_Label.vert"
#include "ccShader_Label_df.frag"
#include "ccShader_Label_df_glow.frag"
#include "ccShader_Label_normal.frag"
#include "ccShader_Label_outline.frag"

//
#include "ccShader_3D_PositionTex.vert"
#include "ccShader_3D_Color.frag"
#include "ccShader_3D_ColorTex.frag"
#include "ccShader_3D_PositionNormalTex.vert"
#include "ccShader_3D_ColorNormal.frag"
#include "ccShader_3D_ColorNormalTex.frag"

FLAKOR_NS_END
