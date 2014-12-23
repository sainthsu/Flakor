#ifndef _FK_ELEMENT_H_
#define _FK_ELEMENT_H_

#include <math.h>
#include <float.h>

FLAKOR_NS_BEGIN

typedef enum {
	POINT_2D,
	POINT_3D,
} PointType;

class Size;

class Point
{
	public:
		float x;
		float y;
		float z;
		PointType type;
		const static Point Zero;
	public:

	Point();	
	Point(float x,float y);
	Point(float x,float y,float z);
	Point(const Point& other);
	Point(const Size& size);

	static Point make(float x,float y);
	static Point make(float x,float y,float z);
	static Point makeZero(void);
	
	~Point(void);

	/**
	 *运算符重载
	 */
	Point& operator= (const Point& other);
    Point& operator= (const Size& size);
    Point operator+(const Point& right) const;
    Point operator-(const Point& right) const;
    Point operator-() const;
    Point operator*(float a) const;
    Point operator/(float a) const;

	void setPoint(float x, float y);
	void setPoint(float x, float y, float z);
	void setPoint(const Point& other);
	void setType(PointType type);
	PointType getType() const;

    bool equals(const Point& target) const;

	/** @returns if points have fuzzy equality which means 
	 * equal with some degree of variance.
     */
    bool fuzzyEquals(const Point& target, float variance) const;

	/**
	 * getter
	 */
	inline float getLength() const {
        return sqrtf(x*x + y*y);
    };

	 /** Calculates the square length of a CCPoint (not calling sqrt() )
     * @return float
     */
    inline float getLengthSq() const {
        return dot(*this); //x*x + y*y;
    };

    /** Calculates the square distance between two points (not calling sqrt() )
     @return float
    */
    inline float getDistanceSq(const Point& other) const {
        return (*this - other).getLengthSq();
    };

    /** Calculates the distance between two points
     @return float
     */
    inline float getDistance(const Point& other) const {
        return (*this - other).getLength();
    };

    /** @returns the angle in radians between this vector and the x axis
    */
    inline float getAngle() const {
        return atan2f(y, x);
    };

    /** @returns the angle in radians between two vector directions
    */
    float getAngle(const Point& other) const;

    /** Calculates dot product of two points.
     */
    inline float dot(const Point& other) const {
        return x*other.x + y*other.y;
    };

    /** Calculates cross product of two points.
     @return float
     */
    inline float cross(const Point& other) const {
        return x*other.y - y*other.x;
    };

    /** Calculates perpendicular of v, rotated 90 degrees counter-clockwise -- cross(v, perp(v)) >= 0
     @return Point
     */
    inline Point getPerp() const {
        return Point(-y, x);
    };

    /** Calculates perpendicular of v, rotated 90 degrees clockwise -- cross(v, rperp(v)) <= 0
     @return Point
     */
    inline Point getRPerp() const {
        return Point(y, -x);
    };

    /** Calculates the projection of this over other.
     @return Point
     */
    inline Point project(const Point& other) const {
        return other * (dot(other)/other.dot(other));
    };

    /** Complex multiplication of two points ("rotates" two points).
     @return Point vector with an angle of this.getAngle() + other.getAngle(),
     and a length of this.getLength() * other.getLength().
     */
    inline Point rotate(const Point& other) const {
        return Point(x*other.x - y*other.y, x*other.y + y*other.x);
    };

    /** Unrotates two points.
     @return Point vector with an angle of this.getAngle() - other.getAngle(),
     and a length of this.getLength() * other.getLength().
     */
    inline Point unrotate(const Point& other) const {
        return Point(x*other.x + y*other.y, y*other.x - x*other.y);
    };

    /** Returns point multiplied to a length of 1.
     * If the point is 0, it returns (1, 0)
     @return Point
     */
    inline Point normalize() const {
        float length = getLength();
        if(length == 0.) return Point(1.f, 0);
        return *this / getLength();
    };

    /** Linear Interpolation between two points a and b
     @returns
        alpha == 0 ? a
        alpha == 1 ? b
        otherwise a value between a..b
     */
    inline Point lerp(const Point& other, float alpha) const {
        return *this * (1.f - alpha) + other * alpha;
    };

    /** Rotates a point counter clockwise by the angle around a pivot
     @param pivot is the pivot, naturally
     @param angle is the angle of rotation ccw in radians
     @returns the rotated point
     */
    Point rotateByAngle(const Point& pivot, float angle) const;

    static inline Point forAngle(const float a)
    {
    	return Point(cosf(a), sinf(a));
    }

};

class Size
{
public:
	float width;
	float height;
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

#define PointMake(x,y) Point((float)(x),(float)(y))
const Point PointZero = PointMake(0.f,0.f);

#define SizeMake(width, height) Size((float)(width), (float)(height))
const Size SizeZero = SizeMake(0.f,0.f);

#define RectMake(x,y,width, height) Rect((float)(x), (float)(y) , (float)(width), (float)(height))
const Rect RectZero = RectMake(0.f,0.f,0.f,0.f);

FLAKOR_NS_END

#endif
