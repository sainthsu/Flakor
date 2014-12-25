#include "base/lang/Str.h"
#include <stdlib.h>
#include <stdio.h>

FLAKOR_NS_BEGIN

namespace {

    /* Helper function for sdssplitargs() that returns non zero if 'c'
     * is a valid hex digit. */
    int is_hex_digit(char c) {
        return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') ||
               (c >= 'A' && c <= 'F');
    }

    /* Helper function for sdssplitargs() that converts a hex digit into an
     * integer from 0 to 15 */
    int hex_digit_to_int(char c) {
        switch(c) {
        case '0': return 0;
        case '1': return 1;
        case '2': return 2;
        case '3': return 3;
        case '4': return 4;
        case '5': return 5;
        case '6': return 6;
        case '7': return 7;
        case '8': return 8;
        case '9': return 9;
        case 'a': case 'A': return 10;
        case 'b': case 'B': return 11;
        case 'c': case 'C': return 12;
        case 'd': case 'D': return 13;
        case 'e': case 'E': return 14;
        case 'f': case 'F': return 15;
        default: return 0;
        }
    }
}

String::String(void)
:_string(NULL),
_length(0),
_free(0)
{}

String::String(const String& str)
{
    _length = str._length;
    _free = str._free;
    _string = malloc(_length+_free+1);
    if(_string != NULL)
    {
        memcpy(_string,str._string,_length);
        _string[_length] = '\0';
    }
    else
    {
        _length = _free = 0;
    }
}

String::~String()
{
    if(_string != NULL)
        free(_string);
    _free = _length = 0;
}

static String* String::create(const char * str)
{
	int len = str == NULL? 0 : strlen(str);
        return String::create(str,len);
}


static String* String::create(const char * str,size_t len)
{
    String* string = new String();
    if(str)
    {
            string->_string = malloc(len+1);
    }
    else
    {
            string->_string = calloc(1,len+1);
    }

    if(string->_string == NULL) return NULL;

    string->_length = len;

    if(len && str)
    {
            memcpy(string->_string,str,len);
    }
    string->_string[_length] = '\0';
    string->autorelease();

    return string;
}

static String* String::create(const std::string& str)
{
    int len = str == NULL? 0 : str.length();
    return String::create(str.c_str(),len);
}

String* String::createWithFormat(const char* format, ...)
{
    String* str = new String();
    va_list ap;
    va_start(ap, format);
    str->initWithFormatAndValist(format, ap);
    va_end(ap);

    return str;
}

String* String::createWithData(const unsigned char* pData, unsigned long nLen)
{
    String* pRet = NULL;
    if (pData != NULL)
    {
        char* pStr = (char*)malloc(nLen+1);
        if (pStr != NULL)
        {
            pStr[nLen] = '\0';
            if (nLen > 0)
            {
                memcpy(pStr, pData, nLen);
            }

            pRet = String::create(pStr);
            free(pStr);
        }
    }
    return pRet;
}

String* String::createWithContentsOfFile(const char* pszFileName)
{
    unsigned long size = 0;
    unsigned char* pData = 0;
    String* pRet = NULL;
    pData = NULL;//FileUtils::sharedFileUtils()->getFileData(pszFileName, "rb", &size);
    pRet = String::createWithData(pData, size);
    FK_SAFE_DELETE_ARRAY(pData);
    return pRet;
}

String& String::operator= (const String& other)
{
        _string = other._string;
        _length = other._length;
        _free = other._free;
	return *this;
}

bool String::initWithFormatAndValist(const char* format, va_list ap)
{
    bool ret = false;
    char* buf = (char*)malloc(MAX_STRING_LEN);
    if (buf != NULL)
    {
        vsnprintf(buf, MAX_STRING_LEN, format, ap);

        int len = strlen(buf);
        _string = malloc(len+1);

        if(_string != NULL)
        {
            memcpy(_string,buf,len);
            _string[len] = '\0';
            _length = len;
            _free = 0;
            ret = true;
        }

        free(pBuf);
    }
    return ret;
}

bool String::initWithFormat(const char* format, ...)
{
    bool bRet = false;
    m_sString.clear();

    va_list ap;
    va_start(ap, format);

    bRet = initWithFormatAndValist(format, ap);

    va_end(ap);

    return bRet;
}

int String::intValue() const
{
    if (length() == 0)
    {
        return 0;
    }
    return atoi(_string);
}

unsigned int String::uintValue() const
{
    if (length() == 0)
    {
        return 0;
    }
    return (unsigned int)atoi(_string);
}

float String::floatValue() const
{
    if (length() == 0)
    {
        return 0.0f;
    }
    return (float)atof(_string);
}

double String::doubleValue() const
{
    if (length() == 0)
    {
        return 0.0;
    }
    return atof(_string);
}

bool String::boolValue() const
{
    if (length() == 0)
    {
        return false;
    }

    if (0 == strcmp(_string, "0") || 0 == strcmp(_string, "false"))
    {
        return false;
    }
    return true;
}

const char* String::getCString() const
{
    return _string;
}

unsigned int String::length() const
{
    return _length;
}

int String::compare(const char * pStr) const
{
    return strcmp(_string, pStr);
}

/*
Object* String::copyWithZone(Zone* pZone)
{
    Assert(pZone == NULL, "String should not be inherited.");
    String* pStr = new String(m_sString.c_str());
    return pStr;
}
*/

bool String::equal(const Object* pObject)
{
    bool bRet = false;
    const String* pStr = dynamic_cast<const String*>(pObject);
    if (pStr != NULL)
    {
        if (0 == m_sString.compare(pStr->m_sString))
        {
            bRet = true;
        }
    }
    return bRet;
}

void String::acceptVisitor(DataVisitor &visitor)
{
    visitor.visit(this);
}

void String::trim(char* cset=" \t\v")
{
    char* start,end;

}

void String::toLower()
{
    int len = _length, j;

    for (j = 0; j < len; j++) _string[j] = tolower(_string[j]);
}

void String::toUpper()
{
    int len = _length, j;
    for (j = 0; j < len; j++) _string[j] = toupper(_string[j]);
}

void String::makeRoomFor(size_t addlen)
{
    if(_free > addlen) return;

}

FLAKOR_NS_END
