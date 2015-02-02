#include "targetMacros.h"
#include "core/resource/LoaderThread.h"
#include "core/resource/ILoader.h"
#include "core/resource/ResourceManager.h"
#include "core/resource/Resource.h"

#include <unistd.h>


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
    pthread_create(&_thread, &attr, LoaderThread::run, this);
}

void* LoaderThread::run(void* thread)
{
    ResourceManager *mgr = ResourceManager::thisManager();
    LoaderThread* thr = (LoaderThread*)thread;
    thr->setRun(true);
    
    while(thr->isRunning())
    {
        if(mgr->waitLoads >= 1)
        {

            pthread_mutex_lock(&mgr->mutex);
			Resource* res = mgr->getWaitingRes();
			pthread_mutex_unlock(&mgr->mutex);

            ILoader* loader = res->getLoader();
			loader->load(res);

            mgr->waitLoads--;
            
        }
        else
        {
            usleep(80);
        }
    }
    
    return NULL;
}

pid_t LoaderThread::getPid()
{
    if(_pid == 0)
        _pid = getpid();
    return _pid;
}

FLAKOR_NS_END
