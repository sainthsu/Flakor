#include "base/lang/String.h"
#include <stdlib.h>
#include <stdio.h>

FLAKOR_NS_BEGIN

#define kMaxStringLen (1024*100)

String::String(void)
:m_sString("")
{}

String::String(const char * str)
    :m_sString(str)
{}

String::String(const std::string& str)
    :m_sString(str)
{}

String::String(const String& str)
    :m_sString(str.getCString())
{}

String::~String()
{ 
    m_sString.clear();
}

String& String::operator= (const String& other)
{
	m_sString = other.m_sString;
	return *this;
}

bool String::initWithFormatAndValist(const char* format, va_list ap)
{
	bool bRet = false;
    char* pBuf = (char*)malloc(kMaxStringLen);
    if (pBuf != NULL)
    {
        vsnprintf(pBuf, kMaxStringLen, format, ap);
        m_sString = pBuf;
        free(pBuf);
        bRet = true;
    }
    return bRet;
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
    return atoi(m_sString.c_str());
}

unsigned int String::uintValue() const
{
    if (length() == 0)
    {
        return 0;
    }
    return (unsigned int)atoi(m_sString.c_str());
}

float String::floatValue() const
{
    if (length() == 0)
    {
        return 0.0f;
    }
    return (float)atof(m_sString.c_str());
}

double String::doubleValue() const
{
    if (length() == 0)
    {
        return 0.0;
    }
    return atof(m_sString.c_str());
}

bool String::boolValue() const
{
    if (length() == 0)
    {
        return false;
    }

    if (0 == strcmp(m_sString.c_str(), "0") || 0 == strcmp(m_sString.c_str(), "false"))
    {
        return false;
    }
    return true;
}

const char* String::getCString() const
{
    return m_sString.c_str();
}

unsigned int String::length() const
{
    return m_sString.length();
}

int String::compare(const char * pStr) const
{
    return strcmp(getCString(), pStr);
}

/*
Object* String::copyWithZone(CCZone* pZone)
{
    CCAssert(pZone == NULL, "CCString should not be inherited.");
    CCString* pStr = new CCString(m_sString.c_str());
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

String* String::create(const std::string& str)
{
    String* pRet = new String(str);
    pRet->autorelease();
    return pRet;
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

String* String::createWithFormat(const char* format, ...)
{
    String* pRet = String::create("");
    va_list ap;
    va_start(ap, format);
    pRet->initWithFormatAndValist(format, ap);
    va_end(ap);

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

void String::acceptVisitor(DataVisitor &visitor)
{
    visitor.visit(this);
}

FLAKOR_NS_END
