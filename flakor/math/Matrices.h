/**
 * @author Saint Hsu(saint@aliyun.com)
 * @since 2013.5.12.18:00
 * @copyright Saint Hsu
 * This file contains 3 kinds of matrice:
 * Matrix2 for 2x2
 * Matrix3 for 3x3
 * Matrix4 for 4x4
 *
 * All matrices are column major. (OpenGL uses column-major matrix)
 * | 0 2 |    | 0 3 6 |    |  0  4  8  12 |
 * | 1 3 |    | 1 4 7 |    |  1  5  9  13 |
 *            | 2 5 8 |    |  2  6 10  14 |
 *                         |  3  7 11  15 |
 * last modified at 2014-09-22
 */

#ifndef _FK_MATRICES_H_
#define _FK_MATRICES_H_

#include <iostream>

#include "math/Vectors.h"
#include "base/element/Element.h"

enum MATRIX_MAJOR{
    ROW_MAJOR = 1,
    COLUMN_MAJOR
};

FLAKOR_NS_BEGIN

///////////////////////////////////////////////////////////////////////////
// 2x2 matrix
// |xx xy|
// |yx yy|
///////////////////////////////////////////////////////////////////////////
class Matrix2
{
	public:
    // constructors
    Matrix2();  // init with identity
    Matrix2(const float src[4],enum MATRIX_MAJOR major);
    Matrix2(float xx, float xy, float yx, float yy);

    //setter
    void        set(const float src[4],enum MATRIX_MAJOR major);
    void        set(float xx, float xy, float yx, float yy);
    void        setRow(int index, const float row[2]);
    void        setRow(int index, const Vector2& v);
    void        setColumn(int index, const float col[2]);
    void        setColumn(int index, const Vector2& v);

    //getter
    const float* get() const;
    float       getDeterminant();

    Matrix2&    identity();
    Matrix2&    transpose();                            // transpose itself and return reference
    Matrix2&    invert();

    // operators
    Matrix2     operator+(const Matrix2& rhs) const;    // add rhs
    Matrix2     operator-(const Matrix2& rhs) const;    // subtract rhs
    Matrix2&    operator+=(const Matrix2& rhs);         // add rhs and update this object
    Matrix2&    operator-=(const Matrix2& rhs);         // subtract rhs and update this object
    Vector2     operator*(const Vector2& rhs) const;    // multiplication: v' = M * v
    Matrix2     operator*(const Matrix2& rhs) const;    // multiplication: M3 = M1 * M2
    Matrix2&    operator*=(const Matrix2& rhs);         // multiplication: M1' = M1 * M2
    bool        operator==(const Matrix2& rhs) const;   // exact compare, no epsilon
    bool        operator!=(const Matrix2& rhs) const;   // exact compare, no epsilon
    float       operator[](int index) const;            // subscript operator v[0], v[1]
    float&      operator[](int index);                  // subscript operator v[0], v[1]

    friend Matrix2 operator-(const Matrix2& m);                     // unary operator (-)
    friend Matrix2 operator*(float scalar, const Matrix2& m);       // pre-multiplication 左乘，给定矩阵乘以当前矩阵
    friend Vector2 operator*(const Vector2& vec, const Matrix2& m); // pre-multiplication
    friend std::ostream& operator<<(std::ostream& os, const Matrix2& m);

private:
    float m[4];
};

/**
 * 
 * This matrix represents an affine transformation with the following matrix:
 * <pre>[ a , c , tx ]
 * 		[ b , d , ty ]
 * 		[ u , v , w  ]</pre>
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
///////////////////////////////////////////////////////////////////////////
// 3x3 matrix
///////////////////////////////////////////////////////////////////////////
class Matrix3
{
public:
    // constructors
    Matrix3();  // init with identity
    Matrix3(const float src[9],enum MATRIX_MAJOR major);
    Matrix3(float xx, float xy, float xz,
            float yx, float yy, float yz,
            float zx, float zy, float zz);
    Matrix3(const Matrix2& matrix);

    void        set(const float src[9],enum MATRIX_MAJOR major);
    void        set(float xx, float xy, float xz,
                    float yx, float yy, float yz,
                    float zx, float zy, float zz);
    void        setRow(int index, const float row[3]);
    void        setRow(int index, const Vector3& v);
    void        setColumn(int index, const float col[3]);
    void        setColumn(int index, const Vector3& v);

    const float* get() const;
    float       getDeterminant();

    /******************************************************
     * set to Identity Matrix
     * 		[ 1 , 0 , 0 ]
     * 		[ 0 , 1 , 0 ]
     * 		[ 0 , 0 , 1 ]
     *****************************************************/
    Matrix3&    identity();
    Matrix3&    transpose();                            // transpose itself and return reference
    Matrix3&    invert();

    // operators
    Matrix3     operator+(const Matrix3& rhs) const;    // add rhs
    Matrix3     operator-(const Matrix3& rhs) const;    // subtract rhs
    Matrix3&    operator+=(const Matrix3& rhs);         // add rhs and update this object
    Matrix3&    operator-=(const Matrix3& rhs);         // subtract rhs and update this object
    Vector3     operator*(const Vector3& rhs) const;    // multiplication: v' = M * v
    Matrix3     operator*(const Matrix3& rhs) const;    // multiplication: M3 = M1 * M2
    Matrix3&    operator*=(const Matrix3& rhs);         // multiplication: M1' = M1 * M2
    bool        operator==(const Matrix3& rhs) const;   // exact compare, no epsilon
    bool        operator!=(const Matrix3& rhs) const;   // exact compare, no epsilon
    float       operator[](int index) const;            // subscript operator v[0], v[1]
    float&      operator[](int index);                  // subscript operator v[0], v[1]

    friend Matrix3 operator-(const Matrix3& m);                     // unary operator (-)
    friend Matrix3 operator*(float scalar, const Matrix3& m);       // pre-multiplication
    friend Vector3 operator*(const Vector3& vec, const Matrix3& m); // pre-multiplication

protected:

private:
    float m[9];

};

// | m[0] m[4] m[8]  m[12] |     | m11 m21 m31 m41 |     | a c 0 tx |
// | m[1] m[5] m[9]  m[13] |     | m12 m22 m32 m42 |     | b d 0 ty |
// | m[2] m[6] m[10] m[14] | <=> | m13 m23 m33 m43 | <=> | 0 0 1  0 |
// | m[3] m[7] m[11] m[15] |     | m14 m24 m34 m44 |     | 0 0 0  1 |
///////////////////////////////////////////////////////////////////////////
// 4x4 matrix
///////////////////////////////////////////////////////////////////////////
class Matrix4
{
public:
	friend class Vector3;
    friend class Vector4;
    friend class Quaternion;

    // constructors
    Matrix4();  // 初始化为单位矩阵（init with identity） 
    Matrix4(const float src[16],enum MATRIX_MAJOR major); //通过数组初始化 init with an array of 16 elements
    Matrix4(float xx, float xy, float xz, float xw,
            float yx, float yy, float yz, float yw,
            float zx, float zy, float zz, float zw,
            float wx, float wy, float wz, float ww);//传入每一项初始化 init with each argument
    Matrix4(const Matrix3& matrix);

	static Matrix4 make(float xx, float xy, float xz, float xw,
            	  float yx, float yy, float yz, float yw,
                  float zx, float zy, float zz, float zw,
                  float wx, float wy, float wz, float ww);

	//settter and getter
    void        set(const float src[16],enum MATRIX_MAJOR major);//通过数组设置
    void        set(float xx, float xy, float xz, float xw,
                    float yx, float yy, float yz, float yw,
                    float zx, float zy, float zz, float zw,
                    float wx, float wy, float wz, float ww);//传入每一项设置
	//设置行
    void        setRow(int index, const float row[4]);
    void        setRow(int index, const Vector4& v);
    void        setRow(int index, const Vector3& v);
	//设置列
    void        setColumn(int index, const float col[4]);
    void        setColumn(int index, const Vector4& v);
    void        setColumn(int index, const Vector3& v);

	//获取
    const float* get() const;
    const float* getTranspose();                        // return transposed matrix
    float        getDeterminant();
	const char* toString(void) const;

    Matrix4&    identity();
    Matrix4&    transpose();                            // transpose itself and return reference
    Matrix4&    invert();                               // check best inverse method before inverse
    Matrix4&    invertEuclidean();                      // inverse of Euclidean transform matrix
    Matrix4&    invertAffine();                         // inverse of affine transform matrix
    Matrix4&    invertProjective();                     // inverse of projective matrix using partitioning
    Matrix4&    invertGeneral();                        // inverse of generic matrix

    // transform matrix
    Matrix4&    translate(float x, float y, float z);   // translation by (x,y,z)
    Matrix4&    translate(const Vector3& v);            //
    Matrix4&    postTranslate( float tx, float ty, float tz );

    Matrix4&    rotate(float angle, const Vector3& axis); // rotate angle(degree) along the given axix
    Matrix4&    rotate(float angle, float x, float y, float z);
    Matrix4&    rotateX(float angle);                   // rotate on X-axis with degree
    Matrix4&    rotateY(float angle);                   // rotate on Y-axis with degree
    Matrix4&    rotateZ(float angle);                   // rotate on Z-axis with degree
    Matrix4&    scale(float scale);                     // uniform scale
    Matrix4&    scale(float sx, float sy, float sz);    // scale by (sx, sy, sz) on each axis
	//keep Z no change ,used in 2D skew
	Matrix4&    skew2D(float skewXAngle,float skewYAngle);
	Matrix4&    skew2DX(float skewXAngle);
	Matrix4&    skew2DY(float skewYAngle);

	static Matrix4& perspective( float width, float height, float nearPlane, float farPlane );
	static Matrix4& orthographic( float width, float height, float nearPlane, float farPlane );
    static Matrix4& lookAt( const Vector3& vEye, const Vector3& vAt, const Vector3& vUp );

    // operators
    //rhs = righthand side 右侧
    Matrix4     operator+(const Matrix4& rhs) const;    // add rhs
    Matrix4     operator-(const Matrix4& rhs) const;    // subtract rhs
    Matrix4&    operator+=(const Matrix4& rhs);         // add rhs and update this object
    Matrix4&    operator-=(const Matrix4& rhs);         // subtract rhs and update this object
    Vector4     operator*(const Vector4& rhs) const;    // multiplication: v' = M * v
    Vector3     operator*(const Vector3& rhs) const;    // multiplication: v' = M * v
    Point     operator*(const Point& rhs) const;     // multiplication: p' = M * p
    Matrix4     operator*(const Matrix4& rhs) const;    // multiplication: M3 = M1 * M2
    Matrix4&    operator*=(const Matrix4& rhs);         // multiplication: M1' = M1 * M2
    bool        operator==(const Matrix4& rhs) const;   // exact compare, no epsilon
    bool        operator!=(const Matrix4& rhs) const;   // exact compare, no epsilon
    float       operator[](int index) const;            // subscript operator v[0], v[1]
    float&      operator[](int index);                  // subscript operator v[0], v[1]

    friend Matrix4 operator-(const Matrix4& m);                     // unary operator (-)
    friend Matrix4 operator*(float scalar, const Matrix4& m);       // pre-multiplication
    friend Vector3 operator*(const Vector3& vec, const Matrix4& m); // pre-multiplication
    friend Vector4 operator*(const Vector4& vec, const Matrix4& m); // pre-multiplication

private:
    float  getCofactor(float m0, float m1, float m2,
                       float m3, float m4, float m5,
                       float m6, float m7, float m8);

    float m[16];
    float tm[16];                                     // transpose m

};

///////////////////////////////////////////////////////////////////////////
// inline functions for Matrix2
///////////////////////////////////////////////////////////////////////////
inline Matrix2::Matrix2()
{
    // initially identity matrix
    identity();
}


inline Matrix2::Matrix2(const float src[4],enum MATRIX_MAJOR major=ROW_MAJOR)
{
    set(src,major);
}

inline Matrix2::Matrix2(float xx, float xy, float yx, float yy)
{
    set(xx, xy, yx, yy);
}

inline void Matrix2::set(const float src[4],enum MATRIX_MAJOR major)
{
    if (ROW_MAJOR == major)
    {
        m[0] = src[0];  m[1] = src[2];  m[2] = src[1];  m[3] = src[3];
    }
    else
    {
        m[0] = src[0];  m[1] = src[1];  m[2] = src[2];  m[3] = src[3];
    }
}


inline void Matrix2::set(float xx, float xy, float yx, float yy)
{
    m[0]= xx;  m[2] = xy;  m[1] = yx;  m[3]= yy;
}


inline void Matrix2::setRow(int index, const float row[2])
{
    m[index] = row[0];  m[index + 2] = row[1];
}


inline void Matrix2::setRow(int index, const Vector2& v)
{
    m[index] = v.x;  m[index + 2] = v.y;
}



inline void Matrix2::setColumn(int index, const float col[2])
{
    m[index*2] = col[0];  m[index*2 + 1] = col[1];
}



inline void Matrix2::setColumn(int index, const Vector2& v)
{
    m[index*2] = v.x;  m[index*2 + 1] = v.y;
}



inline const float* Matrix2::get() const
{
    return m;
}

inline Matrix2& Matrix2::transpose()
{
    FK_SWAP(m[1],  m[2] , float);
    return *this;
}

inline Matrix2& Matrix2::identity()
{
    m[0] = m[3] = 1.0f;
    m[1] = m[2] = 0.0f;
    return *this;
}



inline Matrix2 Matrix2::operator+(const Matrix2& rhs) const
{
    return Matrix2(m[0]+rhs[0],
                   m[1]+rhs[1],
                   m[2]+rhs[2],
                   m[3]+rhs[3]);
}



inline Matrix2 Matrix2::operator-(const Matrix2& rhs) const
{
    return Matrix2(m[0]-rhs[0],
                   m[1]-rhs[1],
                   m[2]-rhs[2],
                   m[3]-rhs[3]);
}



inline Matrix2& Matrix2::operator+=(const Matrix2& rhs)
{
    m[0] += rhs[0];
    m[1] += rhs[1];
    m[2] += rhs[2];
    m[3] += rhs[3];
    return *this;
}


inline Matrix2& Matrix2::operator-=(const Matrix2& rhs)
{
    m[0] -= rhs[0];
    m[1] -= rhs[1];
    m[2] -= rhs[2];
    m[3] -= rhs[3];
    return *this;
}


inline Vector2 Matrix2::operator*(const Vector2& rhs) const
{
    return Vector2(m[0]*rhs.x + m[2]*rhs.y,
                   m[1]*rhs.x + m[3]*rhs.y);
}

/**| 0 2 |   | 0 2 |
 * | 1 3 | x | 1 3 |
 */
inline Matrix2 Matrix2::operator*(const Matrix2& rhs) const
{
    return Matrix2(m[0]*rhs[0] + m[2]*rhs[1],
                   m[0]*rhs[2] + m[2]*rhs[3],
                   m[1]*rhs[0] + m[3]*rhs[1],
                   m[1]*rhs[2] + m[3]*rhs[3]);
}


inline Matrix2& Matrix2::operator*=(const Matrix2& rhs)
{
    *this = *this * rhs;
    return *this;
}



inline bool Matrix2::operator==(const Matrix2& rhs) const
{
    return (m[0] == rhs[0])
        && (m[1] == rhs[1])
        && (m[2] == rhs[2])
        && (m[3] == rhs[3]);
}



inline bool Matrix2::operator!=(const Matrix2& rhs) const
{
    return (m[0] != rhs[0]) || (m[1] != rhs[1]) || (m[2] != rhs[2]) || (m[3] != rhs[3]);
}



inline float Matrix2::operator[](int index) const
{
    return m[index];
}



inline float& Matrix2::operator[](int index)
{
    return m[index];
}



inline Matrix2 operator-(const Matrix2& rhs)
{
    return Matrix2(-rhs[0], -rhs[2], -rhs[1], -rhs[3]);
}



inline Matrix2 operator*(float s, const Matrix2& rhs)
{
    return Matrix2(s*rhs[0], s*rhs[1], s*rhs[2], s*rhs[3]);
}



inline Vector2 operator*(const Vector2& v, const Matrix2& rhs)
{
    return Vector2(v.x*rhs[0] + v.y*rhs[1],
                   v.x*rhs[2] + v.y*rhs[3]);
}

// END OF MATRIX2 INLINE //////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////
// inline functions for Matrix3
///////////////////////////////////////////////////////////////////////////
inline Matrix3::Matrix3()
{
    // initially identity matrix
    identity();
}



inline Matrix3::Matrix3(const float src[9],enum MATRIX_MAJOR major)
{
    set(src,major);
}



inline Matrix3::Matrix3(float xx, float xy, float xz,
                        float yx, float yy, float yz,
                        float zx, float zy, float zz)
{
    set(xx, xy, xz,  yx, yy, yz,  zx, zy, zz);
}

inline Matrix3::Matrix3(const Matrix2& matrix)
{
    m[0] = matrix[0]; m[1] = matrix[1];
    m[3] = matrix[2]; m[4] = matrix[3];
    m[2] = m[5] = m[6] = m[7] = 0;
    m[8] = 1;
}

inline void Matrix3::set(const float src[9],enum MATRIX_MAJOR major)
{
    if (ROW_MAJOR == major) {
        m[0] = src[0];  m[3] = src[1];  m[6] = src[2];
        m[1] = src[3];  m[4] = src[4];  m[7] = src[5];
        m[2] = src[6];  m[5] = src[7];  m[8] = src[8];
    }
    else
    {
        m[0] = src[0];  m[1] = src[1];  m[2] = src[2];
        m[3] = src[3];  m[4] = src[4];  m[5] = src[5];
        m[6] = src[6];  m[7] = src[7];  m[8] = src[8];
    }
}



inline void Matrix3::set(float xx, float xy, float xz,
                         float yx, float yy, float yz,
                         float zx, float zy, float zz)
{
    m[0] = xx;  m[3] = xy;  m[6] = xz;
    m[1] = yx;  m[4] = yy;  m[7] = yz;
    m[2] = zx;  m[5] = zy;  m[8] = zz;
}



inline void Matrix3::setRow(int index, const float row[3])
{
    m[index] = row[0];  m[index + 3] = row[1];  m[index + 6] = row[2];
}



inline void Matrix3::setRow(int index, const Vector3& v)
{
    m[index] = v.x;  m[index + 3] = v.y;  m[index + 6] = v.z;
}



inline void Matrix3::setColumn(int index, const float col[3])
{
    m[index*3] = col[0];  m[index*3 + 1] = col[1];  m[index*3 + 2] = col[2];
}



inline void Matrix3::setColumn(int index, const Vector3& v)
{
    m[index*3] = v.x;  m[index*3 + 1] = v.y;  m[index*3 + 2] = v.z;
}



inline const float* Matrix3::get() const
{
    return m;
}



inline Matrix3& Matrix3::identity()
{
    m[0] = m[4] = m[8] = 1.0f;
    m[1] = m[2] = m[3] = m[5] = m[6] = m[7] = 0.0f;
    return *this;
}



inline Matrix3 Matrix3::operator+(const Matrix3& rhs) const
{
    return Matrix3(m[0]+rhs[0], m[3]+rhs[3], m[6]+rhs[6],
                   m[1]+rhs[1], m[4]+rhs[4], m[7]+rhs[7],
                   m[2]+rhs[2], m[5]+rhs[5], m[8]+rhs[8]);
}



inline Matrix3 Matrix3::operator-(const Matrix3& rhs) const
{
    return Matrix3(m[0]-rhs[0], m[3]-rhs[3], m[6]-rhs[6],
                   m[1]-rhs[1], m[4]-rhs[4], m[7]-rhs[7],
                   m[2]-rhs[2], m[5]-rhs[5], m[8]-rhs[8]);
}



inline Matrix3& Matrix3::operator+=(const Matrix3& rhs)
{
    m[0] += rhs[0];  m[1] += rhs[1];  m[2] += rhs[2];
    m[3] += rhs[3];  m[4] += rhs[4];  m[5] += rhs[5];
    m[6] += rhs[6];  m[7] += rhs[7];  m[8] += rhs[8];
    return *this;
}



inline Matrix3& Matrix3::operator-=(const Matrix3& rhs)
{
    m[0] -= rhs[0];  m[1] -= rhs[1];  m[2] -= rhs[2];
    m[3] -= rhs[3];  m[4] -= rhs[4];  m[5] -= rhs[5];
    m[6] -= rhs[6];  m[7] -= rhs[7];  m[8] -= rhs[8];
    return *this;
}



inline Vector3 Matrix3::operator*(const Vector3& rhs) const
{
    return Vector3(m[0]*rhs.x + m[3]*rhs.y + m[6]*rhs.z,
                   m[1]*rhs.x + m[4]*rhs.y + m[7]*rhs.z,
                   m[2]*rhs.x + m[5]*rhs.y + m[8]*rhs.z);
}



inline Matrix3 Matrix3::operator*(const Matrix3& rhs) const
{
    return Matrix3(m[0]*rhs[0] + m[3]*rhs[1] + m[6]*rhs[2],  m[0]*rhs[3] + m[3]*rhs[4] + m[6]*rhs[5],  m[0]*rhs[6] + m[3]*rhs[7] + m[6]*rhs[8],
                   m[1]*rhs[0] + m[4]*rhs[1] + m[7]*rhs[2],  m[1]*rhs[3] + m[4]*rhs[4] + m[7]*rhs[5],  m[1]*rhs[6] + m[4]*rhs[7] + m[7]*rhs[8],
                   m[2]*rhs[0] + m[5]*rhs[1] + m[8]*rhs[2],  m[2]*rhs[3] + m[5]*rhs[4] + m[8]*rhs[5],  m[2]*rhs[6] + m[5]*rhs[7] + m[8]*rhs[8]);
}



inline Matrix3& Matrix3::operator*=(const Matrix3& rhs)
{
    *this = *this * rhs;
    return *this;
}



inline bool Matrix3::operator==(const Matrix3& rhs) const
{
    return (m[0] == rhs[0]) && (m[1] == rhs[1]) && (m[2] == rhs[2]) &&
           (m[3] == rhs[3]) && (m[4] == rhs[4]) && (m[5] == rhs[5]) &&
           (m[6] == rhs[6]) && (m[7] == rhs[7]) && (m[8] == rhs[8]);
}



inline bool Matrix3::operator!=(const Matrix3& rhs) const
{
    return (m[0] != rhs[0]) || (m[1] != rhs[1]) || (m[2] != rhs[2]) ||
           (m[3] != rhs[3]) || (m[4] != rhs[4]) || (m[5] != rhs[5]) ||
           (m[6] != rhs[6]) || (m[7] != rhs[7]) || (m[8] != rhs[8]);
}



inline float Matrix3::operator[](int index) const
{
    return m[index];
}



inline float& Matrix3::operator[](int index)
{
    return m[index];
}



inline Matrix3 operator-(const Matrix3& rhs)
{
    return Matrix3(-rhs[0], -rhs[3], -rhs[6], -rhs[1], -rhs[4], -rhs[7], -rhs[2], -rhs[5], -rhs[8]);
}



inline Matrix3 operator*(float s, const Matrix3& rhs)
{
    return Matrix3(s*rhs[0], s*rhs[3], s*rhs[6], s*rhs[1], s*rhs[4], s*rhs[7], s*rhs[2], s*rhs[5], s*rhs[8]);
}



inline Vector3 operator*(const Vector3& v, const Matrix3& m)
{
    return Vector3(v.x*m[0] + v.y*m[1] + v.z*m[2],
                   v.x*m[3] + v.y*m[4] + v.z*m[5],
                   v.x*m[6] + v.y*m[7] + v.z*m[8]);
}


// END OF MATRIX3 INLINE //////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////////
// inline functions for Matrix4
///////////////////////////////////////////////////////////////////////////
inline Matrix4::Matrix4()
{
    // initially identity matrix
    identity();
}



inline Matrix4::Matrix4(const float src[16],enum MATRIX_MAJOR major)
{
    set(src,major);
}



inline Matrix4::Matrix4(float xx, float xy, float xz, float xw,
                        float yx, float yy, float yz, float yw,
                        float zx, float zy, float zz, float zw,
                        float wx, float wy, float wz, float ww)
{
    set(xx, xy, xz, xw,  yx, yy, yz, yw,  zx, zy, zz, zw,  wx, wy, wz, ww);
}

inline Matrix4::Matrix4(const Matrix3& matrix)
{
	m[0] = matrix[0]; m[4] = matrix[4]; m[8] = matrix[8]; m[12] = matrix[12];
    m[1] = matrix[1]; m[5] = matrix[5]; m[9] = matrix[9]; m[13] = matrix[13];
	m[2] = matrix[2]; m[6] = matrix[6]; m[10] = matrix[10]; m[14] = matrix[14];
    m[3] = m[7] = m[11] = m[12] = m[13] = m[14] = 0;
    m[15] = 1;
}

inline void Matrix4::set(const float src[16],enum MATRIX_MAJOR major)
{
    if (ROW_MAJOR == major) {
        m[0] = src[0];  m[4] = src[1];  m[8] = src[2];  m[12] = src[3];
        m[1] = src[4];  m[5] = src[5];  m[9] = src[6];  m[13] = src[7];
        m[2] = src[8];  m[6] = src[9];  m[10]= src[10]; m[14]= src[11];
        m[3]= src[12]; m[7]= src[13]; m[11]= src[14]; m[15]= src[15];
    }
    else
    {
        m[0] = src[0];  m[1] = src[1];  m[2] = src[2];  m[3] = src[3];
        m[4] = src[4];  m[5] = src[5];  m[6] = src[6];  m[7] = src[7];
        m[8] = src[8];  m[9] = src[9];  m[10]= src[10]; m[11]= src[11];
        m[12]= src[12]; m[13]= src[13]; m[14]= src[14]; m[15]= src[15];
    }
}



inline void Matrix4::set(float xx, float xy, float xz, float xw,
                         float yx, float yy, float yz, float yw,
                         float zx, float zy, float zz, float zw,
                         float wx, float wy, float wz, float ww)
{
    m[0] = xx;  m[4] = xy;  m[8] = xz;  m[12] = xw;
    m[1] = yx;  m[5] = yy;  m[9] = yz;  m[13] = yw;
    m[2] = zx;  m[6] = zy;  m[10]= zz;  m[14]= zw;
    m[3]= wx;  m[7]= wy;  m[11]= wz;  m[15]= ww;
}



inline void Matrix4::setRow(int index, const float row[4])
{
    m[index] = row[0];  m[index + 4] = row[1];  m[index + 8] = row[2];  m[index + 12] = row[3];
}



inline void Matrix4::setRow(int index, const Vector4& v)
{
    m[index] = v.x;  m[index + 4] = v.y;  m[index + 8] = v.z;  m[index + 12] = v.w;
}



inline void Matrix4::setRow(int index, const Vector3& v)
{
    m[index] = v.x;  m[index + 4] = v.y;  m[index + 8] = v.z;
}



inline void Matrix4::setColumn(int index, const float col[4])
{
    
    m[index*4] = col[0];  m[index*4 + 1] = col[1];  m[index*4 + 2] = col[2];  m[index*4 + 3] = col[3];
}



inline void Matrix4::setColumn(int index, const Vector4& v)
{
    m[index*4] = v.x;  m[index*4 + 1] = v.y;  m[index*4 + 2] = v.z;  m[index*4 + 3] = v.w;
    
}



inline void Matrix4::setColumn(int index, const Vector3& v)
{
    
    m[index*4] = v.x;  m[index*4 + 1] = v.y;  m[index*4 + 2] = v.z;
}



inline const float* Matrix4::get() const
{
    return m;
}



inline const float* Matrix4::getTranspose()
{
    tm[0] = m[0];   tm[1] = m[4];   tm[2] = m[8];   tm[3] = m[12];
    tm[4] = m[1];   tm[5] = m[5];   tm[6] = m[9];   tm[7] = m[13];
    tm[8] = m[2];   tm[9] = m[6];   tm[10]= m[10];  tm[11]= m[14];
    tm[12]= m[3];   tm[13]= m[7];   tm[14]= m[11];  tm[15]= m[15];
    return tm;
}



inline Matrix4& Matrix4::identity()
{
    m[0] = m[5] = m[10] = m[15] = 1.0f;
    m[1] = m[2] = m[3] = m[4] = m[6] = m[7] = m[8] = m[9] = m[11] = m[12] = m[13] = m[14] = 0.0f;
    return *this;
}



inline Matrix4 Matrix4::operator+(const Matrix4& rhs) const
{
    return Matrix4(m[0]+rhs[0],   m[4]+rhs[4],   m[8]+rhs[8],   m[12]+rhs[12],
                   m[1]+rhs[1],   m[5]+rhs[5],   m[9]+rhs[9],   m[13]+rhs[13],
                   m[2]+rhs[2],   m[6]+rhs[6],   m[10]+rhs[10], m[14]+rhs[14],
                   m[3]+rhs[3], m[7]+rhs[7], m[11]+rhs[11], m[15]+rhs[15]);
}



inline Matrix4 Matrix4::operator-(const Matrix4& rhs) const
{
    return Matrix4(m[0]-rhs[0],   m[4]-rhs[4],   m[8]-rhs[8],   m[12]-rhs[12],
                   m[1]-rhs[4],   m[5]-rhs[5],   m[9]-rhs[9],   m[13]-rhs[13],
                   m[2]-rhs[8],   m[6]-rhs[6],   m[10]-rhs[10], m[14]-rhs[14],
                   m[3]-rhs[12], m[7]-rhs[7], m[11]-rhs[11], m[15]-rhs[15]);
}



inline Matrix4& Matrix4::operator+=(const Matrix4& rhs)
{
    m[0] += rhs[0];    m[1] += rhs[1];    m[2] += rhs[2];    m[3] += rhs[3];
    m[4] += rhs[4];    m[5] += rhs[5];    m[6] += rhs[6];    m[7] += rhs[7];
    m[8] += rhs[8];    m[9] += rhs[9];    m[10] += rhs[10];  m[11] += rhs[11];
    m[12] += rhs[12];  m[13] += rhs[13];  m[14] += rhs[14];  m[15] += rhs[15];
    return *this;
}



inline Matrix4& Matrix4::operator-=(const Matrix4& rhs)
{
    m[0] -= rhs[0];    m[1] -= rhs[1];    m[2] -= rhs[2];    m[3] -= rhs[3];
    m[4] -= rhs[4];    m[5] -= rhs[5];    m[6] -= rhs[6];    m[7] -= rhs[7];
    m[8] -= rhs[8];    m[9] -= rhs[9];    m[10] -= rhs[10];  m[11] -= rhs[11];
    m[12] -= rhs[12];  m[13] -= rhs[13];  m[14] -= rhs[14];  m[15] -= rhs[15];
    return *this;
}



inline Vector4 Matrix4::operator*(const Vector4& rhs) const
{
    return Vector4(m[0]*rhs.x  + m[4]*rhs.y  + m[8]*rhs.z  + m[12]*rhs.w,
                   m[1]*rhs.x  + m[5]*rhs.y  + m[9]*rhs.z  + m[13]*rhs.w,
                   m[2]*rhs.x  + m[6]*rhs.y  + m[10]*rhs.z + m[14]*rhs.w,
                   m[3]*rhs.x + m[7]*rhs.y + m[11]*rhs.z + m[15]*rhs.w);
}



inline Vector3 Matrix4::operator*(const Vector3& rhs) const
{
    return Vector3(m[0]*rhs.x + m[4]*rhs.y + m[8]*rhs.z,
                   m[1]*rhs.x + m[5]*rhs.y + m[9]*rhs.z,
                   m[2]*rhs.x + m[6]*rhs.y + m[10]*rhs.z);
}

//TODO maybe wrong
inline Point Matrix4::operator*(const Point& rhs) const
{
    return Point(m[0]*rhs.x + m[4]*rhs.y + m[8]*rhs.z,
                   m[1]*rhs.x + m[5]*rhs.y + m[9]*rhs.z);
}

inline Matrix4 Matrix4::operator*(const Matrix4& n) const
{
    return Matrix4(m[0]*n[0]  + m[4]*n[1]  + m[8]*n[2]  + m[12]*n[3],   m[0]*n[4]  + m[4]*n[5]  + m[8]*n[6]  + m[12]*n[7],   m[0]*n[8]  + m[4]*n[9]  + m[8]*n[10]  + m[12]*n[11],   m[0]*n[12]  + m[4]*n[13]  + m[8]*n[14]  + m[3]*n[15],
                   m[1]*n[0]  + m[5]*n[1]  + m[9]*n[2]  + m[13]*n[3],   m[1]*n[4]  + m[5]*n[5]  + m[9]*n[6]  + m[13]*n[7],   m[1]*n[8]  + m[5]*n[9]  + m[9]*n[10]  + m[13]*n[11],   m[1]*n[12]  + m[5]*n[13]  + m[9]*n[14]  + m[13]*n[15],
                   m[2]*n[0]  + m[6]*n[1]  + m[10]*n[2] + m[14]*n[3],  m[2]*n[4]  + m[6]*n[5]  + m[10]*n[6] + m[14]*n[7],  m[2]*n[8]  + m[6]*n[9]  + m[10]*n[10] + m[14]*n[11],  m[2]*n[12]  + m[6]*n[13]  + m[10]*n[14] + m[14]*n[15],
                   m[3]*n[0] + m[7]*n[1] + m[11]*n[2] + m[15]*n[3],  m[3]*n[4] + m[7]*n[5] + m[11]*n[6] + m[15]*n[7],  m[3]*n[8] + m[7]*n[9] + m[11]*n[10] + m[15]*n[11],  m[3]*n[12] + m[7]*n[13] + m[11]*n[14] + m[15]*n[15]);
}



inline Matrix4& Matrix4::operator*=(const Matrix4& rhs)
{
	*this = (*this) * rhs;
    return *this;
}



inline bool Matrix4::operator==(const Matrix4& n) const
{
    return (m[0] == n[0])   && (m[1] == n[1])   && (m[2] == n[2])   && (m[3] == n[3]) &&
           (m[4] == n[4])   && (m[5] == n[5])   && (m[6] == n[6])   && (m[7] == n[7]) &&
           (m[8] == n[8])   && (m[9] == n[9])   && (m[10] == n[10]) && (m[11] == n[11]) &&
           (m[12] == n[12]) && (m[13] == n[13]) && (m[14] == n[14]) && (m[15] == n[15]);
}



inline bool Matrix4::operator!=(const Matrix4& n) const
{
    return (m[0] != n[0])   || (m[1] != n[1])   || (m[2] != n[2])   || (m[3] != n[3]) ||
           (m[4] != n[4])   || (m[5] != n[5])   || (m[6] != n[6])   || (m[7] != n[7]) ||
           (m[8] != n[8])   || (m[9] != n[9])   || (m[10] != n[10]) || (m[11] != n[11]) ||
           (m[12] != n[12]) || (m[13] != n[13]) || (m[14] != n[14]) || (m[15] != n[15]);
}



inline float Matrix4::operator[](int index) const
{
    return m[index];
}



inline float& Matrix4::operator[](int index)
{
    return m[index];
}



inline Matrix4 operator-(const Matrix4& rhs)
{
    return Matrix4(-rhs[0], -rhs[4], -rhs[8], -rhs[12],
                   -rhs[1], -rhs[5], -rhs[9], -rhs[13],
                   -rhs[2], -rhs[6], -rhs[10], -rhs[14],
                   -rhs[3], -rhs[7], -rhs[11], -rhs[15]);
}



inline Matrix4 operator*(float s, const Matrix4& rhs)
{
    return Matrix4(s*rhs[0], s*rhs[4], s*rhs[8], s*rhs[12],
                   s*rhs[1], s*rhs[5], s*rhs[9], s*rhs[13],
                   s*rhs[2], s*rhs[6], s*rhs[10], s*rhs[14],
                   s*rhs[3], s*rhs[7], s*rhs[11], s*rhs[15]);
}



inline Vector4 operator*(const Vector4& v, const Matrix4& m)
{
    return Vector4(v.x*m[0] + v.y*m[1] + v.z*m[2] + v.w*m[3],
                   v.x*m[4] + v.y*m[5] + v.z*m[6] + v.w*m[7],
                   v.x*m[8] + v.y*m[9] + v.z*m[10] + v.w*m[11],
                   v.x*m[12] + v.y*m[13] + v.z*m[14] + v.w*m[15]);
}



inline Vector3 operator*(const Vector3& v, const Matrix4& m)
{
    return Vector3(v.x*m[0] + v.y*m[1] + v.z*m[2],
                   v.x*m[4] + v.y*m[5] + v.z*m[6],
                   v.x*m[8] + v.y*m[9] + v.z*m[10]);
}
		
/******************************************************************
 * Quaternion class
 *
 */
class Quaternion
{
private:
    float x, y, z, w;

public:
    friend class Vector3;
    friend class Vector4;
    friend class Matrix4;

    Quaternion()
    {
        x = 0.f;
        y = 0.f;
        z = 0.f;
        w = 1.f;
    }

    Quaternion( const float fX, const float fY, const float fZ, const float fW )
    {
        x = fX;
        y = fY;
        z = fZ;
        w = fW;
    }

    Quaternion( const Vector3 vec, const float fW )
    {
        x = vec.x;
        y = vec.y;
        z = vec.z;
        w = fW;
    }

    Quaternion( const float* p )
    {
        x = *p++;
        y = *p++;
        z = *p++;
        w = *p++;
    }

    Quaternion operator*( const Quaternion rhs )
    {
        Quaternion ret;
        ret.x = x * rhs.w + y * rhs.z - z * rhs.y + w * rhs.x;
        ret.y = -x * rhs.z + y * rhs.w + z * rhs.x + w * rhs.y;
        ret.z = x * rhs.y - y * rhs.x + z * rhs.w + w * rhs.z;
        ret.w = -x * rhs.x - y * rhs.y - z * rhs.z + w * rhs.w;
        return ret;
    }

    Quaternion& operator*=( const Quaternion rhs )
    {
        Quaternion ret;
        ret.x = x * rhs.w + y * rhs.z - z * rhs.y + w * rhs.x;
        ret.y = -x * rhs.z + y * rhs.w + z * rhs.x + w * rhs.y;
        ret.z = x * rhs.y - y * rhs.x + z * rhs.w + w * rhs.z;
        ret.w = -x * rhs.x - y * rhs.y - z * rhs.z + w * rhs.w;
        *this = ret;
        return *this;
    }

    Quaternion Conjugate()
    {
        x = -x;
        y = -y;
        z = -z;
        return *this;
    }

    //Non destuctive version
    Quaternion Conjugated()
    {
        Quaternion ret;
        ret.x = -x;
        ret.y = -y;
        ret.z = -z;
        ret.w = w;
        return ret;
    }

    void ToMatrix( Matrix4& mat )
    {
        float x2 = x * x * 2.0f;
        float y2 = y * y * 2.0f;
        float z2 = z * z * 2.0f;
        float xy = x * y * 2.0f;
        float yz = y * z * 2.0f;
        float zx = z * x * 2.0f;
        float xw = x * w * 2.0f;
        float yw = y * w * 2.0f;
        float zw = z * w * 2.0f;

        mat.m[0] = 1.0f - y2 - z2;
        mat.m[1] = xy + zw;
        mat.m[2] = zx - yw;
        mat.m[4] = xy - zw;
        mat.m[5] = 1.0f - z2 - x2;
        mat.m[6] = yz + xw;
        mat.m[8] = zx + yw;
        mat.m[9] = yz - xw;
        mat.m[10] = 1.0f - x2 - y2;

        mat.m[3] = mat.m[7] = mat.m[11] = mat.m[12] = mat.m[13] = mat.m[14] = 0.0f;
        mat.m[15] = 1.0f;
    }

    void ToMatrixPreserveTranslate( Matrix4& mat )
    {
        float x2 = x * x * 2.0f;
        float y2 = y * y * 2.0f;
        float z2 = z * z * 2.0f;
        float xy = x * y * 2.0f;
        float yz = y * z * 2.0f;
        float zx = z * x * 2.0f;
        float xw = x * w * 2.0f;
        float yw = y * w * 2.0f;
        float zw = z * w * 2.0f;

        mat.m[0] = 1.0f - y2 - z2;
        mat.m[1] = xy + zw;
        mat.m[2] = zx - yw;
        mat.m[4] = xy - zw;
        mat.m[5] = 1.0f - z2 - x2;
        mat.m[6] = yz + xw;
        mat.m[8] = zx + yw;
        mat.m[9] = yz - xw;
        mat.m[10] = 1.0f - x2 - y2;

        mat.m[3] = mat.m[7] = mat.m[11] = 0.0f;
        mat.m[15] = 1.0f;
    }

    static Quaternion RotationAxis( const Vector3 axis, const float angle )
    {
        Quaternion ret;
        float s = sinf( angle / 2 );
        ret.x = s * axis.x;
        ret.y = s * axis.y;
        ret.z = s * axis.z;
        ret.w = cosf( angle / 2 );
        return ret;
    }

    void Value( float& fX, float& fY, float& fZ, float& fW )
    {
        fX = x;
        fY = y;
        fZ = z;
        fW = w;
    }
};


FLAKOR_NS_END

#endif
