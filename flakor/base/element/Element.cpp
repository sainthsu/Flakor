#include "macros.h"
#include "base/element/Element.h"

FLAKOR_NS_BEGIN

/**
 * 利用成员变量初始化列表初始化类
 */
Point::Point(void) :x(0),y(0),z(0),type(POINT_2D)
{
}

Point::Point(float x, float y) : x(x),y(y),z(0),type(POINT_2D)
{
}

Point::Point(float x, float y ,float z) : x(x),y(y),z(z),type(POINT_3D)
{
}

Point::Point(const Point& other) : x(other.x),y(other.y),z(other.z),type(other.type)
{
}

Point::Point(const Size& size) : x(size.width),y(size.height),z(0),type(POINT_2D)
{
}

Point::~Point()
{
	x=0;
	y=0;
	z=0;
}

Point& Point::operator= (const Point& other)
{
	setPoint(other);
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
	FKAssert(a, "Point division by 0.");
	//Assert::ok(a, "Point division by 0.");
    return Point(this->x / a, this->y / a);
}

void Point::setPoint(float x, float y)
{
    this->x = x;
    this->y = y;
}

void Point::setPoint(float x, float y ,float z)
{
    this->x = x;
    this->y = y;
	this->z = z;
}

void Point::setPoint(const Point& other)
{
    this->x = other.x;
    this->y = other.y;
	this->z = other.z;
	this->type = other.type;
}

void Point::setType(PointType type)
{
	this->type = type;
}

PointType Point::getType() const
{
	return this->type;
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


//Size definition

Size::Size(void) : width(0),height(0)
{
}

Size::Size(float width, float height) : width(width), height(height)
{
}

Size::Size(const Size& other) : width(other.width), height(other.height)
{
}

Size::Size(const Point& point) : width(point.x), height(point.y)
{
}

Size& Size::operator= (const Size& other)
{
    setSize(other.width, other.height);
    return *this;
}

Size& Size::operator= (const Point& point)
{
    setSize(point.x, point.y);
    return *this;
}

Size Size::operator+(const Size& right) const
{
    return Size(this->width + right.width, this->height + right.height);
}

Size Size::operator-(const Size& right) const
{
    return Size(this->width - right.width, this->height - right.height);
}

Size Size::operator*(float a) const
{
    return Size(this->width * a, this->height * a);
}

Size Size::operator/(float a) const
{
	FKAssert(a, "Size division by 0.");
    return Size(this->width / a, this->height / a);
}

void Size::setSize(float width, float height)
{
    this->width = width;
    this->height = height;
}

bool Size::equals(const Size& target) const
{
    return (fabs(this->width  - target.width)  < FLT_EPSILON)
        && (fabs(this->height - target.height) < FLT_EPSILON);
}


//Rect 

Rect::Rect(void)
{
    setRect(0.0f, 0.0f, 0.0f, 0.0f);
}

Rect::Rect(float x, float y, float width, float height)
{
    setRect(x, y, width, height);
}

Rect::Rect(const Rect& other)
{
    setRect(other.origin.x, other.origin.y, other.size.width, other.size.height);
}

Rect& Rect::operator= (const Rect& other)
{
    setRect(other.origin.x, other.origin.y, other.size.width, other.size.height);
    return *this;
}

void Rect::setRect(float x, float y, float width, float height)
{
    // Rect can support width<0 or height<0
    // FKAssert(width >= 0.0f && height >= 0.0f, "width and height of Rect must not less than 0.");

    origin.x = x;
    origin.y = y;

    size.width = width;
    size.height = height;
}

bool Rect::equals(const Rect& rect) const
{
    return (origin.equals(rect.origin) &&
            size.equals(rect.size));
}

float Rect::getMaxX() const
{
    return (float)(origin.x + size.width);
}

float Rect::getMidX() const
{
    return (float)(origin.x + size.width / 2.0);
}

float Rect::getMinX() const
{
    return origin.x;
}

float Rect::getMaxY() const
{
    return origin.y + size.height;
}

float Rect::getMidY() const
{
    return (float)(origin.y + size.height / 2.0);
}

float Rect::getMinY() const
{
    return origin.y;
}

bool Rect::containsPoint(const Point& point) const
{
    bool bRet = false;

    if (point.x >= getMinX() && point.x <= getMaxX()
        && point.y >= getMinY() && point.y <= getMaxY())
    {
        bRet = true;
    }

    return bRet;
}

bool Rect::intersectsRect(const Rect& rect) const
{
    return !(     getMaxX() < rect.getMinX() ||
             rect.getMaxX() <      getMinX() ||
                  getMaxY() < rect.getMinY() ||
             rect.getMaxY() <      getMinY());
}

FLAKOR_NS_END
