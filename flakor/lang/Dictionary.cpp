#include "Dictionary.h"
#include "String.h"
#include "Integer.h"
#include "platform/FileUtils.h"

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
		char* pEnd = (char *)*pszKey[len-1];
		pStart = pEnd - (MAX_KEY_LEN-1);
	}

	strcpy(m_szKey, pStart);

	m_pObject = pObject;
	memset(&hh, 0, sizeof(hh));
}

DictItem::DictItem(intptr_t iKey, Object pObject)
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
: m_pElements(NULL)
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
            String* pOneKey = new String(pItem->m_szKey);
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
                String* pOneKey = new String(pItem->m_szKey);
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
    // This method uses string as key, therefore we should make sure that the key type of this CCDictionary is string.
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
    // This method uses integer as key, therefore we should make sure that the key type of this CCDictionary is integer.
    FKAssert(m_eDictType == kCCDictInt, "this dictionary does not use integer as key.");

    CCObject* pRetObject = NULL;
    CCDictElement *pElement = NULL;
    HASH_FIND_PTR(m_pElements, &key, pElement);
    if (pElement != NULL)
    {
        pRetObject = pElement->m_pObject;
    }
    return pRetObject;
}

const CCString* CCDictionary::valueForKey(const std::string& key)
{
    CCString* pStr = dynamic_cast<CCString*>(objectForKey(key));
    if (pStr == NULL)
    {
        pStr = CCString::create("");
    }
    return pStr;
}

const CCString* CCDictionary::valueForKey(intptr_t key)
{
    CCString* pStr = dynamic_cast<CCString*>(objectForKey(key));
    if (pStr == NULL)
    {
        pStr = CCString::create("");
    }
    return pStr;
}

void CCDictionary::setObject(CCObject* pObject, const std::string& key)
{
    CCAssert(key.length() > 0 && pObject != NULL, "Invalid Argument!");
    if (m_eDictType == kCCDictUnknown)
    {
        m_eDictType = kCCDictStr;
    }

    CCAssert(m_eDictType == kCCDictStr, "this dictionary doesn't use string as key.");

    CCDictElement *pElement = NULL;
    HASH_FIND_STR(m_pElements, key.c_str(), pElement);
    if (pElement == NULL)
    {
        setObjectUnSafe(pObject, key);
    }
    else if (pElement->m_pObject != pObject)
    {
        CCObject* pTmpObj = pElement->m_pObject;
        pTmpObj->retain();
        removeObjectForElememt(pElement);
        setObjectUnSafe(pObject, key);
        pTmpObj->release();
    }
}

void CCDictionary::setObject(CCObject* pObject, intptr_t key)
{
    CCAssert(pObject != NULL, "Invalid Argument!");
    if (m_eDictType == kCCDictUnknown)
    {
        m_eDictType = kCCDictInt;
    }

    CCAssert(m_eDictType == kCCDictInt, "this dictionary doesn't use integer as key.");

    CCDictElement *pElement = NULL;
    HASH_FIND_PTR(m_pElements, &key, pElement);
    if (pElement == NULL)
    {
        setObjectUnSafe(pObject, key);
    }
    else if (pElement->m_pObject != pObject)
    {
        CCObject* pTmpObj = pElement->m_pObject;
        pTmpObj->retain();
        removeObjectForElememt(pElement);
        setObjectUnSafe(pObject, key);
        pTmpObj->release();
    }

}

void CCDictionary::removeObjectForKey(const std::string& key)
{
    if (m_eDictType == kCCDictUnknown)
    {
        return;
    }
    
    CCAssert(m_eDictType == kCCDictStr, "this dictionary doesn't use string as its key");
    CCAssert(key.length() > 0, "Invalid Argument!");
    CCDictElement *pElement = NULL;
    HASH_FIND_STR(m_pElements, key.c_str(), pElement);
    removeObjectForElememt(pElement);
}

void CCDictionary::removeObjectForKey(intptr_t key)
{
    if (m_eDictType == kCCDictUnknown)
    {
        return;
    }
    
    CCAssert(m_eDictType == kCCDictInt, "this dictionary doesn't use integer as its key");
    CCDictElement *pElement = NULL;
    HASH_FIND_PTR(m_pElements, &key, pElement);
    removeObjectForElememt(pElement);
}

void CCDictionary::setObjectUnSafe(CCObject* pObject, const std::string& key)
{
    pObject->retain();
    CCDictElement* pElement = new CCDictElement(key.c_str(), pObject);
    HASH_ADD_STR(m_pElements, m_szKey, pElement);
}

void CCDictionary::setObjectUnSafe(CCObject* pObject, const intptr_t key)
{
    pObject->retain();
    CCDictElement* pElement = new CCDictElement(key, pObject);
    HASH_ADD_PTR(m_pElements, m_iKey, pElement);
}

void CCDictionary::removeObjectsForKeys(CCArray* pKeyArray)
{
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(pKeyArray, pObj)
    {
        CCString* pStr = (CCString*)pObj;
        removeObjectForKey(pStr->getCString());
    }
}

void CCDictionary::removeObjectForElememt(CCDictElement* pElement)
{
    if (pElement != NULL)
    {
        HASH_DEL(m_pElements, pElement);
        pElement->m_pObject->release();
        CC_SAFE_DELETE(pElement);
    }
}

void CCDictionary::removeAllObjects()
{
    CCDictElement *pElement, *tmp;
    HASH_ITER(hh, m_pElements, pElement, tmp) 
    {
        HASH_DEL(m_pElements, pElement);
        pElement->m_pObject->release();
        CC_SAFE_DELETE(pElement);

    }
}

CCObject* CCDictionary::copyWithZone(CCZone* pZone)
{
    CCAssert(pZone == NULL, "CCDictionary should not be inherited.");
    CCDictionary* pNewDict = new CCDictionary();

    CCDictElement* pElement = NULL;
    CCObject* pTmpObj = NULL;

    if (m_eDictType == kCCDictInt)
    {
        CCDICT_FOREACH(this, pElement)
        {
            pTmpObj = pElement->getObject()->copy();
            pNewDict->setObject(pTmpObj, pElement->getIntKey());
            pTmpObj->release();
        }
    }
    else if (m_eDictType == kCCDictStr)
    {
        CCDICT_FOREACH(this, pElement)
        {
            pTmpObj = pElement->getObject()->copy();
            pNewDict->setObject(pTmpObj, pElement->getStrKey());
            pTmpObj->release();
        }
    }

    return pNewDict;
}

CCObject* CCDictionary::randomObject()
{
    if (m_eDictType == kCCDictUnknown)
    {
        return NULL;
    }
    
    CCObject* key = allKeys()->randomObject();
    
    if (m_eDictType == kCCDictInt)
    {
        return objectForKey(((CCInteger*)key)->getValue());
    }
    else if (m_eDictType == kCCDictStr)
    {
        return objectForKey(((CCString*)key)->getCString());
    }
    else
    {
        return NULL;
    }
}

CCDictionary* CCDictionary::create()
{
    CCDictionary* pRet = new CCDictionary();
    if (pRet != NULL)
    {
        pRet->autorelease();
    }
    return pRet;
}

CCDictionary* CCDictionary::createWithDictionary(CCDictionary* srcDict)
{
    CCDictionary* pNewDict = (CCDictionary*)srcDict->copy();
    pNewDict->autorelease();
    return pNewDict;
}

CCDictionary* CCDictionary::createWithContentsOfFileThreadSafe(const char *pFileName)
{
    return CCFileUtils::sharedFileUtils()->createCCDictionaryWithContentsOfFile(pFileName);
}

void CCDictionary::acceptVisitor(CCDataVisitor &visitor)
{
    return visitor.visit(this);
}

CCDictionary* CCDictionary::createWithContentsOfFile(const char *pFileName)
{
    CCDictionary* pRet = createWithContentsOfFileThreadSafe(pFileName);
    pRet->autorelease();
    return pRet;
}

bool CCDictionary::writeToFile(const char *fullPath)
{
    return CCFileUtils::sharedFileUtils()->writeToFile(this, fullPath);
}



FLAKOR_NS_END
