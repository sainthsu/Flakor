#include "tool/utility/TexUtils.h"

    static PixelFormat TexUtils::convertDataToFormat(const unsigned char* data, ssize_t dataLen, PixelFormat originFormat, PixelFormat format, unsigned char** outData, ssize_t* outDataLen);

    static PixelFormat TexUtils::convertI8ToFormat(const unsigned char* data, ssize_t dataLen, PixelFormat format, unsigned char** outData, ssize_t* outDataLen);
    static PixelFormat TexUtils::convertAI88ToFormat(const unsigned char* data, ssize_t dataLen, PixelFormat format, unsigned char** outData, ssize_t* outDataLen);
    static PixelFormat TexUtils::convertRGB888ToFormat(const unsigned char* data, ssize_t dataLen, PixelFormat format, unsigned char** outData, ssize_t* outDataLen);
    static PixelFormat TexUtils::convertRGBA8888ToFormat(const unsigned char* data, ssize_t dataLen, PixelFormat format, unsigned char** outData, ssize_t* outDataLen);

Texture2D::PixelFormat Texture2D::convertI8ToFormat(const unsigned char* data, ssize_t dataLen, PixelFormat format, unsigned char** outData, ssize_t* outDataLen)
{
    switch (format)
    {
    case PixelFormat::RGBA8888:
        *outDataLen = dataLen*4;
        *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
        convertI8ToRGBA8888(data, dataLen, *outData);
        break;
    case PixelFormat::RGB888:
        *outDataLen = dataLen*3;
        *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
        convertI8ToRGB888(data, dataLen, *outData);
        break;
    case PixelFormat::RGB565:
        *outDataLen = dataLen*2;
        *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
        convertI8ToRGB565(data, dataLen, *outData);
        break;
    case PixelFormat::AI88:
        *outDataLen = dataLen*2;
        *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
        convertI8ToAI88(data, dataLen, *outData);
        break;
    case PixelFormat::RGBA4444:
        *outDataLen = dataLen*2;
        *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
        convertI8ToRGBA4444(data, dataLen, *outData);
        break;
    case PixelFormat::RGB5A1:
        *outDataLen = dataLen*2;
        *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
        convertI8ToRGB5A1(data, dataLen, *outData);
        break;
    default:
        // unsupport convertion or don't need to convert
        if (format != PixelFormat::AUTO && format != PixelFormat::I8)
        {
            CCLOG("Can not convert image format PixelFormat::I8 to format ID:%d, we will use it's origin format PixelFormat::I8", format);
        }

        *outData = (unsigned char*)data;
        *outDataLen = dataLen;
        return PixelFormat::I8;
    }

    return format;
}

Texture2D::PixelFormat Texture2D::convertAI88ToFormat(const unsigned char* data, ssize_t dataLen, PixelFormat format, unsigned char** outData, ssize_t* outDataLen)
{
    switch (format)
    {
    case PixelFormat::RGBA8888:
        *outDataLen = dataLen*2;
        *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
        convertAI88ToRGBA8888(data, dataLen, *outData);
        break;
    case PixelFormat::RGB888:
        *outDataLen = dataLen/2*3;
        *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
        convertAI88ToRGB888(data, dataLen, *outData);
        break;
    case PixelFormat::RGB565:
        *outDataLen = dataLen;
        *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
        convertAI88ToRGB565(data, dataLen, *outData);
        break;
    case PixelFormat::A8:
        *outDataLen = dataLen/2;
        *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
        convertAI88ToA8(data, dataLen, *outData);
        break;
    case PixelFormat::I8:
        *outDataLen = dataLen/2;
        *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
        convertAI88ToI8(data, dataLen, *outData);
        break;
    case PixelFormat::RGBA4444:
        *outDataLen = dataLen;
        *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
        convertAI88ToRGBA4444(data, dataLen, *outData);
        break;
    case PixelFormat::RGB5A1:
        *outDataLen = dataLen;
        *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
        convertAI88ToRGB5A1(data, dataLen, *outData);
        break;
    default:
        // unsupport convertion or don't need to convert
        if (format != PixelFormat::AUTO && format != PixelFormat::AI88)
        {
            CCLOG("Can not convert image format PixelFormat::AI88 to format ID:%d, we will use it's origin format PixelFormat::AI88", format);
        }

        *outData = (unsigned char*)data;
        *outDataLen = dataLen;
        return PixelFormat::AI88;
        break;
    }

    return format;
}

Texture2D::PixelFormat Texture2D::convertRGB888ToFormat(const unsigned char* data, ssize_t dataLen, PixelFormat format, unsigned char** outData, ssize_t* outDataLen)
{
    switch (format)
    {
    case PixelFormat::RGBA8888:
        *outDataLen = dataLen/3*4;
        *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
        convertRGB888ToRGBA8888(data, dataLen, *outData);
        break;
    case PixelFormat::RGB565:
        *outDataLen = dataLen/3*2;
        *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
        convertRGB888ToRGB565(data, dataLen, *outData);
        break;
    case PixelFormat::I8:
        *outDataLen = dataLen/3;
        *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
        convertRGB888ToI8(data, dataLen, *outData);
        break;
    case PixelFormat::AI88:
        *outDataLen = dataLen/3*2;
        *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
        convertRGB888ToAI88(data, dataLen, *outData);
        break;
    case PixelFormat::RGBA4444:
        *outDataLen = dataLen/3*2;
        *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
        convertRGB888ToRGBA4444(data, dataLen, *outData);
        break;
    case PixelFormat::RGB5A1:
        *outDataLen = dataLen;
        *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
        convertRGB888ToRGB5A1(data, dataLen, *outData);
        break;
    default:
        // unsupport convertion or don't need to convert
        if (format != PixelFormat::AUTO && format != PixelFormat::RGB888)
        {
            CCLOG("Can not convert image format PixelFormat::RGB888 to format ID:%d, we will use it's origin format PixelFormat::RGB888", format);
        }

        *outData = (unsigned char*)data;
        *outDataLen = dataLen;
        return PixelFormat::RGB888;
    }
    return format;
}

Texture2D::PixelFormat Texture2D::convertRGBA8888ToFormat(const unsigned char* data, ssize_t dataLen, PixelFormat format, unsigned char** outData, ssize_t* outDataLen)
{

    switch (format)
    {
    case PixelFormat::RGB888:
        *outDataLen = dataLen/4*3;
        *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
        convertRGBA8888ToRGB888(data, dataLen, *outData);
        break;
    case PixelFormat::RGB565:
        *outDataLen = dataLen/2;
        *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
        convertRGBA8888ToRGB565(data, dataLen, *outData);
        break;
    case PixelFormat::A8:
        *outDataLen = dataLen/4;
        *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
        convertRGBA8888ToA8(data, dataLen, *outData);
        break;
    case PixelFormat::I8:
        *outDataLen = dataLen/4;
        *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
        convertRGBA8888ToI8(data, dataLen, *outData);
        break;
    case PixelFormat::AI88:
        *outDataLen = dataLen/2;
        *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
        convertRGBA8888ToAI88(data, dataLen, *outData);
        break;
    case PixelFormat::RGBA4444:
        *outDataLen = dataLen/2;
        *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
        convertRGBA8888ToRGBA4444(data, dataLen, *outData);
        break;
    case PixelFormat::RGB5A1:
        *outDataLen = dataLen/2;
        *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
        convertRGBA8888ToRGB5A1(data, dataLen, *outData);
        break;
    default:
        // unsupport convertion or don't need to convert
        if (format != PixelFormat::AUTO && format != PixelFormat::RGBA8888)
        {
            CCLOG("Can not convert image format PixelFormat::RGBA8888 to format ID:%d, we will use it's origin format PixelFormat::RGBA8888", format);
        }

        *outData = (unsigned char*)data;
        *outDataLen = dataLen;
        return PixelFormat::RGBA8888;
    }

    return format;
}

/*
convert map:
1.PixelFormat::RGBA8888
2.PixelFormat::RGB888
3.PixelFormat::RGB565
4.PixelFormat::A8
5.PixelFormat::I8
6.PixelFormat::AI88
7.PixelFormat::RGBA4444
8.PixelFormat::RGB5A1

gray(5) -> 1235678
gray alpha(6) -> 12345678
rgb(2) -> 1235678
rgba(1) -> 12345678

*/
Texture2D::PixelFormat Texture2D::convertDataToFormat(const unsigned char* data, ssize_t dataLen, PixelFormat originFormat, PixelFormat format, unsigned char** outData, ssize_t* outDataLen)
{
    // don't need to convert
    if (format == originFormat || format == PixelFormat::AUTO)
    {
        *outData = (unsigned char*)data;
        *outDataLen = dataLen;
        return originFormat;
    }
    
    switch (originFormat)
    {
    case PixelFormat::I8:
        return convertI8ToFormat(data, dataLen, format, outData, outDataLen);
    case PixelFormat::AI88:
        return convertAI88ToFormat(data, dataLen, format, outData, outDataLen);
    case PixelFormat::RGB888:
        return convertRGB888ToFormat(data, dataLen, format, outData, outDataLen);
    case PixelFormat::RGBA8888:
        return convertRGBA8888ToFormat(data, dataLen, format, outData, outDataLen);
    default:
        CCLOG("unsupport convert for format %d to format %d", originFormat, format);
        *outData = (unsigned char*)data;
        *outDataLen = dataLen;
        return originFormat;
    }
}


//I8 to XXX

// IIIIIIII -> RRRRRRRRGGGGGGGGGBBBBBBBB
void TexUtils::convertI8ToRGB888(const unsigned char* data, ssize_t dataLen, unsigned char* outData)
{
	for (ssize_t i=0; i < dataLen; ++i)
    {
        *outData++ = data[i];     //R
        *outData++ = data[i];     //G
        *outData++ = data[i];     //B
    }
}

// IIIIIIII -> RRRRRRRRGGGGGGGGGBBBBBBBBAAAAAAAA
void TexUtils::convertI8ToRGBA8888(const unsigned char* data, ssize_t dataLen, unsigned char* outData)
{
	for (ssize_t i = 0; i < dataLen; ++i)
    {
        *outData++ = data[i];     //R
        *outData++ = data[i];     //G
        *outData++ = data[i];     //B
        *outData++ = 0xFF;        //A
    }
}

// IIIIIIII -> RRRRRGGGGGGBBBBB
void TexUtils::convertI8ToRGB565(const unsigned char* data, ssize_t dataLen, unsigned char* outData)
{
	unsigned short* out16 = (unsigned short*)outData;
    for (int i = 0; i < dataLen; ++i)
    {
        *out16++ = (data[i] & 0x00F8) << 8    //R
            | (data[i] & 0x00FC) << 3         //G
            | (data[i] & 0x00F8) >> 3;        //B
    }
}

// IIIIIIII -> RRRRGGGGBBBBAAAA
void TexUtils::convertI8ToRGBA4444(const unsigned char* data, ssize_t dataLen, unsigned char* outData)
{
	unsigned short* out16 = (unsigned short*)outData;
    for (ssize_t i = 0; i < dataLen; ++i)
    {
        *out16++ = (data[i] & 0x00F0) << 8    //R
        | (data[i] & 0x00F0) << 4             //G
        | (data[i] & 0x00F0)                  //B
        | 0x000F;                             //A
    }
}

// IIIIIIII -> RRRRRGGGGGBBBBBA
void TexUtils::convertI8ToRGB5A1(const unsigned char* data, ssize_t dataLen, unsigned char* outData)
{
	unsigned short* out16 = (unsigned short*)outData;
    for (int i = 0; i < dataLen; ++i)
    {
        *out16++ = (data[i] & 0x00F8) << 8    //R
            | (data[i] & 0x00F8) << 3         //G
            | (data[i] & 0x00F8) >> 2         //B
            | 0x0001;                         //A
    }
}

// IIIIIIII -> IIIIIIIIAAAAAAAA
void TexUtils::convertI8ToAI88(const unsigned char* data, ssize_t dataLen, unsigned char* outData)
{
	unsigned short* out16 = (unsigned short*)outData;
    for (ssize_t i = 0; i < dataLen; ++i)
    {
        *out16++ = 0xFF00     //A
        | data[i];            //I
    }
}

//AI88 to XXX

// IIIIIIIIAAAAAAAA -> RRRRRRRRGGGGGGGGBBBBBBBB
void TexUtils::convertAI88ToRGB888(const unsigned char* data, ssize_t dataLen, unsigned char* outData)
{
    for (ssize_t i = 0, l = dataLen - 1; i < l; i += 2)
    {
        *outData++ = data[i];     //R
        *outData++ = data[i];     //G
        *outData++ = data[i];     //B
    }
}

// IIIIIIIIAAAAAAAA -> RRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA
void TexUtils::convertAI88ToRGBA8888(const unsigned char* data, ssize_t dataLen, unsigned char* outData)
{
    for (ssize_t i = 0, l = dataLen - 1; i < l; i += 2)
    {
        *outData++ = data[i];     //R
        *outData++ = data[i];     //G
        *outData++ = data[i];     //B
        *outData++ = data[i + 1]; //A
    }
}


// IIIIIIIIAAAAAAAA -> RRRRRGGGGGGBBBBB
void TexUtils::convertAI88ToRGB565(const unsigned char* data, ssize_t dataLen, unsigned char* outData)
{
    unsigned short* out16 = (unsigned short*)outData;
    for (ssize_t i = 0, l = dataLen - 1; i < l; i += 2)
    {
        *out16++ = (data[i] & 0x00F8) << 8    //R
            | (data[i] & 0x00FC) << 3         //G
            | (data[i] & 0x00F8) >> 3;        //B
    }
}


// IIIIIIIIAAAAAAAA -> RRRRGGGGBBBBAAAA
void TexUtils::convertAI88ToRGBA4444(const unsigned char* data, ssize_t dataLen, unsigned char* outData)
{
    unsigned short* out16 = (unsigned short*)outData;
    for (ssize_t i = 0, l = dataLen - 1; i < l; i += 2)
    {
        *out16++ = (data[i] & 0x00F0) << 8    //R
        | (data[i] & 0x00F0) << 4             //G
        | (data[i] & 0x00F0)                  //B
        | (data[i+1] & 0x00F0) >> 4;          //A
    }
}

// IIIIIIIIAAAAAAAA -> RRRRRGGGGGBBBBBA
void TexUtils::convertAI88ToRGB5A1(const unsigned char* data, ssize_t dataLen, unsigned char* outData)
{
    unsigned short* out16 = (unsigned short*)outData;
    for (ssize_t i = 0, l = dataLen - 1; i < l; i += 2)
    {
        *out16++ = (data[i] & 0x00F8) << 8    //R
            | (data[i] & 0x00F8) << 3         //G
            | (data[i] & 0x00F8) >> 2         //B
            | (data[i + 1] & 0x0080) >> 7;    //A
    }
}

// IIIIIIIIAAAAAAAA -> AAAAAAAA
void TexUtils::convertAI88ToA8(const unsigned char* data, ssize_t dataLen, unsigned char* outData)
{
    for (ssize_t i = 1; i < dataLen; i += 2)
    {
        *outData++ = data[i]; //A
    }
}

// IIIIIIIIAAAAAAAA -> IIIIIIII
void TexUtils::convertAI88ToI8(const unsigned char* data, ssize_t dataLen, unsigned char* outData)
{
    for (ssize_t i = 0, l = dataLen - 1; i < l; i += 2)
    {
        *outData++ = data[i]; //R
    }
}


//RGB888 to XXX

// RRRRRRRRGGGGGGGGBBBBBBBB -> RRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA
void TexUtils::convertRGB888ToRGBA8888(const unsigned char* data, ssize_t dataLen, unsigned char* outData)
{
    for (ssize_t i = 0, l = dataLen - 2; i < l; i += 3)
    {
        *outData++ = data[i];         //R
        *outData++ = data[i + 1];     //G
        *outData++ = data[i + 2];     //B
        *outData++ = 0xFF;            //A
    }
}


// RRRRRRRRGGGGGGGGBBBBBBBB -> RRRRRGGGGGGBBBBB
void TexUtils::convertRGB888ToRGB565(const unsigned char* data, ssize_t dataLen, unsigned char* outData)
{
    unsigned short* out16 = (unsigned short*)outData;
    for (ssize_t i = 0, l = dataLen - 2; i < l; i += 3)
    {
        *out16++ = (data[i] & 0x00F8) << 8    //R
            | (data[i + 1] & 0x00FC) << 3     //G
            | (data[i + 2] & 0x00F8) >> 3;    //B
    }
}


// RRRRRRRRGGGGGGGGBBBBBBBB -> IIIIIIII
void TexUtils::convertRGB888ToI8(const unsigned char* data, ssize_t dataLen, unsigned char* outData)
{
    for (ssize_t i = 0, l = dataLen - 2; i < l; i += 3)
    {
        *outData++ = (data[i] * 299 + data[i + 1] * 587 + data[i + 2] * 114 + 500) / 1000;  //I =  (R*299 + G*587 + B*114 + 500) / 1000
    }
}

// RRRRRRRRGGGGGGGGBBBBBBBB -> IIIIIIIIAAAAAAAA
void TexUtils::convertRGB888ToAI88(const unsigned char* data, ssize_t dataLen, unsigned char* outData)
{
    for (ssize_t i = 0, l = dataLen - 2; i < l; i += 3)
    {
        *outData++ = (data[i] * 299 + data[i + 1] * 587 + data[i + 2] * 114 + 500) / 1000;  //I =  (R*299 + G*587 + B*114 + 500) / 1000
        *outData++ = 0xFF;
    }
}

// RRRRRRRRGGGGGGGGBBBBBBBB -> RRRRGGGGBBBBAAAA
void TexUtils::convertRGB888ToRGBA4444(const unsigned char* data, ssize_t dataLen, unsigned char* outData)
{
    unsigned short* out16 = (unsigned short*)outData;
    for (ssize_t i = 0, l = dataLen - 2; i < l; i += 3)
    {
        *out16++ = ((data[i] & 0x00F0) << 8           //R
                    | (data[i + 1] & 0x00F0) << 4     //G
                    | (data[i + 2] & 0xF0)            //B
                    |  0x0F);                         //A
    }
}


// RRRRRRRRGGGGGGGGBBBBBBBB -> RRRRRGGGGGBBBBBA
void TexUtils::convertRGB888ToRGB5A1(const unsigned char* data, ssize_t dataLen, unsigned char* outData)
{
    unsigned short* out16 = (unsigned short*)outData;
    for (ssize_t i = 0, l = dataLen - 2; i < l; i += 3)
    {
        *out16++ = (data[i] & 0x00F8) << 8    //R
            | (data[i + 1] & 0x00F8) << 3     //G
            | (data[i + 2] & 0x00F8) >> 2     //B
            |  0x01;                          //A
    }
}


//RGBA8888 to XXX

// RRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA -> RRRRRRRRGGGGGGGGBBBBBBBB
void TexUtils::convertRGBA8888ToRGB888(const unsigned char* data, ssize_t dataLen, unsigned char* outData)
{
    for (ssize_t i = 0, l = dataLen - 3; i < l; i += 4)
    {
        *outData++ = data[i];         //R
        *outData++ = data[i + 1];     //G
        *outData++ = data[i + 2];     //B
    }
}

// RRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA -> RRRRRGGGGGGBBBBB
void TexUtils::convertRGBA8888ToRGB565(const unsigned char* data, ssize_t dataLen, unsigned char* outData)
{
    unsigned short* out16 = (unsigned short*)outData;
    for (ssize_t i = 0, l = dataLen - 3; i < l; i += 4)
    {
        *out16++ = (data[i] & 0x00F8) << 8    //R
            | (data[i + 1] & 0x00FC) << 3     //G
            | (data[i + 2] & 0x00F8) >> 3;    //B
    }
}

// RRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA -> IIIIIIII
void TexUtils::convertRGBA8888ToI8(const unsigned char* data, ssize_t dataLen, unsigned char* outData)
{
    for (ssize_t i = 0, l = dataLen - 3; i < l; i += 4)
    {
        *outData++ = (data[i] * 299 + data[i + 1] * 587 + data[i + 2] * 114 + 500) / 1000;  //I =  (R*299 + G*587 + B*114 + 500) / 1000
    }
}

// RRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA -> AAAAAAAA
void TexUtils::convertRGBA8888ToA8(const unsigned char* data, ssize_t dataLen, unsigned char* outData)
{
    for (ssize_t i = 0, l = dataLen -3; i < l; i += 4)
    {
        *outData++ = data[i + 3]; //A
    }
}

// RRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA -> IIIIIIIIAAAAAAAA
void TexUtils::convertRGBA8888ToAI88(const unsigned char* data, ssize_t dataLen, unsigned char* outData)
{
    for (ssize_t i = 0, l = dataLen - 3; i < l; i += 4)
    {
        *outData++ = (data[i] * 299 + data[i + 1] * 587 + data[i + 2] * 114 + 500) / 1000;  //I =  (R*299 + G*587 + B*114 + 500) / 1000
        *outData++ = data[i + 3];
    }
}

// RRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA -> RRRRGGGGBBBBAAAA
void TexUtils::convertRGBA8888ToRGBA4444(const unsigned char* data, ssize_t dataLen, unsigned char* outData)
{
    unsigned short* out16 = (unsigned short*)outData;
    for (ssize_t i = 0, l = dataLen - 3; i < l; i += 4)
    {
        *out16++ = (data[i] & 0x00F0) << 8    //R
        | (data[i + 1] & 0x00F0) << 4         //G
        | (data[i + 2] & 0xF0)                //B
        |  (data[i + 3] & 0xF0) >> 4;         //A
    }
}

// RRRRRRRRGGGGGGGGBBBBBBBB -> RRRRRGGGGGBBBBBA
void TexUtils::convertRGBA8888ToRGB5A1(const unsigned char* data, ssize_t dataLen, unsigned char* outData)
{
    unsigned short* out16 = (unsigned short*)outData;
    for (ssize_t i = 0, l = dataLen - 2; i < l; i += 4)
    {
        *out16++ = (data[i] & 0x00F8) << 8    //R
            | (data[i + 1] & 0x00F8) << 3     //G
            | (data[i + 2] & 0x00F8) >> 2     //B
            |  (data[i + 3] & 0x0080) >> 7;   //A
    }
}

// conventer function end
//////////////////////////////////////////////////////////////////////////

