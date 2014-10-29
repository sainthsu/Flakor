
#include "Size.h"

FLAKOR_NS_BEGIN

Size::Size(void) : width(0),height(0)
{
}

CCSize::CCSize(float width, float height) : width(width), height(height)
{
}

CCSize::CCSize(const CCSize& other) : width(other.width), height(other.height)
{
}

CCSize::CCSize(const CCPoint& point) : width(point.x), height(point.y)
{
}

CCSize& CCSize::operator= (const CCSize& other)
{
    setSize(other.width, other.height);
    return *this;
}

CCSize& CCSize::operator= (const CCPoint& point)
{
    setSize(point.x, point.y);
    return *this;
}

CCSize CCSize::operator+(const CCSize& right) const
{
    return CCSize(this->width + right.width, this->height + right.height);
}

CCSize CCSize::operator-(const CCSize& right) const
{
    return CCSize(this->width - right.width, this->height - right.height);
}

CCSize CCSize::operator*(float a) const
{
    return CCSize(this->width * a, this->height * a);
}

CCSize CCSize::operator/(float a) const
{
	CCAssert(a, "CCSize division by 0.");
    return CCSize(this->width / a, this->height / a);
}

void CCSize::setSize(float width, float height)
{
    this->width = width;
    this->height = height;
}

bool CCSize::equals(const CCSize& target) const
{
    return (fabs(this->width  - target.width)  < FLT_EPSILON)
        && (fabs(this->height - target.height) < FLT_EPSILON);
}

FLAKOR_NS_END
