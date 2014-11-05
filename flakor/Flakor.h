#ifndef _FK_FLAKOR_H_
#define _FK_FLAKOR_H_

#define FLAKOR_VERSION 0x00000100

const char* getFlakorVersion()
{
       return "flakor engine 0.1.0beta";
}
//macro
#include "macros.h"
#include "stdC.h"

//core opengl
#include "core/opengl/GL.h"

//lang include
#include "base/lang/Object.h"
#include "base/lang/Array.h"
#include "base/lang/AutoreleasePool.h"
#include "base/lang/Bool.h"
#include "base/lang/DataVisitor.h"
#include "base/lang/Dictionary.h"
#include "base/lang/Double.h"
#include "base/lang/Float.h"
#include "base/lang/Integer.h"
#include "base/lang/Set.h"
#include "base/lang/String.h"
#include "base/lang/Zone.h"

//element
#include "base/element/Point.h"
#include "base/element/Size.h"
#include "base/element/Rect.h"
#include "base/element/Color.h"
#include "base/element/Type.h"

//entity
#include "base/entity/Entity.h"
#include "base/entity/Scene.h"
#include "base/entity/Sprite.h"

#endif

FLAKOR_NS_BEGIN

/**
 * @addtogroup data_structures
 * @{
 */

/**
@brief Returns a Core Graphics rectangle structure corresponding to the data in a given string.
@param pszContent   A string object whose contents are of the form "{{x,y},{w, h}}",
                    where x is the x coordinate, y is the y coordinate, w is the width, and h is the height.
                    These components can represent integer or float values.
                    An example of a valid string is "{{3,2},{4,5}}".
                    The string is not localized, so items are always separated with a comma.
@return A Core Graphics structure that represents a rectangle.
        If the string is not well-formed, the function returns RectZero.
*/
Rect RectFromString(const char* pszContent);

/**
@brief Returns a Core Graphics point structure corresponding to the data in a given string.
@param pszContent   A string object whose contents are of the form "{x,y}",
                    where x is the x coordinate and y is the y coordinate.
                    The x and y values can represent integer or float values.
                    An example of a valid string is "{3.0,2.5}".
                    The string is not localized, so items are always separated with a comma.
@return A Core Graphics structure that represents a point.
        If the string is not well-formed, the function returns PointZero.
*/
Point PointFromString(const char* pszContent);

/**
@brief Returns a Core Graphics size structure corresponding to the data in a given string.
@param pszContent   A string object whose contents are of the form "{w, h}",
                    where w is the width and h is the height.
                    The w and h values can be integer or float values.
                    An example of a valid string is "{3.0,2.5}".
                    The string is not localized, so items are always separated with a comma.
@return A Core Graphics structure that represents a size.
        If the string is not well-formed, the function returns SizeZero.
*/
Size SizeFromString(const char* pszContent);

// end of data_structure group
/// @}

FLAKOR_NS_END
