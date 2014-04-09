
#ifndef _FK_RECT_H_
#define _FK_RECT_H_

#include "base/geometry/Point.h"
#include "base/geometry/Size.h"

class Rect
{
public:
    Point origin;
    Size  size;

public:
    Rect();
    Rect(float x, float y, float width, float height);
    Rect(const Rect& other);
    
    Rect& operator= (const Rect& other);
    void setRect(float x, float y, float width, float height);

    float getMinX() const; /// return the leftmost x-value of current rect
    float getMidX() const; /// return the midpoint x-value of current rect
    float getMaxX() const; /// return the rightmost x-value of current rect

    float getMinY() const; /// return the bottommost y-value of current rect
    float getMidY() const; /// return the midpoint y-value of current rect
    float getMaxY() const; /// return the topmost y-value of current rect

    bool equals(const Rect& rect) const;   
    bool containsPoint(const Point& point) const;
    bool intersectsRect(const Rect& rect) const;
};

#define MakeRect(x,y,width, height) Rect((float)(x), (float)(y) , (float)(width), (float)(height))

const Rect RectZero = RectMake(0,0,0,0); 

#endif
