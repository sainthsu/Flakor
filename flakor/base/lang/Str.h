#ifndef _FK_STR_H_
#define _FK_STR_H_

#if (FK_TARGET_PLATFORM == FK_PLATFORM_BLACKBERRY)
#include <string.h>
#endif

#include <sys/types.h>
#include <stdarg.h>
#include <string>
#include <functional>
#include "base/lang/DataVisitor.h"

#define MAX_STRING_LEN (1024*1024)
#define STRING_MAX_PREALLOC (1024*1024)

FLAKOR_NS_BEGIN

class String : public Object
{
protected:
        char *_string;
        unsigned int _length;
        unsigned int _free;
public:
        String(void);
        String(const String& str);
        virtual ~String();

        static String* create(const char* str);
        static String* create(const char* str,size_t len);
        /** create a string with std string
         *  @return A String pointer which is an autorelease object pointer,
         *          it means that you needn't do a release operation unless you retain it.
         */
        static String* create(const std::string& str);

        /** create a string with format, it's similar with the c function 'sprintf', the default buffer size is (1024*100) bytes,
         *  if you want to change it, you should modify the kMaxStringLen macro in CCString.cpp file.
         *  `@return A String pointer which is an autorelease object pointer,
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
         *  @return A String pointer which is an autorelease object pointer,
         *          it means that you needn't do a release operation unless you retain it.
         */
        static String* createWithContentsOfFile(const char* pszFileName);

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

        /** get the available free */
        unsigned int avail() const;

        unsigned int findFirst(const String* c);
        unsigned int findFirst(const char* c ,unsigned int pos = 0);
        unsigned int findFirstNot(const String* c);
        unsigned int findFirstNot(const char* c ,unsigned int pos = 0);
        unsigned int findLast(const String* c);
        unsigned int findLast(const char* c ,unsigned int pos = 0);
        unsigned int findLastNot(const String* c);
        unsigned int findLastNot(const char* c ,unsigned int pos = 0);

        const char at(unsigned int pos);

    	/** compare to a c string */
    	int compare(const char *) const;
		/** compare to a String */
		int compare(const String *str) const;
	
    	//virtual Object* copyWithZone(Zone* pZone);
    	virtual bool equal(const Object* pObject);

        /* Modify a String on-place to make it empty (zero length).
         * However all the existing buffer is not discarded but set as free space
         * so that next append operations will not require allocations up to the
         * number of bytes previously available. */
        void clear();

        /* Remove the part of the string from left and from right composed just of
		 * contiguous characters found in 'cset', that is a null terminted C string.
		 *
	     * Example:
		 *
		 * String *s = String::create("AA...AA.a.aa.aHelloWorld     :::");
		 * s->trim("A. :");
		 * printf("%s\n", s->getCString());
		 *
		 * Output will be just "Hello World".
		 */
        void trim(const char *cset);

        //Apply tolower() to every character of _string
        void toLower();

        //Apply toupper() to every character of _string
        void toUpper();

        /* Return the total size of the allocation of the specifed String,
         * including:
         * 1) The String pointer.
         * 2) The string.
         * 3) The free buffer at the end if any.
         * 4) The implicit null term.
         */
        size_t getAllocSize();

        bool append(const String* str);
        bool append(const char* cat);

		/* Turn the string into a smaller (or equal) string containing only the
		 * substring specified by the 'start' and 'end' indexes.
		 *
	     * start and end can be negative, where -1 means the last character of the
		 * string, -2 the penultimate character, and so forth.
		 *
		 * The interval is inclusive, so the start and end characters will be part
		 * of the resulting string.
		 *
		 * The string is modified in-place.
		 *
		 * Example:
		 *
		 * String* s = String::create("Hello World");
		 * s->range(1,-1); => "ello World"
		 */
		void range(int start,int end);

		String** split(const char* sep,int *count);
    	/**
     	* @lua NA
     	*/
    	virtual void acceptVisitor(DataVisitor &visitor);

private:
		bool initLength(const char* str,size_t len);	
		
    	/** only for internal use */
    	bool initWithFormatAndValist(const char* format, va_list ap);

        /* Enlarge the free space at the end of the String so that the caller
         * is sure that after calling this function can overwrite up to addlen
         * bytes after the end of the string, plus one more byte for nul term.
         *
         * Note: this does not change the *length* of the String as returned
         * by length(), but only the free buffer space we have. */
        bool makeRoomFor(size_t addlen);

        /* Append the specified binary-safe string pointed by 't' of 'len' bytes to the
         * end of the specified sds string 's'.
         *
         * After the call, the passed sds string is no longer valid and all the
         * references must be substituted with the new pointer returned by the call. */
        bool catlen(const void *t, size_t len);
};

FLAKOR_NS_END

#endif

