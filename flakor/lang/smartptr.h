#ifndef _FK_SMARTPTR_H_
#define _FK_SMARTPTR_H_

/*
Characters   
1. reference based and ciruit immunity.
2. determinative deconstructed and no-determinative deconstruct
3. polymorph supported. assign a derived-type point to a base-type point is possible
4. Array supported
5. mutithread safety
6. both primitive type and complex type are supported
7. use just like a raw point
8. new operator is leave not overloaded. you can overload it as you want
9. cross platform, Both Windows and Linux are supported
10. multi-inheritance supported 
11. run-time Array boundary check
//6. use in both single thread or multi thread model

constraints
1. don't use raw point, if you has that point managed by GC
2. don't use point to SmartPtr, but object only. That's to say, SmartPtr<A> p = .. not SmartPtr<A>* p = ..
3. avoid use multi-inheritence, if it is possible
4. avoid overload operator new, if it is possible
5. if you want pass a point to a function that accept raw proint only, such as CDC::SelectObject,
   be caution, you'd better don't use this SmartPtr. or I can add another more function to support
   external reference.
6. when you are define a point, use the most derived type if it is possible.
   such as, class B derived from A. 
   <I>class B:A{}<I>
   use 
   <I>SmartPtr<B> p = new B;<I> 
   but not
   <I>SmartPtr<A> p = new B;<I>
   if it is possible, though this cause no error.

*/
#include <malloc.h>
#include <set>
#ifdef WIN32
//#include <windows.h> //do not include windows.h, this may cause starnge compile time error
#ifndef _DLL
#error To use the util library, please compile the project with "Multithread DLL (/MD)" or "Multithread Debug DLL (/MDd)" 
#endif
#endif


//former declaration
class ObjectWrapper;
template<typename T> class SmartPtr;
template<typename T> class ArrayPtr;

typedef void (*DESTORY_PROC)(void*);
//extern bool collecting ;

template<typename S, typename T>
SmartPtr<S> ptr_static_cast(SmartPtr<T> ptr) { return static_cast<S*>(ptr.getTarget()); }

template<typename S, typename T>
SmartPtr<S> ptr_dynamic_cast(SmartPtr<T> ptr) { return dynamic_cast<S*>(ptr.getTarget()); }

#ifdef  _MSC_VER 
inline size_t blockSize(void *p)
{
#pragma warning(push)
#pragma warning( disable : 4311) 
	if((int)p%8)//when allocate an array in windows with MSVC, address returned by new is the address returned
				//by malloc +4;, so, if pass this address to _msize will cause an error
	{
#pragma warning(pop)
		//	return _msize_dbg(p, _NORMAL_BLOCK);
		return _msize((char*)p-4)-4;
	}
	return _msize(p);
	//return _msize_dbg(p, _NORMAL_BLOCK);
}
//#define blockSize _msize


#elif defined(__GNUC__)
#define blockSize malloc_usable_size
#else
#error Only MS C++ and GCC are supported
#endif



#define ptrTrace(...) void()

//three types of a SmartPtr that can be. it be UNKNOW when it is created.
//if a SmartPtr object is iner another object that managed by GC, the type of SmartPtr is MANAGED
//else, a SmartPtr is managed by user self, type is USER
#define PTR_UNKNOWN 0
#define PTR_USER	1
#define PTR_MANAGED	2

bool  isCollecting();

template <typename T>
struct destructor
{
	static void destroy(void* obj) { delete static_cast<T*>(obj); }
};
template <typename T>
struct arrayDestructor
{
	static void destroy(void* obj) { delete[] static_cast<T*>(obj); }
};
#ifdef WIN32
class  Lock
{
public:

	Lock();
	~Lock();
};
#else

class Lock
{
	static pthread_mutex_t mutex;
public:
	Lock()
	{
		static bool inited=false;
		if(!inited)
		{
			pthread_mutexattr_t attr;
			pthread_mutexattr_init(&attr);
			pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
			pthread_mutex_init (&mutex,&attr);
			inited=true;
		}
		pthread_mutex_lock (&mutex);
	}
	~Lock()
	{
		pthread_mutex_unlock(&mutex);
		
	}
};
#endif

class  LinkNode
{
	LinkNode *pPrev;
	LinkNode *pNext;
public:
	LinkNode()
	{
		pNext = NULL;
		pPrev = NULL;
	}
	friend class SmartPtrManager;
	friend class WrapperManager;
	friend void mark(SmartPtr<void>* ptr);
	friend  void gc();
};


class ObjectWrapper;
/**
 * struct WrapperPointer is a simple struct contains a ObjectWrapper pointer. We want put a ObjectWrapper* into a 
 * STL set, but the compiler doesn't allow us to define a operator < for a pointer. We must encapsulate the pointer
 * as a struct, then overload the operator < for the struct
 */
struct WrapperPointer
{
	ObjectWrapper* p;
	WrapperPointer(ObjectWrapper* p) { this->p = p; }
	WrapperPointer() {}
};
bool  operator < (const WrapperPointer& w1, const WrapperPointer& w2);

class   ObjectWrapper 
{
	int count;	//reference count
	void *pTarget;	//the real object allocated by usrer;
	DESTORY_PROC destory; //a routine to call delete method
	bool inuse;
	size_t size;	//size of pObj. may not equal sizeof(T)
private:
	~ObjectWrapper();
	ObjectWrapper(void * p,DESTORY_PROC destory,size_t memSize=0);
public:
	void addRef();
	void release();
	void* getTarget();
	void setFinalizer(DESTORY_PROC finalize);
	friend class WrapperManager;
	friend  void gc();
	friend void mark(SmartPtr<void>* ptr);
	template<typename T>
		friend class ArrayPtr;
	friend bool  operator < (const WrapperPointer& w1, const WrapperPointer& w2);
};


/**
manage all the ObjectWrapper and ArrayWrapper.
*/
class  WrapperManager 
{
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning( disable : 4251) 
#endif
	typedef std::set<WrapperPointer> WrapperSet;
	WrapperSet wrappers;
#ifdef _MSC_VER
#pragma warning(pop)
#endif

	ObjectWrapper* nullWrapper;
/**
	return an element of ObjectWrapper list, where pTarget should 
	be insert after.
	return pos, where pos->pTarget <= pTarget < (pos+1)->pTarget
	<B> NOTE: not used now. use STL set to store wrapper objects.
	*/
	//ObjectWrapper* positionToInsert(void *pTarget);
	WrapperManager();
	~WrapperManager();
public:
	/**
	size of extra bytes more than requested allocated by new.
	*/
	static int extraSize;
	/**
	size reserved at head of the memory block
	*/
	static int arrayHeadSize;
	void remove(ObjectWrapper* pWrapper);
	void collect(ObjectWrapper* pWrapper);
	/**
	if target has already exist in the list, just return the old Wrapper.
	this allow to cast a derived-type point to a base-type point. 
	this make multi-inheritance enabled
	else create a new Wrapper
	If a new Wrapper is create and returned, the new Wrapper has reference count 1
	If an exists Wrapper is returned, the reference count is add 1 before return
	*/
	template <typename T>
	ObjectWrapper* getWrapper(void* pTarget)
	{
		Lock l;
		if(pTarget == NULL)
			return nullWrapper;

		// a non zero memSize make the ObjectWrapper use this size instead of calculate size itself
		// which save the computer resource
		ObjectWrapper tempWrapper(pTarget, NULL, 1);

		
		WrapperSet::const_iterator i = wrappers.find(&tempWrapper);
		if(i == wrappers.end())
		{
			ObjectWrapper* pW = new ObjectWrapper(pTarget,destructor<T>::destroy);
			wrappers.insert(pW);
			return pW;
		}
		else
		{
			(*i).p->addRef();
			return (*i).p;
		}
	}
	bool isManagedPtr(SmartPtr<void> *ptr);
	static WrapperManager* getInstance();
	friend  void gc();
	friend void mark(SmartPtr<void>* ptr);
	friend bool  operator < (const WrapperPointer& w1, const WrapperPointer& w2);
};

class  SmartPtrManager
{
	LinkNode *pHead;
	LinkNode *pTail;
	SmartPtrManager();
	~SmartPtrManager();

	LinkNode *pUserPtrHead;
	LinkNode *pUserPtrTail;

	LinkNode *pEmbeddedPtrHead;
	LinkNode *pEmbeddedPtrTail;

public:
	static SmartPtrManager* getInstance();
	void add(LinkNode* ptr);
	void remove(LinkNode* ptr);
	void moveToUserPtr(LinkNode* ptr);
	void moveToEmbeddedPtr(LinkNode* ptr);
	friend  void gc();
	friend void mark(SmartPtr<void>* ptr);
};


template<typename T> class SmartPtr : public LinkNode
{
protected:
#ifdef _DEBUG
	T* targetObj;
#else
	int reserved; //reserved
#endif
	ObjectWrapper *pWrapper;
public:
	SmartPtr<T>& operator=(T* p)
	{
		Lock l;
		ptrTrace("SmartPtr operator=(T* p)\r\n");
		ObjectWrapper *old = pWrapper;
		pWrapper = WrapperManager::getInstance()->getWrapper<T>(p);
		old->release();
#ifdef _DEBUG
		targetObj = (T*)pWrapper->getTarget();
#endif

		return *this;
	}
	SmartPtr<T>& operator=(const SmartPtr<T> &ptr)
	{
		Lock l;
		ptrTrace("SmartPtr operator=(const SmartPtr<T> &ptr)\r\n");
		if(pWrapper == ptr.pWrapper)//assign to self
			return *this;
		pWrapper->release();
		pWrapper = ptr.pWrapper;
		pWrapper->addRef();
#ifdef _DEBUG
		targetObj = (T*)pWrapper->getTarget();
#endif
		return *this;
	}
	SmartPtr(const SmartPtr<T>& ptr)
	{
		Lock l;
		SmartPtrManager::getInstance()->add(this);
		ptrTrace("SmartPtr con (const SmartPtr<T>& ptr) %p\r\n",this);
		pWrapper = ptr.pWrapper;
		pWrapper->addRef();
#ifdef _DEBUG
		targetObj = (T*)pWrapper->getTarget();
#endif
		//type = PTR_UNKNOWN;
	}
	SmartPtr(T* pObj=NULL)
	{
		Lock l;
		SmartPtrManager::getInstance()->add(this);
		ptrTrace("SmartPtr con (T* pObj) %p\r\n",this);
		pWrapper = WrapperManager::getInstance()->getWrapper<T>(pObj);
#ifdef _DEBUG
		targetObj = (T*)pWrapper->getTarget();
#endif
		//type = PTR_UNKNOWN;
	}
public:
	//int operator==(T* p)
	//{
	//	return this->pWrapper->pObj == p;
	//}
	operator const T* () const
	{
		ptrTrace("SmartPtr operator T* \r\n");
		return getTarget();
	}

	operator T* () 
	{
		ptrTrace("SmartPtr operator T* \r\n");
		return getTarget();
	}
protected:
	inline const T* getTarget() const
	{
		return static_cast<T*> (pWrapper->getTarget());
	}

	inline T* getTarget() 
	{
		return static_cast<T*> (pWrapper->getTarget());
	}

public:
	T* operator->() {return getTarget();}
	const T* operator->() const {return getTarget();}
	~SmartPtr(void)
	{
		Lock l;
		ptrTrace("SmartPtr decon %p\r\n",this);
		SmartPtrManager::getInstance()->remove(this);
		if(isCollecting())
			return;
		pWrapper->release();
	}


	//template<typename P>
	//operator P* () 
	//{
	//	return static_cast<P*>(getTarget());
	//}

	friend void mark(SmartPtr<void>* p);
	friend  void gc(void);

	template<typename S, typename V>
	friend SmartPtr<S> ptr_static_cast(SmartPtr<V> ptr);

	template<typename S, typename V>
	friend SmartPtr<S> ptr_dynamic_cast(SmartPtr<V> ptr);
};

template<typename T> class ArrayPtr : public SmartPtr<T>
{
	//up bound of array
	int upBound;
public:
	ArrayPtr<T>& operator=(T* p)
	{
		SmartPtr<T>::operator=(p);
		SmartPtr<T>::pWrapper->setFinalizer(&arrayDestructor<T>::destroy);
		upBound = SmartPtr<T>::pWrapper->size/sizeof(T);
		return *this;
	}
	ArrayPtr<T>& operator=(const ArrayPtr<T> &ptr)
	{
		SmartPtr<T>::operator=(ptr);
		upBound = ptr->upBound;
		return *this;
	}
	ArrayPtr(const ArrayPtr<T>& ptr):SmartPtr<T>(ptr)
	{
		upBound = ptr.upBound;
	}
	ArrayPtr(T pObj[]=NULL):SmartPtr<T>(pObj)
	{
		SmartPtr<T>::pWrapper->setFinalizer(&arrayDestructor<T>::destroy);
		///determine size of an array block
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable:4267)
#endif
		upBound = SmartPtr<T>::pWrapper->size/sizeof(T);
#ifdef _MSC_VER
#pragma warning (pop)
#endif
	}
	//operator T* ()
	//{
	//	return getTarget();
	//}

	//inline T* getTarget()
	//{
	//	return static_cast<T*> (pWrapper->getTarget());
	//}
	T* operator->(){return SmartPtr<T>::getTarget();}
	T& operator[](int index)
	{
		return SmartPtr<T>::getTarget()[index];
	}
public:
	~ArrayPtr(void)
	{
		ptrTrace("ArrayPtr decon %p\r\n",this);
	}

	friend void mark(SmartPtr<void>* p);
	friend  void gc(void);
};

 void gc();
#endif //_FK_SMARTPTR_H_
