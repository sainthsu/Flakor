#ifndef _FK_OBJECT_H_
#define _FK_OBJECT_H_

FLAKOR_NS_BEGIN

class Entity;
class AutoReleasePool;

class Object
{
	public:
    	// object id, ScriptSupport need public m_uID
    	unsigned int        m_uID;
    	// Lua reference id
    	int                 m_nLuaID;
	protected:
    	// count of references
    	unsigned int        m_uReference;
    	// count of autorelease
    	unsigned int        m_uAutoReleaseCount;
	public:
		Object(void);
		virtual ~Object(void);

    	bool isSingleReference(void) const;
    	unsigned int retainCount(void) const;
		void release(void);
	    void retain(void);
	    Object* autorelease(void);
		Object* copy();

		virtual bool equal(const Object* pObject);
		virtual void finalize();
    	friend class AutoreleasePool;
};

typedef void (Object::*SEL_SCHEDULE)(float);
typedef void (Object::*SEL_CallFunc)();
typedef void (Object::*SEL_CallFuncN)(Entity*);
typedef void (Object::*SEL_CallFuncND)(Entity*, void*);
typedef void (Object::*SEL_CallFuncO)(Object*);
typedef void (Object::*SEL_MenuHandler)(Object*);
//typedef void (Object::*SEL_EventHandler)(Event*);
typedef int (Object::*SEL_Compare)(Object*);

#define schedule_selector(_SELECTOR) (SEL_SCHEDULE)(&_SELECTOR)
#define callfunc_selector(_SELECTOR) (SEL_CallFunc)(&_SELECTOR)
#define callfuncN_selector(_SELECTOR) (SEL_CallFuncN)(&_SELECTOR)
#define callfuncND_selector(_SELECTOR) (SEL_CallFuncND)(&_SELECTOR)
#define callfuncO_selector(_SELECTOR) (SEL_CallFuncO)(&_SELECTOR)
#define menu_selector(_SELECTOR) (SEL_MenuHandler)(&_SELECTOR)
#define event_selector(_SELECTOR) (SEL_EventHandler)(&_SELECTOR)
#define compare_selector(_SELECTOR) (SEL_Compare)(&_SELECTOR)

FLAKOR_NS_END

#endif
