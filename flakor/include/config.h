/****************************************************************************
Copyright (c) 2013-2014 flakor.org

http://www.flakor.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef _FK_CONFIG_H_
#define _FK_CONFIG_H_

#include "target.h"

/**
 @file
 FLAKOR (fk) configuration file
*/

/** @def FK_ENABLE_STACKABLE_ACTIONS
 If enabled, actions that alter the position property (eg: MoveBy, JumpBy, BezierBy, etc..) will be stacked.
 If you run 2 or more 'position' actions at the same time on a entity, then end position will be the sum of all the positions.
 If disabled, only the last run action will take effect.
 
 Enabled by default.
 */
#ifndef FK_ENABLE_STACKABLE_ACTIONS
	#define FK_ENABLE_STACKABLE_ACTIONS 1
#endif

/** @def FK_ENABLE_GL_STATE_CACHE
 If enabled, FLAKOR will maintain an OpenGL state cache internally to avoid unnecessary switches.
 In order to use them, you have to use the following functions, instead of the the GL ones:
    - fkGLUseProgram() instead of glUseProgram()
    - fkGLDeleteProgram() instead of glDeleteProgram()
    - fkGLBlendFunc() instead of glBlendFunc()

 If this functionality is disabled, then fkGLUseProgram(), fkGLDeleteProgram(), fkGLBlendFunc() will call the GL ones, without using the cache.

 It is recommended to enable whenever possible to improve speed.
 If you are migrating your code from GL ES 1.1, then keep it disabled. Once all your code works as expected, turn it on.
 
 Default value: Enabled by default
 
 @since v0.1.0
 */
#ifndef FK_ENABLE_GL_STATE_CACHE
	#define FK_ENABLE_GL_STATE_CACHE 1
#endif

/** @def FK_FIX_ARTIFACTS_BY_STRECHING_TEXEL
If enabled, the texture coordinates will be calculated by using this formula:
- texCoord.left = (rect.origin.x*2+1) / (texture.wide*2);
- texCoord.right = texCoord.left + (rect.size.width*2-2)/(texture.wide*2);

The same for bottom and top.

This formula prevents artifacts by using 99% of the texture.
The "correct" way to prevent artifacts is by using the spritesheet-artifact-fixer.py or a similar tool.

Affected nodes:
- Sprite / SpriteBatchNode and subclasses: LabelBMFont, TMXTiledMap
- LabelAtlas
- QuadParticleSystem
- TileMap

To enabled set it to 1. Disabled by default.
*/
#ifndef FK_FIX_ARTIFACTS_BY_STRECHING_TEXEL
	#define FK_FIX_ARTIFACTS_BY_STRECHING_TEXEL 0
#endif

/** @def FK_DIRECTOR_FPS_INTERVAL
 Seconds between FPS updates.
 0.5 seconds, means that the FPS number will be updated every 0.5 seconds.
 Having a bigger number means a more reliable FPS
 
 Default value: 0.5f
 */
#ifndef FK_DIRECTOR_STATS_INTERVAL
	#define FK_DIRECTOR_STATS_INTERVAL (0.5f)
#endif

/** @def FK_DIRECTOR_FPS_POSITION
 Position of the FPS

 Default: 0,0 (bottom-left corner)
 */
#ifndef FK_DIRECTOR_FPS_POSITION
	#define FK_DIRECTOR_FPS_POSITION PointMake(0,0)
#endif

/** @def FK_DIRECTOR_DISPATCH_FAST_EVENTS
 If enabled, and only when it is used with CCFastDirector, the main loop will wait 0.04 seconds to
 dispatch all the events, even if there are not events to dispatch.
 If your game uses lot's of events (eg: touches) it might be a good idea to enable this feature.
 Otherwise, it is safe to leave it disabled.
 
 To enable set it to 1. Disabled by default.
 
 @warning This feature is experimental
 */
#ifndef FK_DIRECTOR_DISPATCH_FAST_EVENTS
	#define FK_DIRECTOR_DISPATCH_FAST_EVENTS 0
#endif

/** @def FK_ENTITY_RENDER_SUBPIXEL
 If enabled, the Entity objects (Sprite, Label,etc) will be able to render in subpixels.
 If disabled, integer pixels will be used.
 
 To enable set it to 1. Enabled by default.
 */
#ifndef FK_ENTITY_RENDER_SUBPIXEL
#define FK_ENTITY_RENDER_SUBPIXEL 1
#endif

/** @def FK_SPRITEBATCH_RENDER_SUBPIXEL
 If enabled, the Sprite objects rendered with SpriteBatch will be able to render in subpixels.
 If disabled, integer pixels will be used.
 
 To enable set it to 1. Enabled by default.
 */
#ifndef FK_SPRITEBATCH_RENDER_SUBPIXEL
#define FK_SPRITEBATCH_RENDER_SUBPIXEL    1
#endif

/** @def FK_TEXTURE_ATLAS_USE_TRIANGLE_STRIP
 Use GL_TRIANGLE_STRIP instead of GL_TRIANGLES when rendering the texture atlas.
 It seems it is the recommend way, but it is much slower, so, enable it at your own risk
 
 To enable set it to a value different than 0. Disabled by default.

 */
#ifndef FK_TEXTURE_ATLAS_USE_TRIANGLE_STRIP
#define FK_TEXTURE_ATLAS_USE_TRIANGLE_STRIP 0
#endif

/** @def FK_TEXTURE_ATLAS_USE_VAO
 By default, TextureAtlas (used by many cocos2d classes) will use VAO (Vertex Array Objects).
 Apple recommends its usage but they might consume a lot of memory, specially if you use many of them.
 So for certain cases, where you might need hundreds of VAO objects, it might be a good idea to disable it.
 
 To disable it set it to 0. Enabled by default.
 
 */
#ifndef FK_TEXTURE_ATLAS_USE_VAO
    #if (FK_TARGET_PLATFORM == FK_PLATFORM_IOS)
        #define FK_TEXTURE_ATLAS_USE_VAO 1
    #else
        /* Some Windows display adapter driver cannot support VAO. */
        /* Some android devices cannot support VAO very well, so we disable it by default for android platform. */
        /* Blackberry also doesn't support this feature. */
		#define FLAKOR_TEXTURE_ATLAS_USE_VAO 0
    #endif
#endif


/** @def FK_USE_LA88_LABELS
 If enabled, it will use LA88 (Luminance Alpha 16-bit textures) for LabelTTF objects.
 If it is disabled, it will use A8 (Alpha 8-bit textures).
 LA88 textures are 6% faster than A8 textures, but they will consume 2x memory.

 This feature is enabled by default.

 @since v0.01.0
 */
#ifndef FK_USE_LA88_LABELS
	#define FK_USE_LA88_LABELS 1
#endif

/** @def FK_SPRITE_DEBUG_DRAW
 If enabled, all subclasses of Sprite will draw a bounding box
 Useful for debugging purposes only. It is recommended to leave it disabled.
 
 To enable set it to a value different than 0. Disabled by default:
 0 -- disabled
 1 -- draw bounding box
 2 -- draw texture box
 0 -- disabled
 1 -- draw bounding box
 2 -- draw texture box
*/
#ifndef FK_SPRITE_DEBUG_DRAW
#define FK_SPRITE_DEBUG_DRAW 0
#endif

/** @def FK_SPRITEBATCH_DEBUG_DRAW
If enabled, all subclasses of Sprite that are rendered using an SpriteBatch draw a bounding box.
Useful for debugging purposes only. It is recommended to leave it disabled.

To enable set it to a value different than 0. Disabled by default.
*/
#ifndef FK_SPRITEBATCH_DEBUG_DRAW
#define FK_SPRITEBATCH_DEBUG_DRAW 0
#endif

/** @def FK_LABELBMFONT_DEBUG_DRAW
If enabled, all subclasses of LabelBMFont will draw a bounding box
Useful for debugging purposes only. It is recommended to leave it disabled.

To enable set it to a value different than 0. Disabled by default.
*/
#ifndef FK_LABELBMFONT_DEBUG_DRAW
#define FK_LABELBMFONT_DEBUG_DRAW 0
#endif

/** @def FK_LABELATLAS_DEBUG_DRAW
 If enabled, all subclasses of LabeltAtlas will draw a bounding box
 Useful for debugging purposes only. It is recommended to leave it disabled.
 
 To enable set it to a value different than 0. Disabled by default.
 */
#ifndef FK_LABELATLAS_DEBUG_DRAW
#define FK_LABELATLAS_DEBUG_DRAW 0
#endif

/** @def FK_ENABLE_PROFILERS
 If enabled, will activate various profilers within FLAKOR. This statistical data will be output to the console
 once per second showing average time (in milliseconds) required to execute the specific routine(s).
 Useful for debugging purposes only. It is recommended to leave it disabled.
 
 To enable set it to a value different than 0. Disabled by default.
 */
#ifndef FK_ENABLE_PROFILERS
#define FK_ENABLE_PROFILERS 0
#endif

/** Enable Lua engine debug log */
#ifndef FK_LUA_ENGINE_DEBUG
#define FK_LUA_ENGINE_DEBUG 0
#endif


/** Enable KUNKUA engine debug log */
#ifndef FK_KUNKUA_ENGINE_DEBUG
#define FK_KUNKUA_ENGINE_DEBUG 0
#endif

#endif // _FK_CONFIG_H_
