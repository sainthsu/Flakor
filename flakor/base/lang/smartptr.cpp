#include "smartptr.h"
#include <stdio.h>
#include <stdlib.h>
#include <new.h>
#include <set>
#include <vector>


#ifndef WIN32
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

//former declaration
static void idleCollect();
#ifdef WIN32
#include "windows.h"
#include "pdh.h"
#pragma comment(lib, "Kernel32.lib")
#pragma comment(lib, "pdh.lib")

/**
 * set thread's name. copied from MSDN
 */
//
// Usage: SetThreadName (-1, "MainThread");
//
#define MS_VC_EXCEPTION 0x406D1388

typedef struct tagTHREADNAME_INFO
{
   DWORD dwType; // Must be 0x1000.
   LPCSTR szName; // Pointer to name (in user addr space).
   DWORD dwThreadID; // Thread ID (-1=caller thread).
   DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;

void SetThreadName( DWORD dwThreadID, LPCSTR szThreadName)
{
   THREADNAME_INFO info;
   info.dwType = 0x1000;
   info.szName = szThreadName;
   info.dwThreadID = dwThreadID;
   info.dwFlags = 0;

   __try
   {
      RaiseException( MS_VC_EXCEPTION, 0, sizeof(info)/sizeof(DWORD), (DWORD*)&info );
   }
   __except(EXCEPTION_CONTINUE_EXECUTION)
   {
   }
}

DWORD WINAPI threadProc(LPVOID )
{
	idleCollect();
	return 1;
}
typedef HANDLE T_HANDLE;
#else //Linux or UNIX
void* threadProc(void*)
{
	idleCollect();
	return NULL;
}
typedef pthread_t T_HANDLE;
#endif

//global variables;
bool collecting = false;
bool isCollecting(){return collecting;}

//static variable
#ifdef WIN32
static CRITICAL_SECTION Lock_cs;  // this variable is used by class Lock only
static _PNH old_new_handler;
static int old_new_mode;
#else
pthread_mutex_t Lock::mutex;
typedef void (*_PNH)(void);
static _PNH old_new_handler;
#define _set_new_handler set_new_handler
#endif

static T_HANDLE thread=0;
int WrapperManager::extraSize = -1;
static int gcCount = 0;//how many object has freed by gc
static bool quit = false;

#ifdef WIN32
///< Lock constructor
Lock::Lock() 
{
	static bool inited = false;
	if(!inited)
	{
		InitializeCriticalSection(&Lock_cs);//should call DeleteCriticalSection(&cs)
		inited = true;
	}
	EnterCriticalSection(&Lock_cs); 
}
///< Lock destructor
Lock::~Lock() 
{ 
	LeaveCriticalSection(&Lock_cs); 
}

#endif //WIN32

static void startThread()
{
#ifdef WIN32
	DWORD threadId;
	thread = CreateThread(NULL,  0,  threadProc,  NULL,  0,  &threadId);
	if(!thread)
		fprintf(stderr,"fail to create Garbage Collect thread\n");
	SetThreadName(threadId,"Garbage Collector");
#else
	int ret = pthread_create(&thread,NULL,threadProc,NULL);
	if(ret)
		fprintf(stderr,"fail to create Garbage Collect thread\n");
#endif

}
bool systemIdle()
{
#ifdef WIN32
	DWORD			counterType;
	HQUERY          hQuery;
	PDH_STATUS      pdhStatus;
	HCOUNTER        hCounter;
	static PDH_RAW_COUNTER lastrawdata;
	static bool first = true;


	TCHAR           szCounterPath[] = 
		TEXT("\\Processor(_Total)\\% Processor Time");

	/* We can use "% Idle Time" to retrieve IDL time directly, but this counter is available 
	 * on WindowsXP only. Fortunatelly, we can get IDL=100-Processer
	 */
	//TCHAR           szCounterPath[] = 
	//	TEXT("\\Processor(0)\\% Idle Time");

	// Open a query object.
	pdhStatus = PdhOpenQuery (NULL, 0, &hQuery);
	if( pdhStatus != ERROR_SUCCESS )
	{
		return true;
	}

	// Add one counter that will provide the data.
	pdhStatus = PdhAddCounter (hQuery,
		szCounterPath,
		0,
		&hCounter);
	if( pdhStatus != ERROR_SUCCESS )
	{
		PdhCloseQuery(hQuery);
		return true;
	}


	PDH_RAW_COUNTER rawdata2;

	pdhStatus = PdhCollectQueryData(hQuery);
	if (pdhStatus != ERROR_SUCCESS) 
	{
		PdhCloseQuery(hQuery);
		return true;
	}
	PdhGetRawCounterValue(hCounter, &counterType, &rawdata2);
	if(first)
	{
		lastrawdata = rawdata2;
		first = false;
		PdhCloseQuery(hQuery);
		return true;
	}

	pdhStatus = PdhCollectQueryData(hQuery);
	if (pdhStatus != ERROR_SUCCESS) 
	{
		PdhCloseQuery(hQuery);
		return true;
	}
	PdhGetRawCounterValue(hCounter, &counterType, &rawdata2);


	PDH_FMT_COUNTERVALUE fmtValue;
	pdhStatus = PdhCalculateCounterFromRawValue(hCounter,
		PDH_FMT_DOUBLE,
		&rawdata2,
		&lastrawdata,
		&fmtValue);
	if (pdhStatus != ERROR_SUCCESS) 
	{
		PdhCloseQuery(hQuery);
		return true;
	}
	lastrawdata = rawdata2;

	pdhStatus = PdhCloseQuery(hQuery);
	return fmtValue.doubleValue <= 20;
/*
	//Method 2, use GetSystemTimes, this is more Simple, but
	//GetSystemTimes is not available on Window2000 OS
	static __int64 last_idle = 0,last_kernel = 0,last_user = 0;
	__int64 idle, kernel, user;
	if(! GetSystemTimes((LPFILETIME)&idle,(LPFILETIME)&kernel,(LPFILETIME)&user))
	{
		ptrTrace("fail to call GetSystemTimes\r\n");
		return false;
	}
	//PdhOpenQuery
	double rate =  (double)(idle-last_idle)/(user-last_user+kernel-last_kernel);
	last_idle=idle;
	last_kernel = kernel;
	last_user = user;
	if( rate >= 0.8 )//more than 80% of system is in idle
		return true;
	return false;
*/
#else //Linux & UNIX
#define MAXLOAD 20 //that's to say, if the system overload is no more than 20%, it's idle
	char buf[4];
	int f=open("/proc/loadavg", O_RDONLY);
	if(f<0)
	{
		ptrTrace("fail to open file /proc/loadavg");
		return false;
	}
	if (read(f, buf, 4)!=4)
	{
		ptrTrace("fail to read file /proc/loadavg");
		close(f);
		return false;
	}
	close(f);
	return ((buf[0]=='0') && (atoi(buf+2))<MAXLOAD);
#endif
}
#define INTERVAL 30 //collect every 30 seconds
/**
when system is in idle, this function will be called to collect garbage.
*/
static void idleCollect() 
{
	unsigned int count = 0;
	while(!quit)
	{//quit of the appclication will delay at most 1 second
#ifdef WIN32
		Sleep(1000);//there's no sleep in MSVC? how strange!
#else
		sleep(1000); //nanosleep also available on Linux platform
#endif
		++count;
		if(count == INTERVAL)
		{
			count = 0;
			if(systemIdle())
			{
				gc();
			}
		}
	}
}


//ObjectWrapper* WrapperManager::positionToInsert(void *pTarget)
//{
//	Lock l;
//	ObjectWrapper* p= (ObjectWrapper*)pHead;
//	while(p->pNext!=pTail)
//	{
//		if(	pTarget >= p->pTarget && pTarget < ((ObjectWrapper*)p->pNext)->pTarget)
//		{
//			return p;
//		}
//		p=(ObjectWrapper*)p->pNext;
//	}
//	return p;
//}

WrapperManager::WrapperManager()
{
	Lock l;
	nullWrapper = new ObjectWrapper(NULL,NULL);
	if(extraSize < 0)
	{
		int *p = new int;
		extraSize = blockSize(p) - sizeof(int);//Windows,0; Linux, 4
		delete p;
	}
}
WrapperManager::~WrapperManager()
{
	Lock l;
	collecting = true;
	ptrTrace("WrapperManager deconstructed\r\n");

	for(WrapperSet::iterator i=wrappers.begin(); i!=wrappers.end(); i++)
	{
		delete (*i).p;
	}
	wrappers.clear();
	delete nullWrapper;
	collecting = false;

}
void WrapperManager::remove(ObjectWrapper* pWrapper)
{
	Lock l;
	if(pWrapper==nullWrapper)
		return;
	if(isCollecting())
		return;
	wrappers.erase(WrapperPointer(pWrapper));
	delete pWrapper;
}
void WrapperManager::collect(ObjectWrapper* pWrapper)
{
	Lock l;
	wrappers.erase(WrapperPointer(pWrapper));
	delete pWrapper; //delete the wrapper. which also make memory reclaimed

}
//int markCount = 0;
void mark(SmartPtr<void>* ptr)
{
	Lock l;
	ObjectWrapper* pWrapper = ptr->pWrapper;
	pWrapper->inuse = true;
	SmartPtrManager* ptrManager = SmartPtrManager::getInstance();
	LinkNode * p= (LinkNode*)ptrManager->pEmbeddedPtrHead->pNext;
	int i=0;
	while(p!=ptrManager->pEmbeddedPtrTail)
	{
		//++markCount;
		//SmartPtr<void>* point = static_cast<SmartPtr<void>*>(p);
		SmartPtr<void>* point = (SmartPtr<void>*)p;
		unsigned char * address = (unsigned char*)point;
		if((!point->pWrapper->inuse)
			&& (address >= pWrapper->pTarget
				&& address < (unsigned char*)pWrapper->pTarget + pWrapper->size)
				)
			{//point is in scope of ptr
					mark(point);
			}
		p= (LinkNode*)p->pNext;
	}
}

/**
 * return true if the ptr itself is located in the inner of other object managed by GC.
 */
bool WrapperManager::isManagedPtr(SmartPtr<void> *ptr)
{
	Lock l;
	ObjectWrapper tempWrapper(reinterpret_cast<void*>(ptr), NULL, 1);
	WrapperSet::const_iterator i = wrappers.find(WrapperPointer(&tempWrapper));
	return i != wrappers.end();

}
 void gc()
{
	//LOG_FATAL("Begin mark ptr");
	//markCount = 0;
	Lock l;
	ptrTrace("to collect garbage\r\n");
	SmartPtrManager* ptrManager = SmartPtrManager::getInstance();
	WrapperManager *wrapManager = WrapperManager::getInstance();
	LinkNode * p= (LinkNode*)ptrManager->pHead->pNext;
	//update Ptr's state  that are new created
	while(p!=ptrManager->pTail)
	{
		SmartPtr<void>* ptr = static_cast<SmartPtr<void>*>(p);
		p=p->pNext;
		if(wrapManager->isManagedPtr(ptr))
			ptrManager->moveToEmbeddedPtr(ptr);
		else
			ptrManager->moveToUserPtr(ptr);
	}

	//mark all point as nouse
	for(WrapperManager::WrapperSet::iterator i = wrapManager->wrappers.begin(); i !=  wrapManager->wrappers.end(); i++)
	{
		(*i).p->inuse = false;
	}

	//markup all used points
	p= (LinkNode*)ptrManager->pUserPtrHead->pNext;
	while(p!=ptrManager->pUserPtrTail)
	{
		mark(static_cast<SmartPtr<void>*>(p));
		p=p->pNext;
	}
	
	//LOG_FATAL("Finish mark. %d ptr processed, total %d objects", markCount, WrapperManager::getInstance()->wrappers.size());

	//clear all unused object
	gcCount = 0;
	collecting = true;

	/**
	 * use a vector to store all the pointer that are no used. Because I can't delete the wrapper object during 
	 * iterating
	 */
	std::vector<WrapperPointer> garbage;
	for(WrapperManager::WrapperSet::iterator i = wrapManager->wrappers.begin(); i !=  wrapManager->wrappers.end(); i++)
	{
		WrapperPointer wp = *i;
		ObjectWrapper *pWrap = wp.p;
		if(!pWrap->inuse)
		{//no other reachable object use this one
			gcCount ++;
			garbage.push_back(wp);
			
		}
	}
	for(std::vector<WrapperPointer>::iterator i = garbage.begin(); i != garbage.end(); i++)
	{
		WrapperPointer wp = *i;
		wrapManager->wrappers.erase(wp);
		delete (wp).p; //delete the wrapper. which also make memory reclaimed
		//wrapManager->collect((*i).p); //collect this object and the wrapper object
	}
	collecting = false;
	ptrTrace("%d objects collected\r\n",gcCount);
	ptrTrace("complete gc\r\n");
}

#ifdef WIN32
int gc_new_handler(size_t sz)
#else
void gc_new_handler()
#endif
{
   gc();
   if(!gcCount)//there's no memory collected, make the defualt handler to deal with
   {
#ifdef WIN32
	    return old_new_handler(sz );
#else
		old_new_handler();
#endif
   }

#ifdef WIN32
   return gcCount; //GC has reclaimed some memory, do allocate again
#endif
}

SmartPtrManager* SmartPtrManager::getInstance()
{
	static SmartPtrManager Instance;
	return &Instance;
}
void SmartPtrManager::add(LinkNode* ptr)
{
	Lock l;
	pHead->pNext->pPrev = ptr;
	ptr->pNext = pHead->pNext;
	pHead->pNext = ptr;
	ptr->pPrev = pHead;

}
void SmartPtrManager::remove(LinkNode* ptr)
{
	Lock l;
	ptr->pPrev->pNext = ptr->pNext;
	ptr->pNext->pPrev = ptr->pPrev;
}

void SmartPtrManager::moveToUserPtr(LinkNode* ptr)
{
	Lock l;
	ptr->pPrev->pNext = ptr->pNext;
	ptr->pNext->pPrev = ptr->pPrev;

	pUserPtrHead->pNext->pPrev = ptr;
	ptr->pNext = pUserPtrHead->pNext;
	pUserPtrHead->pNext = ptr;
	ptr->pPrev = pUserPtrHead;

}
void SmartPtrManager::moveToEmbeddedPtr(LinkNode* ptr)
{
	Lock l;
	ptr->pPrev->pNext = ptr->pNext;
	ptr->pNext->pPrev = ptr->pPrev;

	pEmbeddedPtrHead->pNext->pPrev = ptr;
	ptr->pNext = pEmbeddedPtrHead->pNext;
	pEmbeddedPtrHead->pNext = ptr;
	ptr->pPrev = pEmbeddedPtrHead;

}

SmartPtrManager::SmartPtrManager()
{
	Lock l;
	pHead = new LinkNode;
	pTail = new LinkNode;
	pHead->pNext = pTail;
	pTail->pPrev = pHead;

	pUserPtrHead = new LinkNode;
	pUserPtrTail = new LinkNode;
	pUserPtrHead->pNext = pUserPtrTail;
	pUserPtrTail->pPrev = pUserPtrHead;

	pEmbeddedPtrHead = new LinkNode;
	pEmbeddedPtrTail = new LinkNode;
	pEmbeddedPtrHead->pNext = pEmbeddedPtrTail;
	pEmbeddedPtrTail->pPrev = pEmbeddedPtrHead;

	//do gc initialize here
	startThread(); //start a thread to collect garbage when system is in idle
	old_new_handler = ::_set_new_handler(gc_new_handler); //set new handler, so 
									//gc runs when there's no sufficient memory
#ifdef WIN32
	old_new_mode = ::_set_new_mode(1);
#endif
}
SmartPtrManager::~SmartPtrManager()
{
	Lock l;
	_set_new_handler(old_new_handler); //restore new handler
#ifdef WIN32
	_set_new_mode(old_new_mode);	   //and new mode
#endif
	quit=true;
	delete pHead;
	delete pTail;
}

ObjectWrapper::~ObjectWrapper()
{
	ptrTrace("Wrapper decon %p, target:%p\r\n",this,pTarget);
	if(pTarget && destory)
		destory( pTarget );
}

ObjectWrapper::ObjectWrapper(void * p,DESTORY_PROC destory,size_t memSize)
{
	ptrTrace("Wrapper con %p, target:%p\r\n",this,p);
	pTarget = p;
	if(p==NULL)
	{
		size=0;
	}
	else
	{
		if(memSize)
			size = memSize;
		else
		{
			size = blockSize(p) - WrapperManager::extraSize;
			//ASSERT(size);
		}
	}
	count = 1;
	this->destory = destory;
}

void ObjectWrapper::addRef(){++count;};

void ObjectWrapper::release()
{
	Lock l;
	--count;
	if(count==0)
	{
		WrapperManager::getInstance()->remove(this);
	}
}
void* ObjectWrapper::getTarget()
{
	return pTarget;
}

void ObjectWrapper::setFinalizer(DESTORY_PROC finalize)
{
	destory = finalize;
}

WrapperManager* WrapperManager::getInstance()
{
	static WrapperManager  Instance;
	
	return &Instance;
}

bool operator < (const WrapperPointer& w1, const WrapperPointer& w2)
{
	return ((char*)(w1.p->pTarget) + w1.p->size ) < w2.p->pTarget;
}
