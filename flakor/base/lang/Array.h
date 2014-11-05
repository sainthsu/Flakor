/****************************************************************************
Copyright (c) 2014 flakor.org

http://www.flakor.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#ifndef _FK_ARRAY_H_
#define _FK_ARRAY_H_

#include "base/lang/Object.h"
#include "base/lang/DataVisitor.h"
#include "base/lang/Zone.h"
#include "base/interface/IMatcher.h"
#include "math/CArray.h"

/**
 * @addtogroup data_structures
 * @{
 */

/** @def FKARRAY_FOREACH
A convenience macro to iterate over a Array using. It is faster than the "fast enumeration" interface.
@since v0.99.4
*/
#define FK_ARRAY_FOREACH(__array__, __object__)                                                                         \
    if ((__array__) && (__array__)->data->num > 0)                                                                     \
    for(Object** __arr__ = (__array__)->data->arr, **__end__ = (__array__)->data->arr + (__array__)->data->num-1;    \
    __arr__ <= __end__ && (((__object__) = *__arr__) != NULL/* || true*/);                                             \
    __arr__++)

#define FK_ARRAY_FOREACH_REVERSE(__array__, __object__)                                                                  \
    if ((__array__) && (__array__)->data->num > 0)                                                                      \
    for(Object** __arr__ = (__array__)->data->arr + (__array__)->data->num-1, **__end__ = (__array__)->data->arr;     \
    __arr__ >= __end__ && (((__object__) = *__arr__) != NULL/* || true*/);                                              \
    __arr__--)

#if defined(FLAKOR_DEBUG) && (FLAKOR_DEBUG > 0)
#define FK_ARRAY_VERIFY_TYPE(__array__, __type__)                                                                 \
    do {                                                                                                         \
        if ((__array__) && (__array__)->data->num > 0)                                                           \
            for(Object** __arr__ = (__array__)->data->arr,                                                     \
                **__end__ = (__array__)->data->arr + (__array__)->data->num-1; __arr__ <= __end__; __arr__++)    \
                FKAssert(dynamic_cast<__type__>(*__arr__), "element type is wrong!");                            \
    } while(false)
#else
#define FK_ARRAY_VERIFY_TYPE(__array__, __type__) void(0)
#endif

#define arrayMakeObjectsPerformSelector(pArray, func, elementType)    \
do {                                                                  \
    if(pArray && pArray->count() > 0)                                 \
    {                                                                 \
        Object* child;                                              \
        FK_ARRAY_FOREACH(pArray, child)                                \
        {                                                             \
            elementType pEntity = (elementType) child;                  \
            if(pEntity)                                                 \
            {                                                         \
                pEntity->func();                                        \
            }                                                         \
        }                                                             \
    }                                                                 \
}                                                                     \
while(false)

#define arrayMakeObjectsPerformSelectorWithObject(pArray, func, pObject, elementType)   \
do {                                                                  \
    if(pArray && pArray->count() > 0)                                 \
    {                                                                 \
        Object* child = NULL;                                       \
        FKARRAY_FOREACH(pArray, child)                                \
        {                                                             \
            elementType pEntity = (elementType) child;                  \
            if(pEntity)                                                 \
            {                                                         \
                pEntity->func(pObject);                                 \
            }                                                         \
        }                                                             \
    }                                                                 \
}                                                                     \
while(false)


FLAKOR_NS_BEGIN

//template <typename T>
class Array : public Object
{
public:
    /**
     * @lua NA
     */
    ~Array();

    /** Create an array */
    static Array* create();
    /** Create an array with some objects 
     *  @lua NA
     */
    static Array* create(Object* pObject, ...);
    /** Create an array with one object */
    static Array* createWithObject(Object* pObject);
    /** Create an array with capacity */
    static Array* createWithCapacity(unsigned int capacity);
    /** Create an array with an existing array */
    static Array* createWithArray(Array* otherArray);
    /**
     @brief   Generate a Array pointer by file
     @param   pFileName  The file name of *.plist file
     @return  The CCArray pointer generated from the file
     */
    static Array* createWithContentsOfFile(const char* pFileName);
    
    /*
     @brief The same meaning as arrayWithContentsOfFile(), but it doesn't call autorelease, so the
     invoker should call release().
     @lua NA
     */
    static Array* createWithContentsOfFileThreadSafe(const char* pFileName);

    /** Initializes an array 
     *  @lua NA
     */
    bool init();
    /** Initializes an array with one object
     *  @lua NA
     */
    bool initWithObject(Object* pObject);
    /** Initializes an array with some objects 
     *  @lua NA
     */
    bool initWithObjects(Object* pObject, ...);
    /** Initializes an array with capacity 
     *  @lua NA
     */
    bool initWithCapacity(unsigned int capacity);
    /** Initializes an array with an existing array 
     *  @lua NA
     */
    bool initWithArray(Array* otherArray);

    // Querying an Array

    /** Returns element count of the array */
    unsigned int count() const;
    /** Returns capacity of the array */
    unsigned int capacity() const;
    /** Returns index of a certain object, return UINT_MAX if doesn't contain the object */
    unsigned int indexOfObject(Object* object) const;
    /** Returns an element with a certain index */
    Object* objectAtIndex(unsigned int index);
	/** Returns first element */
	Object* firstObject();
    /** Returns last element */
    Object* lastObject();
    /** Returns a random element */
    Object* randomObject();
	Object* query(IMatcher<Object> matcher);
    /** Returns a Boolean value that indicates whether object is present in array. */
    bool containsObject(Object* object) const;
    /** @since 1.1 */
    bool isEqualToArray(Array* pOtherArray);
    // Adding Objects

    /** Add a certain object */
    void addObject(Object* object);
	void addFirst(Object* object);
    /** Add all elements of an existing array */
    void addObjectsFromArray(Array* otherArray);
    /** Insert a certain object at a certain index */
    void insertObject(Object* object, unsigned int index);

    // Removing Objects

    /** Remove last object */
    void removeLastObject(bool bReleaseObj = true);
    /** Remove a certain object */
    void removeObject(Object* object, bool bReleaseObj = true);
    /** Remove an element with a certain index */
    void removeObjectAtIndex(unsigned int index, bool bReleaseObj = true);
    /** Remove all elements */
    void removeObjectsInArray(Array* otherArray);
    /** Remove all objects */
    void removeAllObjects();
    /** Fast way to remove a certain object */
    void fastRemoveObject(Object* object);
    /** Fast way to remove an element with a certain index */
    void fastRemoveObjectAtIndex(unsigned int index);

    // Rearranging Content

    /** Swap two elements */
    void exchangeObject(Object* object1, Object* object2);
    /** Swap two elements with certain indexes */
    void exchangeObjectAtIndex(unsigned int index1, unsigned int index2);

    /** Replace object at index with another object. */
    void replaceObjectAtIndex(unsigned int uIndex, Object* pObject, bool bReleaseObject = true);

    /** Revers the array */
    void reverseObjects();
    /* Shrinks the array so the memory footprint corresponds with the number of items */
    void reduceMemoryFootprint();
  
    /** override functions 
     *  @js NA
     *  @lua NA
     */
    virtual Object* copyWithZone(Zone* pZone);

    /* override functions */
    virtual void acceptVisitor(DataVisitor &visitor);

public:
    fkArray* data;
    /**
     *  @lua NA
     */
    Array();
    /**
     *  @lua NA
     */
    Array(unsigned int capacity);
};

// end of data_structure group
/// @}

FLAKOR_NS_END

#endif // _FK_ARRAY_H_
