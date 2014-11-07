
#include "base/lang/Object.h"
#include "base/lang/AutoreleasePool.h"

FLAKOR_NS_BEGIN

Object::Object(void)
: m_nLuaID(0)
, m_uReference(1) // when the object is created, the reference count of it is 1
, m_uAutoReleaseCount(0)
{
    static unsigned int uObjectCount = 0;
    m_uID = ++uObjectCount;
}

Object::~Object(void)
{
    // if the object is managed, we should remove it
    // from pool manager
    if (m_uAutoReleaseCount > 0)
    {
        PoolManager::sharedPoolManager()->removeObject(this);
    }

    /** if the object is referenced by Lua engine, remove it
    if (m_nLuaID)
    {
        ScriptEngineManager::sharedManager()->getScriptEngine()->removeScriptObjectByCCObject(this);
    }
    else
    {
        ScriptEngineProtocol* pEngine = ScriptEngineManager::sharedManager()->getScriptEngine();
        if (pEngine != NULL && pEngine->getScriptType() == kScriptTypeJavascript)
        {
                pEngine->removeScriptObjectByCCObject(this);
        }
    }*/
}

Object* Object::copy()
{
}

void Object::release(void)
{
    FKAssert(m_uReference > 0, "reference count should greater than 0");
    --m_uReference;

    if (m_uReference == 0)
    {
        delete this;
    }
}

void Object::retain(void)
{
    FKAssert(m_uReference > 0, "reference count should greater than 0");

    ++m_uReference;
}

Object* Object::autorelease(void)
{
    PoolManager::sharedPoolManager()->addObject(this);
    return this;
}

bool Object::isSingleReference(void) const
{
    return m_uReference == 1;
}

unsigned int Object::retainCount(void) const
{
    return m_uReference;
}


bool Object::equal(const Object* pObject)
{
	return this == pObject;
}

void Object::finalize()
{

}

FLAKOR_NS_END
