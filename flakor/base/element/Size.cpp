
#include "Size.h"

FLAKOR_NS_BEGIN

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

Size& Size::operator= (const CCPoint& point)
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

FLAKOR_NS_END
