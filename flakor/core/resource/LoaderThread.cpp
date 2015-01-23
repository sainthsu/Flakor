#include "targetMacros.h"
#include "core/resource/LoaderThread.h"
#include "core/resource/ResourceManager.h"
#include "core/resource/Resource.h"

#include   <unistd.h>


FLAKOR_NS_BEGIN

LoaderThread::LoaderThread()
:_pid(0)
,_thread(0)
{}

LoaderThread::~LoaderThread()
{
	
}

LoaderThread* LoaderThread::create()
{
    LoaderThread* thread = NULL;
    thread = new LoaderThread();

    return thread;
}

void LoaderThread::start()
{
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&_thread, &attr, LoaderThread::run, NULL);
}

void* LoaderThread::run(void* param)
{
    ResourceManager *mgr = ResourceManager::thisManager();

    for(;;)
    {
        if(mgr->waitLoads >= 1)
        {

            pthread_mutex_lock(&mgr->mutex);
			Resource* res = mgr->getWaitingRes();
            ILoader *loader = res->createLoader();
			loader->load(res);

            mgr->waitLoads--;
            pthread_mutex_unlock(&mgr->mutex);
        }
        else
        {
            usleep(80);
        }
    }
}


pid_t LoaderThread::getPid()
{
    if(_pid == 0)
        _pid = getpid();
    return _pid;
}

FLAKOR_NS_END
