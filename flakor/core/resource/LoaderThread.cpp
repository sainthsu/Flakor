#ifndef _FK_LOADER_THREAD_H_
#define _FK_LOADER_THREAD_H_

#include "core/resource/LoaderThread.h"

FLAKOR_NS_BEGIN

LoaderThread::LoaderThread()
:pid(0)
,thread(NULL)
{}

LoaderThread::~LoaderThread()
{

}

LoaderThread* LoaderThread::create()
{
    LoaderThread thread = NULL;
    thread = new LoaderThread();

    return thread;
}

void start()
{
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&thread, &attr, LoadThread::run, NULL);
}

void LoaderThread::run(void)
{
    ResourceManager *mgr = ResourceManager::thisManager();

    for(;;)
    {
        if(mgr->waitLoads >= 1)
        {

            pthread_mutex_lock(mgr->mutex);
			Resource* res = mgr->getWaitingRes();
			res->load();

            mgr->waitLoads--;
            pthread_mutex_unlock(mgr->mutex);
        }
        else
        {
            sleep(80);
        }
    }
}


pid_t LoaderThread::getPid()
{
    if(pid == 0)
        pid = getpid();
    return pid;
}

#endif

FLAKOR_NS_END
