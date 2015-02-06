#include "targetMacros.h"
#include "base/update/UpdateThread.h"

#include <unistd.h>

FLAKOR_NS_BEGIN

UpdateThread::UpdateThread()
:_pid(0)
,_thread(0)
{}

UpdateThread::~UpdateThread()
{
}

UpdateThread* UpdateThread::create()
{
    UpdateThread* thread = NULL;
    thread = new UpdateThread();

    return thread;
}

void UpdateThread::start()
{
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&_thread, &attr, UpdateThread::run, this);
}

void* UpdateThread::run(void* thread)
{
    UpdateThread* thr = (UpdateThread*)thread;
    thr->setRun(true);
    
	thr->firstUpdate();

    while(thr->isRunning())
    {
       pthread_mutex_lock(&mgr->mutex);
	   Resource* res = mgr->getWaitingRes();
	   pthread_mutex_unlock(&mgr->mutex);
    }
    
    return NULL;
}

pid_t UpdateThread::getPid()
{
    if(_pid == 0)
        _pid = getpid();
    return _pid;
}

FLAKOR_NS_END
