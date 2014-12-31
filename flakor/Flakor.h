#ifndef _FK_FLAKOR_H_
#define _FK_FLAKOR_H_

#define FLAKOR_VERSION 0x00000100
#define FLAKOR_AUTHOR "Saint Hsu(saint@aliyun.com)"

//macro
#include "macros.h"

//core opengl
#include "core/opengl/GL.h"

//lang include
#include "base/lang/Object.h"
#include "base/lang/Array.h"
#include "base/lang/AutoreleasePool.h"
#include "base/lang/Types.h"
#include "base/lang/DataVisitor.h"
#include "base/lang/Dictionary.h"
#include "base/lang/Set.h"
#include "base/lang/Str.h"
#include "base/lang/Zone.h"

//element
#include "base/element/Element.h"
#include "base/element/Helper.h"
#include "base/element/Color.h"

//entity
#include "2d/Entity.h"
#include "2d/Scene.h"
#include "2d/Sprite.h"

//core systems
//input
#include "core/input/Touch.h"
//opengl
#include "core/opengl/GPUInfo.h"
#include "core/opengl/GLProgram.h"
#include "core/opengl/shader/Shaders.h"
#include "core/opengl/vbo/VBO.h"
#include "core/opengl/texture/Image.h"
#include "core/opengl/texture/Texture2D.h"

//math
#include "math/GLMatrix.h"

#include "platform/Engine.h"
#include "platform/Application.h"
#include "platform/Game.h"

const char* getFlakorVersion();

#endif
