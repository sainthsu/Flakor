#ifndef _FK_FLAKOR_H_
#define _FK_FLAKOR_H_

#define FLAKOR_VERSION 0x00000100

int getFlakorVersion()
{
       return FLAKOR_VERSION;
}
//macro
#include "macros.h"

//lang include
#include "./lang/Array.h"
#include "./lang/AutoreleasePool.h"
#include "./lang/Bool.h"
#include "./lang/DataVisitor.h"
#include "./lang/Dictionary.h"
#include "./lang/Double.h"
#include "./lang/Float.h"
#include "./lang/Integer.h"
#include "./lang/Object.h"
#include "./lang/Set.h"
#include "./lang/String.h"
#include "./lang/Zone.h"

//base
#include "base/geometry/Point.h"
#include "base/geometry/Size.h"
#include "base/geometry/Rect.h"
#include "base/geometry/Color.h"
#include "base/geometry/Type.h"


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
        If the string is not well-formed, the function returns CCRectZero.
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
        If the string is not well-formed, the function returns CCPointZero.
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
        If the string is not well-formed, the function returns CCSizeZero.
*/
Size SizeFromString(const char* pszContent);

// end of data_structure group
/// @}

FLAKOR_NS_END
