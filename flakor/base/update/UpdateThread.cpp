#include "macros.h"
#include "platform/android/Engine.h"
#include "base/update/UpdateThread.h"

#include <unistd.h>

FLAKOR_NS_BEGIN

UpdateThread::UpdateThread()
:_pid(0)
,_thread(0)
,_running(false)
,_engine(NULL)
{}

UpdateThread::~UpdateThread()
{
	_running = false;
}

UpdateThread* UpdateThread::create(Engine* engine)
{
    UpdateThread* thread = NULL;
    thread = new UpdateThread();
	thread->setEngine(engine);

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
    
	Engine* engine = thr->getEngine();
	engine->firstUpdate();

    while(thr->isRunning())
    {
		if(engine->updated && !engine->drawed)
		{
			FKLOG("updateThread clear memory!!!");
			continue;
		}

		engine->onTickUpdate();
		pthread_mutex_lock(&engine->mutex);
    	engine->updated = true;
    	pthread_mutex_unlock(&engine->mutex);
		
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
