/****************************************************************************
Copyright (c) 2013-2014 Saint Hsu(saint@aliyun.com)

http://www.flakor.org
****************************************************************************/
#ifndef _FK_SHADER_H_
#define _FK_SHADER_H_

#include "core/opengl/GL.h"
#include "targetMacros.h"

FLAKOR_NS_BEGIN

/**
 * @addtogroup shaders
 * @{
 */
class Shader
{
	public:
	static const GLchar * Position_uColor_frag;
	static const GLchar * Position_uColor_vert;

	static const GLchar * PositionColor_frag;
	static const GLchar * PositionColor_vert;

	static const GLchar * PositionTexture_frag;
	static const GLchar * PositionTexture_vert;

	static const GLchar * PositionTextureA8Color_frag;
	static const GLchar * PositionTextureA8Color_vert;

	static const GLchar * PositionTextureColor_frag;
	static const GLchar * PositionTextureColor_vert;

	static const GLchar * PositionTextureColor_noMVP_frag;
	static const GLchar * PositionTextureColor_noMVP_vert;

	static const GLchar * PositionTextureColorAlphaTest_frag;

	static const GLchar * PositionTexture_uColor_frag;
	static const GLchar * PositionTexture_uColor_vert;

	static const GLchar * PositionColorLengthTexture_frag;
	static const GLchar * PositionColorLengthTexture_vert;

	static const GLchar * LabelDistanceFieldNormal_frag;
	static const GLchar * LabelDistanceFieldGlow_frag;
	static const GLchar * LabelNormal_frag;
	static const GLchar * LabelOutline_frag;

	static const GLchar * Label_vert;

	static const GLchar * PositionTex_3D_vert;
	static const GLchar * SkinPositionTex_3D_vert;
	static const GLchar * ColorTex_3D_frag;
	static const GLchar * Color_3D_frag;
	static const GLchar * PositionNormalTex_3D_vert;
	static const GLchar * SkinPositionNormalTex_3D_vert;
	static const GLchar * ColorNormalTex_3D_frag;
	static const GLchar * ColorNormal_3D_frag;
};
// end of shaders group
/// @}
FLAKOR_NS_END

#endif /* _FK_SHADER_H__ */
