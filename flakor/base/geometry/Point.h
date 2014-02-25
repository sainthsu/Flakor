#ifdef _FK_POINT_H_
#define _FK_POINT_H_

#include <math.h>

FLAKOR_NS_START

typedef enum {
	2D,
	3D,
}PointType;


class Point
{
	public:
		float x;
		float y;
		float z;
		PointType type;
	public:

	Point();	
	Point(int x,int y);
	Point(const Point& other);
	Point(const Size& size);

	Point make(int x,int y) const;
	Point makeZero(void) const;
	
	~Point(void);

	/**
	 *运算符重载
	 */
	Point& operator= (const CCPoint& other);
    Point& operator= (const CCSize& size);
    Point operator+(const CCPoint& right) const;
    Point operator-(const CCPoint& right) const;
    Point operator-() const;
    Point operator*(float a) const;
    Point operator/(float a) const;

	void setPoint(float x, float y);
    bool equals(const Point& target) const;

	/** @returns if points have fuzzy equality which means 
	 * equal with some degree of variance.
     */
    bool fuzzyEquals(const CCPoint& target, float variance) const;

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

	transform();

}

#define MakePoint(x,y) Point((float)(x),(float)(y));
const Point PointZero = MakePoint(0,0);

FLAKOR_NS_END

#endif
