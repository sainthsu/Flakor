
#include "Point.h"

FLAKOR_NS_BEGIN

/**
 * 利用成员变量初始化列表初始化类
 */
Point::Point(void) :x(0),y(0)
{
}

Point::Point(float x, float y) : x(x),y(y)
{
}

Point::Point(const Point& other) : x(other.x),y(other.y)
{
}

Point::Point(const Size& size) : x(size.width),y(size.height)
{
}

Point& Point::operator= (const Point& other)
{
	setPoint(other.x,other.y);
	return *this;
}

Point& Point::operator= (const Size& size)
{
	setPoint(size.width,size.height);
	return *this;
}

Point Point::operator+(const Point& right) const
{
    return Point(this->x + right.x, this->y + right.y);
}

Point Point::operator-(const Point& right) const
{
    return Point(this->x - right.x, this->y - right.y);
}

Point Point::operator-() const
{
	return Point(-x, -y);
}

Point Point::operator*(float a) const
{
    return Point(this->x * a, this->y * a);
}

Point Point::operator/(float a) const
{
	Assert::ok(a, "Point division by 0.");
    return Point(this->x / a, this->y / a);
}

void Point::setPoint(float x, float y)
{
    this->x = x;
    this->y = y;
}

bool Point::equals(const Point& target) const
{
    return (fabs(this->x - target.x) < FLT_EPSILON)
        && (fabs(this->y - target.y) < FLT_EPSILON);
}

bool Point::fuzzyEquals(const Point& b, float var) const
{
    if(x - var <= b.x && b.x <= x + var)
        if(y - var <= b.y && b.y <= y + var)
            return true;
    return false;
}

float Point::getAngle(const Point& other) const
{
    Point a2 = normalize();
    Point b2 = other.normalize();
    float angle = atan2f(a2.cross(b2), a2.dot(b2));
    if( fabs(angle) < FLT_EPSILON ) return 0.f;
    return angle;
}

Point Point::rotateByAngle(const Point& pivot, float angle) const
{
    return pivot + (*this - pivot).rotate(Point::forAngle(angle));
}

FLAKOR_NS_END
