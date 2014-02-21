#ifdef _FK_POINT_H_
#define _FK_POINT_H_

#define MakePoint(__X__,__Y__) Point::make(__X__,__Y__);
#include <math.h>

FLAKOR_NS_START

typedef enum {
	2d,
	3d,
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

	Point* make(int x,int y);
	Point* makeZero(void);
	
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

	transforn();

}

FLAKOR_NS_END

#endif
