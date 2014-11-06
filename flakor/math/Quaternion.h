#ifndef _FK_QUATERNION_H_
#define _FK_QUATERNION_H_

FLAKOR_NS_BEGIN

class Matrix4;

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
