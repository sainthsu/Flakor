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

#ifndef _FK_DICTIONARY_H_
#define _FK_DICTIONARY_H_

#include "support/data_support/uthash.h"
#include "Object.h"
#include "Array.h"
#include "String.h"

FLAKOR_CC_BEGIN

class Dictionary;

/**
 * @addtogroup data_structures
 * @{
 */


/**
 *  DictItem is used for traversing Dictionary.
 *
 *  A DictItem is one element of Dictionary, it contains two properties, key and object.
 *  Its key has two different type (integer and string).
 *
 *  @note The key type is unique, all the elements in Dictionary has the same key type(integer or string).
 *  @code
 *  DictItem* pItem;
 *  FKDICT_FOREACH(dict, pElement)
 *  {
 *      const char*key = pElement->getStrKey();
 *      // You certainly know the type of value, so we assume that it's a CCSprite.
 *      Sprite* pSprite = (Sprite*)pElement->getObject();
 *      // ......
 *  }
 *  @endcode
 *  @js NA
 *  @lua NA
 */
class DictItem
{
private:
    /**
     *  Constructor of DictItem. It's only for internal usage. Dictionary is its friend class.
     *  
     *  @param  pszKey    The string key of this element.
     *  @param  pObject   The object of this element.
     */
    DictItem(const char* pszKey, CCObject* pObject);

    /**
     *  Constructor of DictItem. It's only for internal usage. CCDictionary is its friend class.
     *
     *  @param  iKey    The integer key of this element.
     *  @param  pObject   The object of this element.
     */
    DictItem(intptr_t iKey, CCObject* pObject);
    
public:
    /**
     *  The destructor of DictItem.
     */
    ~DictItem();

    // Inline functions need to be implemented in header file on Android.
    
    /**
     * Get the string key of this element.
     * @note    This method assumes you know the key type in the element. 
     *          If the element's key type is integer, invoking this method will cause an assert.
     *
     * @return  The string key of this element.
     */
    inline const char* getStrKey() const
    {
        FKAssert(m_szKey[0] != '\0', "Should not call this function for integer dictionary");
        return m_szKey;
    }

    /**
     * Get the integer key of this element.
     * @note    This method assumes you know the key type in the element.
     *          If the element's key type is string, invoking this method will cause an assert.
     *
     * @return  The integer key of this element.
     */
    inline intptr_t getIntKey() const
    {
        FKAssert(m_szKey[0] == '\0', "Should not call this function for string dictionary");
        return m_iKey;
    }
    
    /**
     * Get the object of this element.
     *
     * @return  The object of this element.
     */
    inline Object* getObject() const { return m_pObject; }

private:
    // The max length of string key.
    #define   MAX_KEY_LEN   256
    // char array is needed for HASH_ADD_STR in UT_HASH.
    // So it's a pain that all elements will allocate 256 bytes for this array.
    char      m_szKey[MAX_KEY_LEN];     // hash key of string type 存储名称的字符数组。
    intptr_t  m_iKey;       // hash key of integer type 哈希表索引
    CCObject* m_pObject;    // hash value 哈希值（Object指针）
public:
    UT_hash_handle hh;      // makes this class hashable 哈希表结构指针 
    friend class Dictionary; // declare Dictionary as friend class
};

/** The macro for traversing dictionary
 *  遍历词典中的所有词汇的一个宏，它内部调用HASH_ITER来进行for循环遍历链表。 
 *  @note It's faster than getting all keys and traversing keys to get objects by objectForKey.
 *        It's also safe to remove elements while traversing.
 */
#define FKDICT_FOREACH(__dict__, __el__) \
    DictItem* pTmp##__dict__##__el__ = NULL; \
    if (__dict__) \
    HASH_ITER(hh, (__dict__)->m_pElements, __el__, pTmp##__dict__##__el__)



/**
 *  Dictionary is a class like NSDictionary in Obj-C .
 *
 *  @note Only the pointer of Object or its subclass can be inserted to Dictionary.
 *  @code
 *  // Create a dictionary, return an autorelease object.
 *  Dictionary* pDict = Dictionary::create();
 *
 *  // Insert objects to dictionary
 *  String* pValue1 = String::create("100");
 *  String* pValue2 = String::create("120");
 *  Integer* pValue3 = Integer::create(200);
 *  pDict->setObject(pValue1, "key1");
 *  pDict->setObject(pValue2, "key2");
 *  pDict->setObject(pValue3, "key3");
 *
 *  // Get the object for key
 *  String* pStr1 = (String*)pDict->objectForKey("key1");
 *  FKLog("{ key1: %s }", pStr1->getCString());
 *  Integer* pInteger = (CCInteger*)pDict->objectForKey("key3");
 *  FKLog("{ key3: %d }", pInteger->getValue());
 *  @endcode
 *  @js NA
 *
 */

class Dictionary : public Object
{
public:
    /** 
     * The constructor of Dictionary.
     * @lua NA
     */
    Dictionary();

    /**
     * The destructor of Dictionary
     * @lua NA
     */
    ~Dictionary();

    /**
     *  Get the count of elements in Dictionary.
     *  取得所有词汇的数量。
     *  @return  The count of elements.
     */
    unsigned int count();

    /**
     *  Return all keys of elements.
     *  返回所有的查询关键字。
     *  @return  The array contains all keys of elements. It's an autorelease object yet.
     */
    Array* allKeys();

    /** 
     *  Get all keys according to the specified object.
	 *  取得对应CCObject指针的所有关键字或索引值。
     *  @warning  We use '==' to compare two objects
     *  @return   The array contains all keys for the specified object. It's an autorelease object yet.
     */
    Array* allKeysForObject(CCObject* object);

    /**
     *  Get the object according to the specified string key.
     *  通过查询关键字取得对应CCObject指针
     *  @note The dictionary needs to use string as key. If integer is passed, an assert will appear.
     *  @param key  The string key for searching.
     *  @return The object matches the key. You need to force convert it to the type you know.
     *  @code
     *     // Assume that the elements are String* pointers. Convert it by following code.
     *     String* pStr = (Ctring*)pDict->objectForKey("key1");
     *     // Do something about pStr.
     *     // If you don't know the object type, properly you need to use dynamic_cast<SomeType*> to check it.
     *     String* pStr2 = dynamic_cast<CCString*>(pDict->objectForKey("key1"));
     *     if (pStr2 != NULL) {
     *          // Do something about pStr2
     *     }
     *  @endcode
     *  @see objectForKey(intptr_t)
     */
    Object* objectForKey(const std::string& key);
    
    /**
     *  Get the object according to the specified integer key.
     *  通过哈希索引值取得对应CCObject指针
     *  @note The dictionary needs to use integer as key. If string is passed, an assert will appear.
     *  @param key  The integer key for searching.
     *  @return The object matches the key.
     *  @see objectForKey(const std::string&)
     */
    Object* objectForKey(intptr_t key);
    
    /** Get the value according to the specified string key.
     *
     *  @note Be careful to use this function since it assumes the objects in the dictionary are CCString pointer.
     *  @param key  The string key for searching
     *  @return An instance of String.
     *          It will return an empty string if the objects aren't CCString pointer or the key wasn't found.
     *  @see valueForKey(intptr_t)
     */
    const String* valueForKey(const std::string& key);
    
    /** Get the value according to the specified integer key.
     *
     *  @note Be careful to use this function since it assumes the objects in the dictionary are CCString pointer.
     *  @param key  The string key for searching.
     *  @return An instance of CCString.
     *          It will return an empty string if the objects aren't CCString pointer or the key wasn't found.
     *  @see valueForKey(intptr_t)
     */
    const String* valueForKey(intptr_t key);

    /** Insert an object to dictionary, and match it with the specified string key.
     *
     *  @note Whe the first time this method is invoked, the key type will be set to string.
     *        After that you can't setObject with an integer key.
     *        If the dictionary contains the key you passed, the object matching the key will be released and removed from dictionary.
     *        Then the new object will be inserted after that.
     *
     *  @param pObject  The Object to be inserted.
     *  @param key      The string key for searching.
     *  @see setObject(Object*, intptr_t)
     */
    void setObject(Object* pObject, const std::string& key);
    
    /** Insert an object to dictionary, and match it with the specified string key.
     *
     *  @note Then the first time this method is invoked, the key type will be set to string.
     *        After that you can't setObject with an integer key.
     *        If the dictionary contains the key you passed, the object matching the key will be released and removed from dictionary.
     *        Then the new object will be inserted after that.
     *  @param pObject  The Object to be inserted.
     *  @param key      The string key for searching.
     *  @see setObject(Object*, const std::string&)
     */
    void setObject(Object* pObject, intptr_t key);

    /** 
     *  Remove an object by the specified string key.
     *
     *  @param key  The string key for searching.
     *  @see removeObjectForKey(intptr_t), removeObjectsForKeys(CCArray*),
     *       removeObjectForElememt(CCDictElement*), removeAllObjects().
     */
    void removeObjectForKey(const std::string& key);
    
    /**
     *  Remove an object by the specified integer key.
     *
     *  @param key  The integer key for searching.
     *  @see removeObjectForKey(const std::string&), removeObjectsForKeys(CCArray*),
     *       removeObjectForElememt(CCDictElement*), removeAllObjects().
     */
    void removeObjectForKey(intptr_t key);
    
    /**
     *  Remove objects by an array of keys.
     *
     *  @param pKeyArray  The array contains keys to be removed.
     *  @see removeObjectForKey(const std::string&), removeObjectForKey(intptr_t),
     *       removeObjectForElememt(CCDictElement*), removeAllObjects().
     */
    void removeObjectsForKeys(Array* pKeyArray);
    
    /**
     *  Remove an object by an element.
     *
     *  @param pElement  The element need to be removed.
     *  @see removeObjectForKey(const std::string&), removeObjectForKey(intptr_t),
     *       removeObjectsForKeys(Array*), removeAllObjects().
     *  @lua NA
     */
    void removeObjectForElememt(DictItem* pElement);
    
    /**
     *  Remove all objects in the dictionary.
     *
     *  @see removeObjectForKey(const std::string&), removeObjectForKey(intptr_t),
     *       removeObjectsForKeys(CCArray*), removeObjectForElememt(CCDictElement*).
     */
    void removeAllObjects();

    /// @{
    /// @name Function override
    /**
     *  This function is used for deepcopy elements from source dictionary to destination dictionary.
     *  You shouldn't invoke this function manually since it's called by CCObject::copy.
     *  @lua NA
     *  @js  NA
     */
    virtual Object* copyWithZone(CCZone* pZone);
    /// @}
    
    /**
     *  Return a random object in the dictionary.
     *
     *  @return The random object. 
     *  @see objectForKey(intptr_t), objectForKey(const std::string&)
     *  @lua NA
     */
    Object* randomObject();
    
    /**
     *  Create a dictionary.
     *  @return A dictionary which is an autorelease object.
     *  @see createWithDictionary(CCDictionary*), createWithContentsOfFile(const char*), createWithContentsOfFileThreadSafe(const char*).
     */
    static Dictionary* create();

    /**
     *  Create a dictionary with an existing dictionary.
     *
     *  @param srcDict The exist dictionary.
     *  @return A dictionary which is an autorelease object.
     *  @see create(), createWithContentsOfFile(const char*), createWithContentsOfFileThreadSafe(const char*).
     */
    static Dictionary* createWithDictionary(Dictionary* srcDict);
    
    /**
     *  Create a dictionary with a plist file.
     *  @param  pFileName  The name of the plist file.
     *  @return A dictionary which is an autorelease object.
     *  @see create(), createWithDictionary(CCDictionary*), createWithContentsOfFileThreadSafe(const char*).
     */
    static Dictionary* createWithContentsOfFile(const char *pFileName);
    
    /**
     *  Write a dictionary to a plist file.
     *  @param fullPath The full path of the plist file. You can get writeable path by getWritablePath()
     *  @return true if successed, false if failed
     *  @lua NA
     */
    bool writeToFile(const char *fullPath);
     
    /**
     *  Create a dictionary with a plist file.
     *  
     *  @note the return object isn't an autorelease object.
     *        This can make sure not using autorelease pool in a new thread.
     *        Therefore, you need to manage the lifecycle of the return object.
     *        It means that when you don't need it, CC_SAFE_RELEASE needs to be invoked.
     *
     *  @param  pFileName  The name of the plist file.
     *  @return A dictionary which isn't an autorelease object.
     *  @lua NA
     */
    static Dictionary* createWithContentsOfFileThreadSafe(const char *pFileName);

    /* override functions 
     * @lua NA
     */
    virtual void acceptVisitor(CCDataVisitor &visitor);

private:
    /** 
     *  For internal usage, invoked by setObject.
     */
    void setObjectUnSafe(Object* pObject, const std::string& key);
    void setObjectUnSafe(Object* pObject, const intptr_t key);
    
public:
    /**
     *  All the elements in dictionary.
     *  词汇的哈希表头部结构指针
     *  @note For internal usage, we need to declare this member variable as public since it's used in UT_HASH.
     */
    DictItem* m_pItems;
private:
    
    /** The support type of dictionary, it's confirmed when setObject is invoked. */
    enum DictType
    {
        kDictUnknown = 0,
        kDictStr,
        kDictInt
    };
    
    /** 
     *  The type of dictionary, it's assigned to kDictUnknown by default.
	 *  当前词典查询类型。一个词典实例要求只有一种固定词典查询类型。
     */
    DictType m_eDictType;
};

// end of data_structure group
/// @}

FLAKOR_CC_END

#endif /* __CCDICTIONARY_H__ */
