/****************************************************************************
Copyright (c) 2013-2014 Saint Hsu(saint@aliyun.com)

http://www.flakor.org
****************************************************************************/
#ifndef _FK_SHADER_H_
#define _FK_SHADER_H_

#include "platform/GL.h"
#include "platform/PlatformMacros.h"

FLAKOR_NS_BEGIN

/**
 * @addtogroup shaders
 * @{
 */
class Shader
{
	const GLchar * Position_uColor_frag;
	const GLchar * Position_uColor_vert;

	const GLchar * PositionColor_frag;
	const GLchar * PositionColor_vert;

	const GLchar * PositionTexture_frag;
	const GLchar * PositionTexture_vert;

	const GLchar * PositionTextureA8Color_frag;
	const GLchar * PositionTextureA8Color_vert;

	const GLchar * PositionTextureColor_frag;
	const GLchar * PositionTextureColor_vert;

	const GLchar * PositionTextureColor_noMVP_frag;
	const GLchar * PositionTextureColor_noMVP_vert;

	const GLchar * PositionTextureColorAlphaTest_frag;

	const GLchar * PositionTexture_uColor_frag;
	const GLchar * PositionTexture_uColor_vert;

	const GLchar * PositionColorLengthTexture_frag;
	const GLchar * PositionColorLengthTexture_vert;

	const GLchar * LabelDistanceFieldNormal_frag;
	const GLchar * LabelDistanceFieldGlow_frag;
	const GLchar * LabelNormal_frag;
	const GLchar * LabelOutline_frag;

	const GLchar * Label_vert;

	const GLchar * 3D_PositionTex_vert;
	const GLchar * 3D_SkinPositionTex_vert;
	const GLchar * 3D_ColorTex_frag;
	const GLchar * 3D_Color_frag;
	const GLchar * 3D_PositionNormalTex_vert;
	const GLchar * 3D_SkinPositionNormalTex_vert;
	const GLchar * 3D_ColorNormalTex_frag;
	const GLchar * 3D_ColorNormal_frag;
}
// end of shaders group
/// @}
FLAKOR_NS_END

#endif /* _FK_SHADER_H__ */
