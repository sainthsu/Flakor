/**
 * @author Saint Hsu(saint.hsu@outlook.com)
 * @since 2013.5.12.18:00
 * @copyright Saint Hsu
 * This class represents an affine transformation with the following matrix:
 * <pre>[ a , b , u ]
 * 		[ c , d , v ]
 * 		[ tx, ty, w ]</pre>
 * where:
 * <ul>
 *  <li><b>a</b> is the <b>x scale</b></li>
 *  <li><b>b</b> is the <b>y skew</b></li>
 *  <li><b>c</b> is the <b>x skew</b></li>
 *  <li><b>d</b> is the <b>y scale</b></li>
 *  <li><b>tx</b> is the <b>x translation (position)</b></li>
 *  <li><b>ty</b> is the <b>y translation (position)</b></li>
 *  <li>The u, v, and w positions are static values that remain at 0, 0, and 1 respectively<li>
 * </ul>
 *  x' = x*a + y*c + tx
 *  y' = x*b + y*d + ty
 */

#ifndef _FK_TRANSFORMMATRIX_H_
#define _FK_TRANSFORMMATRIX_H_

#include <math.h>

FLAKOR_NS_BEGIN
/**
 * @author Saint Hsu(saint@aliyun.com)
 * @since 2013.5.12.18:00
 * @copyright Saint Hsu
 * This class represents an affine transformation with the following matrix:
 * <pre>[ a , b , u ]
 * 		[ c , d , v ]
 * 		[ tx, ty, w ]</pre>
 * where:
 * <ul>
 *  <li><b>a</b> is the <b>x scale</b></li>
 *  <li><b>b</b> is the <b>y skew</b></li>
 *  <li><b>c</b> is the <b>x skew</b></li>
 *  <li><b>d</b> is the <b>y scale</b></li>
 *  <li><b>tx</b> is the <b>x translation (position)</b></li>
 *  <li><b>ty</b> is the <b>y translation (position)</b></li>
 *  <li>The u, v, and w positions are static values that remain at 0, 0, and 1 respectively<li>
 * </ul>
 *  x' = x*a + y*c + tx
 *  y' = x*b + y*d + ty
 */
class TransformMatrix
{
	private:
		float a,b,c,b;
		float tx,ty;
	public:
		TransformMatrix();

		/******************************************************
	 	* set to Identity Matrix
	 	* 		[ 1 , 0 , 0 ]
	 	* 		[ 0 , 1 , 0 ]
	 	* 		[ 0 , 0 , 1 ]
		 *****************************************************/
		void indentity();
		void setMatrix(const TransformMatrix& matrix);
		void reset();

		void translate(float tX,float tY) const;
		void scale(float sX, float sY) const;
		void skew(float skewX,float skewY) const;
		void rotate(float angle) const;
		void concat(const TransformMatrix& right) const;
		void preconcat(const TransformMatrix& left) const;
		void inverse();

		Point transform(const Point& point) const;

		const char* toString(void);
		
}

FLAKOR_NS_END
