///////////////////////////////////////////////////////////////////////////////
// Matrice.cpp
// ===========
// NxN Matrix Math classes
// @url http://www.flakor.cn/2014-01-18-182.html
///////////////////////////////////////////////////////////////////////////////

#include <math.h>
#include "macros.h"

#include "Matrices.h"
#include "base/lang/Str.h"

const float DEG2RAD = 3.141593f / 180;

FLAKOR_NS_BEGIN

// return the determinant of 2x2 matrix
///////////////////////////////////////////////////////////////////////////////
float Matrix2::getDeterminant()
{
    return m[0] * m[3] - m[1] * m[2];
}

///////////////////////////////////////////////////////////////////////////////
// inverse of 2x2 matrix
// If cannot find inverse, set identity matrix
///////////////////////////////////////////////////////////////////////////////
Matrix2& Matrix2::invert()
{
    float determinant = m[0] * m[3] - m[1] * m[2];
    if(fabs(determinant) <= 0.00001f)
    {
        return identity();
    }

    float tmp = m[0];   // copy the first element
    float invDeterminant = 1.0f / determinant;
    m[0] =  invDeterminant * m[3];
    m[1] = -invDeterminant * m[1];
    m[2] = -invDeterminant * m[2];
    m[3] =  invDeterminant * tmp;

    return *this;
}

Matrix3& Matrix3::transpose()
{
    FK_SWAP(m[1],  m[3] , float);
    FK_SWAP(m[2],  m[6] , float);
    FK_SWAP(m[5],  m[7], float);

    return *this;
}


///////////////////////////////////////////////////////////////////////////////
// return determinant of 3x3 matrix
///////////////////////////////////////////////////////////////////////////////
float Matrix3::getDeterminant()
{
    return m[0] * (m[4] * m[8] - m[5] * m[7]) -
           m[1] * (m[3] * m[8] - m[5] * m[6]) +
           m[2] * (m[3] * m[7] - m[4] * m[6]);
}



///////////////////////////////////////////////////////////////////////////////
// inverse 3x3 matrix
// If cannot find inverse, set identity matrix
///////////////////////////////////////////////////////////////////////////////
Matrix3& Matrix3::invert()
{
    float determinant, invDeterminant;
    float tmp[9];

    tmp[0] = m[4] * m[8] - m[5] * m[7];
    tmp[1] = m[2] * m[7] - m[1] * m[8];
    tmp[2] = m[1] * m[5] - m[2] * m[4];
    tmp[3] = m[5] * m[6] - m[3] * m[8];
    tmp[4] = m[0] * m[8] - m[2] * m[6];
    tmp[5] = m[2] * m[3] - m[0] * m[5];
    tmp[6] = m[3] * m[7] - m[4] * m[6];
    tmp[7] = m[1] * m[6] - m[0] * m[7];
    tmp[8] = m[0] * m[4] - m[1] * m[3];

    // check determinant if it is 0
    determinant = m[0] * tmp[0] + m[1] * tmp[3] + m[2] * tmp[6];
    if(fabs(determinant) <= 0.00001f)
    {
        return identity(); // cannot inverse, make it idenety matrix
    }

    // divide by the determinant
    invDeterminant = 1.0f / determinant;
    m[0] = invDeterminant * tmp[0];
    m[1] = invDeterminant * tmp[1];
    m[2] = invDeterminant * tmp[2];
    m[3] = invDeterminant * tmp[3];
    m[4] = invDeterminant * tmp[4];
    m[5] = invDeterminant * tmp[5];
    m[6] = invDeterminant * tmp[6];
    m[7] = invDeterminant * tmp[7];
    m[8] = invDeterminant * tmp[8];

    return *this;
}

Matrix4 Matrix4::make(float xx, float xy, float xz, float xw,
            float yx, float yy, float yz, float yw,
            float zx, float zy, float zz, float zw,
            float wx, float wy, float wz, float ww)
{
	Matrix4* matrix = new Matrix4(xx, xy, xz,  xw,
             yx,  yy,  yz,  yw,
            zx,  zy,  zz,  zw,
             wx,  wy, wz, ww);
	return *matrix;
}

///////////////////////////////////////////////////////////////////////////////
// transpose 4x4 matrix
///////////////////////////////////////////////////////////////////////////////
Matrix4& Matrix4::transpose()
{
    FK_SWAP(m[1],  m[4] , float);
    FK_SWAP(m[2],  m[8] , float);
    FK_SWAP(m[3],  m[12], float);
    FK_SWAP(m[6],  m[9], float);
    FK_SWAP(m[7],  m[13], float);
    FK_SWAP(m[11], m[14], float);

    return *this;
}



///////////////////////////////////////////////////////////////////////////////
// inverse 4x4 matrix
///////////////////////////////////////////////////////////////////////////////
Matrix4& Matrix4::invert()
{
    // If the 4th row is [0,0,0,1] then it is affine matrix and
    // it has no projective transformation.
    if(m[3] == 0 && m[7] == 0 && m[11] == 0 && m[15] == 1)
        this->invertAffine();
    else
    {
        this->invertGeneral();
        /*@@ invertProjective() is not optimized (slower than generic one)
        if(fabs(m[0]*m[5] - m[1]*m[4]) > 0.00001f)
            this->invertProjective();   // inverse using matrix partition
        else
            this->invertGeneral();      // generalized inverse
        */
    }

    return *this;
}



///////////////////////////////////////////////////////////////////////////////
// compute the inverse of 4x4 Euclidean transformation matrix
//
// Euclidean transformation is translation, rotation, and reflection.
// With Euclidean transform, only the position and orientation of the object
// will be changed. Euclidean transform does not change the shape of an object
// (no scaling). Length and angle are reserved.
//
// Use inverseAffine() if the matrix has scale and shear transformation.
//
// M = [ R | T ]
//     [ --+-- ]    (R denotes 3x3 rotation/reflection matrix)
//     [ 0 | 1 ]    (T denotes 1x3 translation matrix)
//
// y = M*x  ->  y = R*x + T  ->  x = R^-1*(y - T)  ->  x = R^T*y - R^T*T
// (R is orthogonal,  R^-1 = R^T)
//
//  [ R | T ]-1    [ R^T | -R^T * T ]    (R denotes 3x3 rotation matrix)
//  [ --+-- ]   =  [ ----+--------- ]    (T denotes 1x3 translation)
//  [ 0 | 1 ]      [  0  |     1    ]    (R^T denotes R-transpose)
///////////////////////////////////////////////////////////////////////////////
Matrix4& Matrix4::invertEuclidean()
{
    // transpose 3x3 rotation matrix part
    // | R^T | 0 |
    // | ----+-- |
    // |  0  | 1 |
    float tmp;
    tmp = m[1];  m[1] = m[4];  m[4] = tmp;
    tmp = m[2];  m[2] = m[8];  m[8] = tmp;
    tmp = m[6];  m[6] = m[9];  m[9] = tmp;

    // compute translation part -R^T * T
    // | 0 | -R^T x |
    // | --+------- |
    // | 0 |   0    |
    float x = m[12];
    float y = m[13];
    float z = m[14];
    m[12]  = -(m[0] * x + m[4] * y + m[8] * z);
    m[13]  = -(m[1] * x + m[5] * y + m[9] * z);
    m[14] = -(m[2] * x + m[6] * y + m[10]* z);

    // last row should be unchanged (0,0,0,1)

    return *this;
}



///////////////////////////////////////////////////////////////////////////////
// compute the inverse of a 4x4 affine transformation matrix
//
// Affine transformations are generalizations of Euclidean transformations.
// Affine transformation includes translation, rotation, reflection, scaling,
// and shearing. Length and angle are NOT preserved.
// M = [ R | T ]
//     [ --+-- ]    (R denotes 3x3 rotation/scale/shear matrix)
//     [ 0 | 1 ]    (T denotes 1x3 translation matrix)
//
// y = M*x  ->  y = R*x + T  ->  x = R^-1*(y - T)  ->  x = R^-1*y - R^-1*T
//
//  [ R | T ]-1   [ R^-1 | -R^-1 * T ]
//  [ --+-- ]   = [ -----+---------- ]
//  [ 0 | 1 ]     [  0   +     1     ]
///////////////////////////////////////////////////////////////////////////////
Matrix4& Matrix4::invertAffine()
{
    // R^-1
    Matrix3 r(m[0],m[4],m[7],
              m[1],m[5],m[8],
              m[2],m[6],m[10]);
    r.invert();
    m[0] = r[0];  m[1] = r[1];  m[2] = r[2];
    m[4] = r[3];  m[5] = r[4];  m[6] = r[5];
    m[8] = r[6];  m[9] = r[7];  m[10]= r[8];

    // -R^-1 * T
    float x = m[12];
    float y = m[13];
    float z = m[14];
    m[12]  = -(r[0] * x + r[4] * y + r[8] * z);
    m[13]  = -(r[1] * x + r[5] * y + r[9] * z);
    m[14] = -(r[2] * x + r[6] * y + r[10] * z);

    // last row should be unchanged (0,0,0,1)
    //m[3] = m[7] = m[14] = 0.0f;
    //m[15] = 1.0f;

    return * this;
}



///////////////////////////////////////////////////////////////////////////////
// inverse matrix using matrix partitioning (blockwise inverse)
// It devides a 4x4 matrix into 4 of 2x2 matrices. It works in case of where
// det(A) != 0. If not, use the generic inverse method
// inverse formula.
// M = [ A | B ]    A, B, C, D are 2x2 matrix blocks
//     [ --+-- ]    det(M) = |A| * |D - ((C * A^-1) * B)|
//     [ C | D ]
//
// M^-1 = [ A' | B' ]   A' = A^-1 - (A^-1 * B) * C'
//        [ ---+--- ]   B' = (A^-1 * B) * -D'
//        [ C' | D' ]   C' = -D' * (C * A^-1)
//                      D' = (D - ((C * A^-1) * B))^-1
//
// NOTE: I wrap with () if it it used more than once.
//       The matrix is invertable even if det(A)=0, so must check det(A) before
//       calling this function, and use invertGeneric() instead.
///////////////////////////////////////////////////////////////////////////////
Matrix4& Matrix4::invertProjective()
{
    // partition
    Matrix2 a(m[0], m[4], m[1], m[5]);
    Matrix2 b(m[8], m[12], m[9], m[13]);
    Matrix2 c(m[2], m[6], m[3], m[7]);
    Matrix2 d(m[10], m[14], m[11], m[15]);

    // pre-compute repeated parts
    a.invert();             // A^-1
    Matrix2 ab = a * b;     // A^-1 * B
    Matrix2 ca = c * a;     // C * A^-1
    Matrix2 cab = ca * b;   // C * A^-1 * B
    Matrix2 dcab = d - cab; // D - C * A^-1 * B

    // check determinant if |D - C * A^-1 * B| = 0
    //NOTE: this function assumes det(A) is already checked. if |A|=0 then,
    //      cannot use this function.
    float determinant = dcab[0] * dcab[3] - dcab[1] * dcab[2];
    if(fabs(determinant) <= 0.00001f)
    {
        return identity();
    }

    // compute D' and -D'
    Matrix2 d1 = dcab;      //  (D - C * A^-1 * B)
    d1.invert();            //  (D - C * A^-1 * B)^-1
    Matrix2 d2 = -d1;       // -(D - C * A^-1 * B)^-1

    // compute C'
    Matrix2 c1 = d2 * ca;   // -D' * (C * A^-1)

    // compute B'
    Matrix2 b1 = ab * d2;   // (A^-1 * B) * -D'

    // compute A'
    Matrix2 a1 = a - (ab * c1); // A^-1 - (A^-1 * B) * C'

    // assemble inverse matrix
    m[0] = a1[0];  m[4] = a1[2];  m[8] = b1[0];  m[12] = b1[2];
    m[1] = a1[1];  m[5] = a1[3];  m[9] = b1[1];  m[13] = b1[3];
    m[2] = c1[0];  m[6] = c1[2];  m[10]= d1[0];  m[14]= d1[2];
    m[3] = c1[1];  m[7]= c1[3];  m[11]= d1[1];  m[15]= d1[3];

    return *this;
}



///////////////////////////////////////////////////////////////////////////////
// compute the inverse of a general 4x4 matrix using Cramer's Rule
// If cannot find inverse, return indentity matrix
// M^-1 = adj(M) / det(M)
///////////////////////////////////////////////////////////////////////////////
Matrix4& Matrix4::invertGeneral()
{
    // get cofactors of minor matrices
    float cofactor0 = getCofactor(m[5],m[9],m[13], m[6],m[10],m[14], m[7],m[11],m[15]);
    float cofactor4 = getCofactor(m[1],m[9],m[13], m[2],m[10],m[14], m[3],m[11],m[15]);
    float cofactor8 = getCofactor(m[1],m[5],m[13], m[2],m[6], m[14], m[3],m[7],m[15]);
    float cofactor12 = getCofactor(m[1],m[5],m[9], m[2],m[6], m[10], m[3],m[7],m[11]);

    // get determinant
    float determinant = m[0] * cofactor0 - m[4] * cofactor4 + m[8] * cofactor8 - m[12] * cofactor12;
    if(fabs(determinant) <= 0.00001f)
    {
        return identity();
    }

    // get rest of cofactors for adj(M)
    float cofactor1 = getCofactor(m[4],m[8],m[12], m[6],m[10],m[14], m[7],m[11],m[15]);
    float cofactor5 = getCofactor(m[0],m[8],m[12], m[2],m[10],m[14], m[3],m[11],m[15]);
    float cofactor9 = getCofactor(m[0],m[4],m[12], m[2],m[6], m[14], m[3],m[7],m[15]);
    float cofactor13 = getCofactor(m[0],m[4],m[8], m[2],m[6], m[10], m[3],m[7],m[11]);

    float cofactor2 = getCofactor(m[4],m[8],m[12], m[5],m[9], m[13],  m[7],m[11],m[15]);
    float cofactor6 = getCofactor(m[0],m[8],m[12], m[1],m[9], m[13],  m[3],m[11],m[15]);
    float cofactor10= getCofactor(m[0],m[4],m[12], m[1],m[5], m[13],  m[3],m[7],m[15]);
    float cofactor14= getCofactor(m[0],m[4],m[8], m[1],m[5], m[9],  m[3],m[7],m[11]);

    float cofactor3= getCofactor(m[4],m[8],m[12], m[5],m[9], m[13],  m[6], m[10],m[14]);
    float cofactor7= getCofactor(m[0],m[8],m[12], m[1],m[9], m[13],  m[2], m[10],m[14]);
    float cofactor11= getCofactor(m[0],m[4],m[12], m[1],m[5], m[13],  m[2], m[6], m[14]);
    float cofactor15= getCofactor(m[0],m[4],m[8], m[1],m[5], m[9],  m[2], m[6], m[10]);

    // build inverse matrix = adj(M) / det(M)
    // adjugate of M is the transpose of the cofactor matrix of M
    float invDeterminant = 1.0f / determinant;
    m[0] =  invDeterminant * cofactor0;
    m[1] = -invDeterminant * cofactor4;
    m[2] =  invDeterminant * cofactor8;
    m[3] = -invDeterminant * cofactor12;

    m[4] = -invDeterminant * cofactor1;
    m[5] =  invDeterminant * cofactor5;
    m[6] = -invDeterminant * cofactor9;
    m[7] =  invDeterminant * cofactor13;

    m[8] =  invDeterminant * cofactor2;
    m[9] = -invDeterminant * cofactor6;
    m[10]=  invDeterminant * cofactor10;
    m[11]= -invDeterminant * cofactor14;

    m[12]= -invDeterminant * cofactor3;
    m[13]=  invDeterminant * cofactor7;
    m[14]= -invDeterminant * cofactor11;
    m[15]=  invDeterminant * cofactor15;

    return *this;
}



///////////////////////////////////////////////////////////////////////////////
// return determinant of 4x4 matrix
///////////////////////////////////////////////////////////////////////////////
float Matrix4::getDeterminant()
{
    return m[0] * getCofactor(m[5],m[9],m[13], m[6],m[10],m[14], m[7],m[11],m[15]) -
           m[4] * getCofactor(m[1],m[9],m[13], m[2],m[10],m[14], m[3],m[11],m[15]) +
           m[8] * getCofactor(m[1],m[5],m[13], m[2],m[6], m[14], m[3],m[7],m[15]) -
           m[12] * getCofactor(m[1],m[5],m[9], m[2],m[6], m[10], m[3],m[7],m[11]);
}



///////////////////////////////////////////////////////////////////////////////
// compute cofactor of 3x3 minor matrix without sign
// input params are 9 elements of the minor matrix
// NOTE: The caller must know its sign.
///////////////////////////////////////////////////////////////////////////////
float Matrix4::getCofactor(float m0, float m3, float m6,
                           float m1, float m4, float m7,
                           float m2, float m5, float m8)
{
    return m0 * (m4 * m8 - m5 * m7) -
           m3 * (m1 * m8 - m7 * m2) +
           m6 * (m1 * m5 - m4 * m2);
}



///////////////////////////////////////////////////////////////////////////////
// translate this matrix by (x, y, z)
///////////////////////////////////////////////////////////////////////////////
Matrix4& Matrix4::translate(const Vector3& v)
{
    return translate(v.x, v.y, v.z);
}
/**
 *|1 0 0 x|   |0 4 8 12|
 *|0 1 0 y|   |1 5 9 13|
 *|0 0 1 z| x |2 6 10 14|
 *|0 0 0 1|   |3 7 11 15|
 */
Matrix4& Matrix4::translate(float x, float y, float z)
{
    m[0] += m[3]*x;   m[3] += m[7]*x;   m[2] += m[11]*x;   m[3] += m[15]*x;
    m[1] += m[3]*y;   m[4] += m[7]*y;   m[6] += m[11]*y;   m[7] += m[15]*y;
    m[2] += m[3]*z;   m[5] += m[7]*z;   m[10]+= m[11]*z;   m[11]+= m[15]*z;
    return *this;
}

Matrix4& Matrix4::postTranslate( float tx, float ty, float tz )
{
    m[12] += (tx * m[0]) + (ty * m[4]) + (tz * m[8]);
    m[13] += (tx * m[1]) + (ty * m[5]) + (tz * m[9]);
    m[14] += (tx * m[2]) + (ty * m[6]) + (tz * m[10]);
    m[15] += (tx * m[3]) + (ty * m[7]) + (tz * m[11]);
    
    return *this;
}


///////////////////////////////////////////////////////////////////////////////
// uniform scale
///////////////////////////////////////////////////////////////////////////////
Matrix4& Matrix4::scale(float s)
{
    return scale(s, s, s);
}

/**
 *|x 0 0 0|   |0 4 8 12|
 *|0 y 0 0|   |1 5 9 13|
 *|0 0 z 0| x |2 6 10 14|
 *|0 0 0 1|   |3 7 11 15|
 */
Matrix4& Matrix4::scale(float x, float y, float z)
{
    m[0] = m[0]*x;   m[4] = m[4]*x;   m[8] = m[8]*x;   m[12] = m[12]*x;
    m[1] = m[1]*y;   m[5] = m[5]*y;   m[9] = m[9]*y;   m[13] = m[13]*y;
    m[2] = m[2]*z;   m[6] = m[6]*z;   m[10]= m[10]*z;  m[14] = m[14]*z;
    return *this;
}



///////////////////////////////////////////////////////////////////////////////
// build a rotation matrix with given angle(degree) and rotation axis, then
// multiply it with this object
///////////////////////////////////////////////////////////////////////////////
Matrix4& Matrix4::rotate(float angle, const Vector3& axis)
{
    return rotate(angle, axis.x, axis.y, axis.z);
}


Matrix4& Matrix4::rotate(float angle, float x, float y, float z)
{
    float c = cosf(angle * DEG2RAD);    // cosine
    float s = sinf(angle * DEG2RAD);    // sine
    float xx = x * x;
    float xy = x * y;
    float xz = x * z;
    float yy = y * y;
    float yz = y * z;
    float zz = z * z;

    // build rotation matrix
    Matrix4 m;
    m[0] = xx * (1 - c) + c;
    m[4] = xy * (1 - c) - z * s;
    m[8] = xz * (1 - c) + y * s;
    m[12] = 0;
    m[1] = xy * (1 - c) + z * s;
    m[5] = yy * (1 - c) + c;
    m[9] = yz * (1 - c) - x * s;
    m[13] = 0;
    m[2] = xz * (1 - c) - y * s;
    m[6] = yz * (1 - c) + x * s;
    m[10]= zz * (1 - c) + c;
    m[14]= 0;
    m[3]= 0;
    m[7]= 0;
    m[11]= 0;
    m[15]= 1;

    // multiply it
    *this = m * (*this);

    return *this;
}

/**
 *|1  0    0  0|   |0 4 8 12|
 *|0 cos  -sin 0|   |1 5 9 13|
 *|0 sin cos 0| x |2 6 10 14|
 *|0  0    0  1|   |3 7 11 15|
 */
Matrix4& Matrix4::rotateX(float angle)
{
    float c = cosf(angle * DEG2RAD);
    float s = sinf(angle * DEG2RAD);
    float m1 = m[1], m5 = m[5], m9 = m[9],  m13 = m[13],
          m2 = m[2], m6 = m[6], m10= m[10], m14= m[14];

    m[1] = m1 * c + m2 *-s;
    m[5] = m5 * c + m6 *-s;
    m[9] = m9 * c + m10*-s;
    m[13] = m13 * c + m14*-s;
    m[2] = m1 * s + m2 * c;
    m[5] = m5 * s + m6 * c;
    m[10]= m9 * s + m10* c;
    m[14]= m13 * s + m14* c;

    return *this;
}

/**
 *|cos 0 sin 0|   |0 4 8 12|
 *| 0  1   0  0|   |1 5 9 13|
 *|－sin 0  cos 0| x |2 6 10 14|
 *| 0  0   0  1|   |3 7 11 15|
 */
Matrix4& Matrix4::rotateY(float angle)
{
    float c = cosf(angle * DEG2RAD);
    float s = sinf(angle * DEG2RAD);
    float m0 = m[0], m4 = m[4], m8 = m[8],  m12 = m[12],
          m2 = m[2], m6 = m[6], m10= m[10], m14= m[14];

    m[0] = m0 * c + m2 * s;
    m[1] = m4 * c + m6 * s;
    m[2] = m8 * c + m10* s;
    m[3] = m12 * c + m14* s;
    m[8] = m0 *-s + m2 * c;
    m[9] = m4 *-s + m6 * c;
    m[10]= m8 *-s + m10* c;
    m[11]= m12 *-s + m14* c;

    return *this;
}

/**
 *|cos -sin 0 0|   |0 4 8 12|
 *|sin  cos 0 0|   |1 5 9 13|
 *| 0    0  1 0| x |2 6 10 14|
 *| 0    0  0 1|   |3 7 11 15|
 */
Matrix4& Matrix4::rotateZ(float angle)
{
    float c = cosf(angle * DEG2RAD);
    float s = sinf(angle * DEG2RAD);
    float m0 = m[0], m4 = m[4], m8 = m[8],  m12 = m[12],
          m1 = m[1], m5 = m[5], m9 = m[9],  m13 = m[13];

    m[0] = m0 * c + m1 *-s;
    m[1] = m4 * c + m5 *-s;
    m[2] = m8 * c + m9 *-s;
    m[3] = m12 * c + m13 *-s;
    m[4] = m0 * s + m1 * c;
    m[5] = m4 * s + m5 * c;
    m[6] = m8 * s + m9 * c;
    m[7] = m12 * s + m13 * c;

    return *this;
}

/**
 * |1 b 0 0 |
 * |d 1 0 0 |
 * |0 0 1 0 |
 * |0 0 0 1 |
 * skewX is a skew angle of X
 * 
 * d = tan(skewX)
 * b = tan(skewy)
 */
Matrix4& Matrix4::skew2D(float skewXAngle,float skewYAngle)
{
	float tanD =  tanf(skewXAngle * DEG2RAD);
	float tanB =  tanf(skewXAngle * DEG2RAD);

	float m0 = m[0];
	float m4 = m[4];
	float m8 = m[8];
	float m12 = m[12];

	m[0] += tanB*m[1];
	m[4] += tanB*m[5];
	m[8] += tanB*m[9];
	m[12] += tanB*m[13];

	m[1] += tanD*m0;
	m[5] += tanD*m4;
	m[9] += tanD*m8;
	m[13] += tanD*m12;

	return *this;
}

/**
 * |1 0 0 0 |
 * |d 1 0 0 |
 * |0 0 1 0 |
 * |0 0 0 1 |
 * d = tan(skewX)
 */
Matrix4& Matrix4::skew2DX(float skewXAngle)
{
	float tan =  tanf(skewXAngle * DEG2RAD);
	
	m[1] += tan*m[0];
	m[5] += tan*m[4];
	m[9] += tan*m[8];
	m[13] += tan*m[12];

	return *this;
}

/**
 * |1 b 0 0 |
 * |0 1 0 0 |
 * |0 0 1 0 |
 * |0 0 0 1 |
 * b = tan(skewy)
 */
Matrix4& Matrix4::skew2DY(float skewYAngle)
{
	float tan = tanf(skewYAngle * DEG2RAD);
	
	m[0] += tan*m[1];
	m[4] += tan*m[5];
	m[8] += tan*m[9];
	m[12] += tan*m[13];

	return *this;
}

Matrix4& Matrix4::perspective( float width, float height, float nearPlane, float farPlane )
{
	float n2 = 2.0f * nearPlane;
    float rcpnmf = 1.f / (nearPlane - farPlane);

    Matrix4* result = new Matrix4();
    (*result)[0] = n2 / width;
    (*result)[4] = 0;
    (*result)[8] = 0;
    (*result)[12] = 0;
    (*result)[1] = 0;
    (*result)[5] = n2 / height;
    (*result)[9] = 0;
    (*result)[13] = 0;
    (*result)[2] = 0;
    (*result)[6] = 0;
    (*result)[10] = (farPlane + nearPlane) * rcpnmf;
    (*result)[14] = farPlane * rcpnmf * n2;
    (*result)[3] = 0;
    (*result)[7] = 0;
    (*result)[11] = -1.0;
    (*result)[15] = 0;

    return *result;
}

Matrix4& Matrix4::orthographic( float width, float height, float nearPlane, float farPlane )
{
  Matrix4* result = new Matrix4();

  (*result)[0] = 1/ width;
  (*result)[4] = 0;
  (*result)[8] = 0;
  (*result)[12] = 0;

  (*result)[1] = 0;
  (*result)[5] = 1/ height;
  (*result)[9] = 0;
  (*result)[13] = 0;

  (*result)[2] = 0;
  (*result)[6] = 0;
  (*result)[10] = -2/(farPlane - nearPlane);
  (*result)[14] = (farPlane+nearPlane)/(nearPlane-farPlane);

  (*result)[3] = 0;
  (*result)[7] = 0;
  (*result)[11] = 0;
  (*result)[15] = 1;

  return *result;
}


Matrix4& Matrix4::lookAt( const Vector3& vec_eye, const Vector3& vec_at, const Vector3& vec_up )
{
	Vector3 vec_forward, vec_up_norm, vec_side;
    Matrix4* result = new Matrix4();

    vec_forward.x = vec_eye.x - vec_at.x;
    vec_forward.y = vec_eye.y - vec_at.y;
    vec_forward.z = vec_eye.z - vec_at.z;

    vec_forward.normalize();
    vec_up_norm = vec_up;
    vec_up_norm.normalize();
    vec_side = vec_up_norm.cross( vec_forward );
    vec_up_norm = vec_forward.cross( vec_side );

    (*result)[0] = vec_side.x;
    (*result)[4] = vec_side.y;
    (*result)[8] = vec_side.z;
    (*result)[12] = 0;
    (*result)[1] = vec_up_norm.x;
    (*result)[5] = vec_up_norm.y;
    (*result)[9] = vec_up_norm.z;
    (*result)[13] = 0;
    (*result)[2] = vec_forward.x;
    (*result)[6] = vec_forward.y;
    (*result)[10] = vec_forward.z;
    (*result)[14] = 0;
    (*result)[3] = 0;
    (*result)[7] = 0;
    (*result)[11] = 0;
    (*result)[15] = 1.0;

    result->postTranslate( -vec_eye.x, -vec_eye.y, -vec_eye.z );
    return *result;
}

const char* Matrix4::toString(void) const
{
    return String::createWithFormat("<Matrix4 | v = (%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f)>",
                                    m[0],m[1],m[2],m[3],m[4],m[5],m[6],m[7],m[8],m[9],m[10],m[11],m[12],m[13],m[14],m[15])->getCString();
}

FLAKOR_NS_END
