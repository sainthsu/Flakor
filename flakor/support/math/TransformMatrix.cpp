#include "TransformMatrix.h"

#define PI 3.14159265
/**
 *[ a , b , u ]
 *[ c , d , v ]
 *[ tx, ty, w ]
 */
TransformMatrix::TransformMatrix()
{
	this.indentity();
}


TransformMatrix::indentity():
a(1.0f),b(0.0f),c(0.0f),d(1.0f),tx(0.0f),ty(0.0f)
{
}

TransformMatrix::setMatrix(const TransformMatrix& matrix)
{
}

TransformMatrix::reset()
{
	this.indentity();
}

TransformMatrix::translate(float tX,float tY)
{
	this.tx += tX;
	this.ty += tY;
}

TransformMatrix::scale(float sX, float sY)
{
	this.a *= sX;
	this.b *= sY;
	this.c *= sX;
	this.d *= sY;
	this.tx *= sX;
	this.ty *= sY;
}

TransformMatrix::skew(float skewX,float skewY)
{
	float tanX = (float) tan(-PI/180.f * skewX);
	float tanY = (float) tan(-PI/180.f * skewY);

	float a = this.a;
	float b = this.b;
	float c = this.c;
	float d = this.d;
	float tx = this.tx;
	float ty = this.ty;

	this.a = a + b * tanX;
	this.b = a * tanY + b;
	this.c = c + d * tanX;
	this.d = c * tanY + d;
	this.tx = tx + ty * tanX;
	this.ty = tx * tanY + ty;
}

TransformMatrix::rotate(float angle)
{
	float angleRad = PI/180.f * angle;
	float sin = sin(angleRad);
	float cos = FloatMath.cos(angleRad);

	float a = this.a;
	float b = this.b;
	float c = this.c;
	float d = this.d;
	float tx = this.tx;
	float ty = this.ty;

	this.a = a * cos - b * sin;
	this.b = a * sin + b * cos;
	this.c = c * cos - d * sin;
	this.d = c * sin + d * cos;
	this.tx = tx * cos - ty * sin;
	this.ty = tx * sin + ty * cos;
}

TransformMatrix::concat(const TransformMatrix& right)
{
}

TransformMatrix::preconcat(const TransformMatrix& left)
{
}

TransformMatrix::inverse()
{
}

Point TransformMatrix::transform(const Point& point)
{
}

const char* TransformMatrix::toString()
{
}


