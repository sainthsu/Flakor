#include "base/lang/Str.h"
#include <stdlib.h>
#include <stdio.h>

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

FLAKOR_NS_BEGIN

String::String(void)
:_string(NULL),
_length(0),
_free(0)
{}

String::String(const String& str)
{
    _length = str._length;
    _free = str._free;
    _string = (char*)malloc(_length+_free+1);
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

String* String::create(const char * str)
{
	int len = str == NULL? 0 : strlen(str);
        return String::create(str,len);
}


String* String::create(const char * str,size_t len)
{
    String* string = new String();
	if(string && string->initLength(str,len))
	{
    	return string;
	}
	return NULL;
}

String* String::create(const std::string& str)
{
    int len = str.length();
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
		//不是自赋值
		if(this != &other)
		{
			if(_string != NULL) free(_string);
        	_string = other._string;
        	_length = other._length;
        	_free = other._free;
		}
		return *this;
}

bool String::initLength(const char* str,size_t len)
{
	if(str)
    {
            _string = (char*)malloc(len+1);
    }
    else
    {
            _string = (char*)calloc(1,len+1);
    }

    if(_string == NULL) return false;

    _length = len;

    if(len && str)
    {
            memcpy(_string,str,len);
    }
    _string[_length] = '\0';
	
	return true;
}

bool String::initWithFormatAndValist(const char* format, va_list ap)
{
    bool ret = false;
    char* buf = (char*)malloc(MAX_STRING_LEN);
    if (buf != NULL)
    {
        vsnprintf(buf, MAX_STRING_LEN, format, ap);

        int len = strlen(buf);
        _string = (char*)malloc(len+1);

        if(_string != NULL)
        {
            memcpy(_string,buf,len);
            _string[len] = '\0';
            _length = len;
            _free = 0;
            ret = true;
        }

        free(buf);
    }
    return ret;
}

bool String::initWithFormat(const char* format, ...)
{
    bool bRet = false;
    if(_string != NULL) free(_string);

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

unsigned int String::avail() const
{
	return _free;
}

int String::compare(const char * pStr) const
{
    return strcmp(_string, pStr);
}

int String::compare(const String *str) const
{
	return compare(str->getCString());
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
    bool ret = false;
    const String* str = dynamic_cast<const String*>(pObject);
    if (str != NULL)
    {
        if (0 == this->compare(str))
        {
            ret = true;
        }
    }
    return ret;
}

void String::acceptVisitor(DataVisitor &visitor)
{
    visitor.visit(this);
}

void String::clear()
{
	_free += _length;
	_string[0] = '\0';
	_length = 0;
}

void String::trim(const char* cset)
{
    char *start,*end,*sp,*ep;
	size_t len;

	start = sp = _string;
	end = ep = _string+_length-1;
	while(sp <= end && strchr(cset,*sp)) sp++;
	while(ep >= start && strchr(cset,*ep)) ep--;
	len = (sp > ep)? 0 : ((ep-sp)+1);
	if(_string != sp) memmove(_string,sp,len);

	_string[len] = '\0';
	_length = len;
	_free = _free + (_length - len);

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

size_t String::getAllocSize()
{
	return sizeof(String)+_length+_free+1;
}

bool String::append(const String* str)
{
	if(str == NULL || str->length() <= 0) return false;
	return catlen(str->getCString(),str->length());
}

bool String::append(const char* cat)
{
	int len = strlen(cat);
	if(len <= 0) return false;
	return catlen(cat,len);
}

void String::range(int start,int end)
{
	size_t newLen;
	if(_length == 0) return;
	if(start < 0)
	{
		start = _length + start;
		if(start < 0) start = 0;
	}
	if(end < 0)
	{
		end = _length + end;
		if(end < 0) end = 0;
	}

	newLen = (start > end)? 0 : (end-start)+1;
	if (newLen != 0) 
	{
		if (start >= (signed)_length)
		{
			newLen = 0;
		}
		else if (end >= (signed)_length)
		{
			end = _length-1;
			newLen = (start > end) ? 0 : (end-start)+1;
		}
	}
	else
	{
		start = 0;
	}
	if (start && newLen) memmove(_string, _string+start, newLen);
	_string[newLen] = '\0';
	_free = _free+(_length-newLen);
	_length = newLen;

}

String** String::split(const char* sep,int *count)
{
	int len = strlen(sep);
	//position array contain start end pairs
	String **strArray;
	if(_length <= 0 || len < 1) return NULL;

	int elements = 0,slots = 5,start = 0,j;
	int search = _length - len + 1;

	strArray = (String **)malloc(sizeof(String *)*slots);
	if(strArray == NULL)
	{
		*count = 0;
		return NULL;
	}

	for(j=0;j < search;j++)
	{
		if( slots < elements+2)
		{
			String** newArray;
			slots *= 2;
			
			newArray = (String **)realloc(strArray,sizeof(String *)*slots);
			if(newArray == NULL) goto cleanup;
			strArray = newArray;
		}

		if((len == 1 && *(_string+j) == sep[0]) || (memcmp(_string+j,sep,len) == 0))
		{
			strArray[elements] = String::create(_string+j,j-start);
			if(strArray[elements] == NULL) goto cleanup;
			
			elements++;
			start = j+len;
			j = start-1;
		}
	}
	
	/* Add the final element. We are sure there is room in the tokens array. */
    strArray[elements] = String::create(_string+start,_length-start);
    if (strArray[elements] == NULL) goto cleanup;
    elements++;
    *count = elements;
    return strArray;
cleanup:
	{
        int i;
        for (i = 0; i < elements; i++) free(strArray[i]);
        free(strArray);
        *count = 0;
        return NULL;
    }
}


bool String::makeRoomFor(size_t addlen)
{
	size_t newLen;
    if(_free > addlen) return true;
	newLen = _length + addlen;
	if(newLen < STRING_MAX_PREALLOC)
		newLen *= 2;
	else
		newLen += STRING_MAX_PREALLOC;
	
	_string = (char*)realloc(_string,newLen);
	if(_string == NULL) return false;
	_free = newLen - _length;
	return true;
}

bool String::catlen(const void* t, size_t len)
{
	size_t curlen = _length;

    if (!this->makeRoomFor(len)) return false;

    memcpy(_string+curlen, t, len);
    _length = curlen+len;
    _free = _free-len;
    _string[curlen+len] = '\0';
	
	return true;
}

FLAKOR_NS_END
