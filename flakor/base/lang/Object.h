#ifndef _FK_OBJECT_H_
#define _FK_OBJECT_H_

#include "macros.h"

FLAKOR_NS_BEGIN

class Entity;
class AutoReleasePool;

/** Interface that defines how to clone an Object */
class Clonable
{
public:
    /** returns a copy of the Object */
    virtual Clonable* clone() const = 0;
    /**
     * @js NA
     * @lua NA
     */
    virtual ~Clonable() {};
};

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

typedef void (Object::*SEL_UPDATE)(float);
typedef void (Object::*SEL_CallFunc)();
typedef void (Object::*SEL_CallFuncE)(Entity*);
typedef void (Object::*SEL_CallFuncED)(Entity*, void*);
typedef void (Object::*SEL_CallFuncO)(Object*);
typedef void (Object::*SEL_MenuHandler)(Object*);
//typedef void (Object::*SEL_EventHandler)(Event*);
typedef int (Object::*SEL_Compare)(Object*);

#define update_selector(_SELECTOR) (SEL_UPDATE)(&_SELECTOR)
#define callfunc_selector(_SELECTOR) (SEL_CallFunc)(&_SELECTOR)
#define callfuncE_selector(_SELECTOR) (SEL_CallFuncE)(&_SELECTOR)
#define callfuncED_selector(_SELECTOR) (SEL_CallFuncED)(&_SELECTOR)
#define callfuncO_selector(_SELECTOR) (SEL_CallFuncO)(&_SELECTOR)
#define menu_selector(_SELECTOR) (SEL_MenuHandler)(&_SELECTOR)
#define event_selector(_SELECTOR) (SEL_EventHandler)(&_SELECTOR)
#define compare_selector(_SELECTOR) (SEL_Compare)(&_SELECTOR)

FLAKOR_NS_END

#endif
