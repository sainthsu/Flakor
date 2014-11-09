#ifndef _FK_SIZE_H_
#define _FK_SIZE_H_

#include "macros.h"
#include "base/element/Point.h"

FLAKOR_NS_BEGIN

class Point;

class Size
{
public:
	int width;
	int height;
public:
	Size();
    Size(float width, float height);
	Size(const Size& other);
	Size(const Point& point);

	/**
	 *运算符重载
	 */
	Size& operator= (const Size& other);
    Size& operator= (const Point& point);
    Size operator+ (const Size& right) const;
    Size operator- (const Size& right) const;
	Size operator* (float a) const;
    Size operator/ (float a) const;

	void setSize(float width, float height);
    bool equals(const Size& target) const;
};

#define MakeSize(width, height) Size((float)(width), (float)(height))

const Size SizeZero = MakeSize(0,0);

FLAKOR_NS_END

#endif
