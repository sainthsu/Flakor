#include "base/lang/Dictionary.h"
#include "base/lang/DataVisitor.h"
#include "base/lang/Types.h"
//#include "platform/FileUtils.h"

using namespace std;

FLAKOR_NS_BEGIN

DictItem::DictItem(const char* pszKey,Object* pObject)
{
	FKAssert(pszKey && strlen(pszKey) > 0, "Invalid key value");
	m_iKey = 0;
	const char* pStart = pszKey;
	
	int len = strlen(pszKey);
	if(len > MAX_KEY_LEN)
	{
		char* pEnd = (char *)&pszKey[len-1];
		pStart = pEnd - (MAX_KEY_LEN-1);
	}

	strcpy(m_szKey, pStart);

	m_pObject = pObject;
	memset(&hh, 0, sizeof(hh));
}

DictItem::DictItem(intptr_t iKey, Object* pObject)
{
	m_szKey[0] = '\0';
	m_iKey = iKey;
	m_pObject = pObject;
	memset(&hh, 0, sizeof(hh));
}

DictItem::~DictItem(void)
{

}

// -----------------------------------------------------------------------
// Dictionary

Dictionary::Dictionary(void)
: m_pItems(NULL)
, m_eDictType(kDictUnknown)
{

}

Dictionary::~Dictionary(void)
{
    removeAllObjects();
}

unsigned int Dictionary::count(void)
{
	return HASH_COUNT(m_pItems);
}

Array* Dictionary::allKeys(void)
{
	int iKeyCount = this->count();
	if (iKeyCount <= 0) return NULL;
	
	Array* pArray = Array::createWithCapacity(iKeyCount);

    DictItem *pItem, *tmp;
    if (m_eDictType == kDictStr)
    {
        HASH_ITER(hh, m_pItems, pItem, tmp) 
        {
            String* pOneKey = String::create(pItem->m_szKey);
            pArray->addObject(pOneKey);
            FK_SAFE_RELEASE(pOneKey);
        }
    }
    else if (m_eDictType == kDictInt)
    {
        HASH_ITER(hh, m_pItems, pItem, tmp) 
        {
            Integer* pOneKey = new Integer(pItem->m_iKey);
            pArray->addObject(pOneKey);
            FK_SAFE_RELEASE(pOneKey);
        }
    }
	
	return pArray;
}

Array* Dictionary::allKeysForObject(Object* object)
{
    int iKeyCount = this->count();
    if (iKeyCount <= 0) return NULL;
    Array* pArray = Array::create();

    DictItem *pItem, *tmp;

    if (m_eDictType == kDictStr)
    {
        HASH_ITER(hh, m_pItems, pItem, tmp) 
        {
            if (object == pItem->m_pObject)
            {
                String* pOneKey = String::create(pItem->m_szKey);
                pArray->addObject(pOneKey);
                FK_SAFE_RELEASE(pOneKey);
            }
        }
    }
    else if (m_eDictType == kDictInt)
    {
        HASH_ITER(hh, m_pItems, pItem, tmp) 
        {
            if (object == pItem->m_pObject)
            {
                Integer* pOneKey = new Integer(pItem->m_iKey);
                pArray->addObject(pOneKey);
                FK_SAFE_RELEASE(pOneKey);
            }
        }
    }
    return pArray;
}

Object* Dictionary::objectForKey(const std::string& key)
{
    // if dictionary wasn't initialized, return NULL directly.
    if (m_eDictType == kDictUnknown) return NULL;
    // Dictionary only supports one kind of key, string or integer.
    // This method uses string as key, therefore we should make sure that the key type of this Dictionary is string.
    FKAssert(m_eDictType == kDictStr, "this dictionary does not use string as key.");

    Object* pRetObject = NULL;
    DictItem *pItem = NULL;
    HASH_FIND_STR(m_pItems, key.c_str(), pItem);
    if (pItem != NULL)
    {
        pRetObject = pItem->m_pObject;
    }
    return pRetObject;
}

Object* Dictionary::objectForKey(intptr_t key)
{
    // if dictionary wasn't initialized, return NULL directly.
    if (m_eDictType == kDictUnknown) return NULL;
    // Dictionary only supports one kind of key, string or integer.
    // This method uses integer as key, therefore we should make sure that the key type of this Dictionary is integer.
    FKAssert(m_eDictType == kDictInt, "this dictionary does not use integer as key.");

    Object* pRetObject = NULL;
    DictItem *pItem = NULL;
    HASH_FIND_PTR(m_pItems, &key, pItem);
    if (pItem != NULL)
    {
        pRetObject = pItem->m_pObject;
    }
    return pRetObject;
}

const String* Dictionary::valueForKey(const std::string& key)
{
    String* pStr = dynamic_cast<String*>(objectForKey(key));
    if (pStr == NULL)
    {
        pStr = String::create("");
    }
    return pStr;
}

const String* Dictionary::valueForKey(intptr_t key)
{
    String* pStr = dynamic_cast<String*>(objectForKey(key));
    if (pStr == NULL)
    {
        pStr = String::create("");
    }
    return pStr;
}

void Dictionary::setObject(Object* pObject, const std::string& key)
{
    FKAssert(key.length() > 0 && pObject != NULL, "Invalid Argument!");
    if (m_eDictType == kDictUnknown)
    {
        m_eDictType = kDictStr;
    }

    FKAssert(m_eDictType == kDictStr, "this dictionary doesn't use string as key.");

    DictItem *pItem = NULL;
    HASH_FIND_STR(m_pItems, key.c_str(), pItem);
    if (pItem == NULL)
    {
        setObjectUnSafe(pObject, key);
    }
    else if (pItem->m_pObject != pObject)
    {
        Object* pTmpObj = pItem->m_pObject;
        pTmpObj->retain();
        removeObjectForElememt(pItem);
        setObjectUnSafe(pObject, key);
        pTmpObj->release();
    }
}

void Dictionary::setObject(Object* pObject, intptr_t key)
{
    FKAssert(pObject != NULL, "Invalid Argument!");
    if (m_eDictType == kDictUnknown)
    {
        m_eDictType = kDictInt;
    }

    FKAssert(m_eDictType == kDictInt, "this dictionary doesn't use integer as key.");

    DictItem *pItem = NULL;
    HASH_FIND_PTR(m_pItems, &key, pItem);
    if (pItem == NULL)
    {
        setObjectUnSafe(pObject, key);
    }
    else if (pItem->m_pObject != pObject)
    {
        Object* pTmpObj = pItem->m_pObject;
        pTmpObj->retain();
        removeObjectForElememt(pItem);
        setObjectUnSafe(pObject, key);
        pTmpObj->release();
    }

}

void Dictionary::removeObjectForKey(const std::string& key)
{
    if (m_eDictType == kDictUnknown)
    {
        return;
    }
    
    FKAssert(m_eDictType == kDictStr, "this dictionary doesn't use string as its key");
    FKAssert(key.length() > 0, "Invalid Argument!");
    DictItem *pItem = NULL;
    HASH_FIND_STR(m_pItems, key.c_str(), pItem);
    removeObjectForElememt(pItem);
}

void Dictionary::removeObjectForKey(intptr_t key)
{
    if (m_eDictType == kDictUnknown)
    {
        return;
    }
    
    FKAssert(m_eDictType == kDictInt, "this dictionary doesn't use integer as its key");
    DictItem *pItem = NULL;
    HASH_FIND_PTR(m_pItems, &key, pItem);
    removeObjectForElememt(pItem);
}

void Dictionary::setObjectUnSafe(Object* pObject, const std::string& key)
{
    pObject->retain();
    DictItem* pItem = new DictItem(key.c_str(), pObject);
    HASH_ADD_STR(m_pItems, m_szKey, pItem);
}

void Dictionary::setObjectUnSafe(Object* pObject, const intptr_t key)
{
    pObject->retain();
    DictItem* pItem = new DictItem(key, pObject);
    HASH_ADD_PTR(m_pItems, m_iKey, pItem);
}

void Dictionary::removeObjectsForKeys(Array* pKeyArray)
{
    Object* pObj = NULL;
    FK_ARRAY_FOREACH(pKeyArray, pObj)
    {
        String* pStr = (String*)pObj;
        removeObjectForKey(pStr->getCString());
    }
}

void Dictionary::removeObjectForElememt(DictItem* pItem)
{
    if (pItem != NULL)
    {
        HASH_DEL(m_pItems, pItem);
        pItem->m_pObject->release();
        FK_SAFE_DELETE(pItem);
    }
}

void Dictionary::removeAllObjects()
{
    DictItem *pItem, *tmp;
    HASH_ITER(hh, m_pItems, pItem, tmp) 
    {
        HASH_DEL(m_pItems, pItem);
        pItem->m_pObject->release();
        FK_SAFE_DELETE(pItem);

    }
}

Object* Dictionary::copyWithZone(Zone* pZone)
{
    FKAssert(pZone == NULL, "Dictionary should not be inherited.");
    Dictionary* pNewDict = new Dictionary();

    DictItem* pItem = NULL;
    Object* pTmpObj = NULL;

    if (m_eDictType == kDictInt)
    {
        FK_DICT_FOREACH(this, pItem)
        {
            pTmpObj = pItem->getObject()->copy();
            pNewDict->setObject(pTmpObj, pItem->getIntKey());
            pTmpObj->release();
        }
    }
    else if (m_eDictType == kDictStr)
    {
        FK_DICT_FOREACH(this, pItem)
        {
            pTmpObj = pItem->getObject()->copy();
            pNewDict->setObject(pTmpObj, pItem->getStrKey());
            pTmpObj->release();
        }
    }

    return pNewDict;
}

Object* Dictionary::randomObject()
{
    if (m_eDictType == kDictUnknown)
    {
        return NULL;
    }
    
    Object* key = allKeys()->randomObject();
    
    if (m_eDictType == kDictInt)
    {
        return objectForKey(((Integer*)key)->getValue());
    }
    else if (m_eDictType == kDictStr)
    {
        return objectForKey(((String*)key)->getCString());
    }
    else
    {
        return NULL;
    }
}

Dictionary* Dictionary::create()
{
    Dictionary* pRet = new Dictionary();
    if (pRet != NULL)
    {
        pRet->autorelease();
    }
    return pRet;
}

Dictionary* Dictionary::createWithDictionary(Dictionary* srcDict)
{
    Dictionary* pNewDict = (Dictionary*)srcDict->copy();
    pNewDict->autorelease();
    return pNewDict;
}

Dictionary* Dictionary::createWithContentsOfFileThreadSafe(const char *pFileName)
{
	return NULL;
    //return FileUtils::sharedFileUtils()->createDictionaryWithContentsOfFile(pFileName);
}

void Dictionary::acceptVisitor(DataVisitor &visitor)
{
    return visitor.visit(this);
}

Dictionary* Dictionary::createWithContentsOfFile(const char *pFileName)
{
    Dictionary* pRet = createWithContentsOfFileThreadSafe(pFileName);
    pRet->autorelease();
    return pRet;
}

bool Dictionary::writeToFile(const char *fullPath)
{
	return false;
    //return FileUtils::sharedFileUtils()->writeToFile(this, fullPath);
}



FLAKOR_NS_END
