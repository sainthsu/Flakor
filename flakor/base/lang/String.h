#ifndef _FK_STRING_H_
#define _FK_STRING_H_

#if (FK_TARGET_PLATFORM == FK_PLATFORM_BLACKBERRY)
#include <string.h>
#endif

#include <stdarg.h>
#include <string>
#include <functional>
#include "base/lang/DataVisitor.h"

FLAKOR_NS_BEGIN

class String : public Object
{
public:
	String(void);
	String(const char* str);
    String(const std::string& str);
    String(const String& str);

    virtual ~String();
    
    /* override assignment operator
     * @lua NA
     */
    String& operator= (const String& other);

    /** init a string with format, it's similar with the c function 'sprintf' 
     * @lua NA
     */
    bool initWithFormat(const char* format, ...) FK_FORMAT_PRINTF(2, 3);

    /** convert to int value */
    int intValue() const;

    /** convert to unsigned int value */
    unsigned int uintValue() const;

    /** convert to float value */
    float floatValue() const;

    /** convert to double value */
    double doubleValue() const;

    /** convert to bool value */
    bool boolValue() const;

    /** get the C string */
    const char* getCString() const;

    /** get the length of string */
    unsigned int length() const;

    /** compare to a c string */
    int compare(const char *) const;
	
    //virtual Object* copyWithZone(Zone* pZone);
    virtual bool equal(const Object* pObject);

    /** create a string with std string, you can also pass a c string pointer because the default constructor of std::string can access a c string pointer. 
     *  @return A String pointer which is an autorelease object pointer,
     *          it means that you needn't do a release operation unless you retain it.
     */
    static String* create(const std::string& str);

    /** create a string with format, it's similar with the c function 'sprintf', the default buffer size is (1024*100) bytes,
     *  if you want to change it, you should modify the kMaxStringLen macro in CCString.cpp file.
     *  @return A String pointer which is an autorelease object pointer,
     *          it means that you needn't do a release operation unless you retain it.
     *  @lua NA
     */ 
    static String* createWithFormat(const char* format, ...) FK_FORMAT_PRINTF(1, 2);

    /** create a string with binary data 
     *  @return A String pointer which is an autorelease object pointer,
     *          it means that you needn't do a release operation unless you retain it.
     */
    static String* createWithData(const unsigned char* pData, unsigned long nLen);

    /** create a string with a file, 
     *  @return A CCString pointer which is an autorelease object pointer,
     *          it means that you needn't do a release operation unless you retain it.
     */
    static String* createWithContentsOfFile(const char* pszFileName);
    /**
     * @lua NA
     */
    virtual void acceptVisitor(DataVisitor &visitor);

private:

    /** only for internal use */
    bool initWithFormatAndValist(const char* format, va_list ap);

public:
    std::string m_sString;
};

struct StringCompare : public std::binary_function<String *, String *, bool> {
    public:
        bool operator() (String * a, String * b) const {
            return strcmp(a->getCString(), b->getCString()) < 0;
        }
};

#define StringMake(str) String::create(str)

FLAKOR_NS_END

#endif
