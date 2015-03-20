#include "macros.h"

#if FK_TARGET_PLATFORM == FK_PLATFORM_ANDROID
#include "platform/android/Engine.h"
#elif FK_TARGET_PLATFORM == FK_PLATFORM_IOS
#include "platform/ios/Engine.h"
#endif

#include "base/update/UpdateThread.h"

#include <unistd.h>

FLAKOR_NS_BEGIN

UpdateThread::UpdateThread()
:_pid(0)
,_thread(0)
,_running(false)
,_inited(false)
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
	if(_inited)
    {
		return;
	}
	pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    int err = pthread_create(&_thread, &attr, UpdateThread::main, this);
	if(err == 0) _inited = true;
}

void* UpdateThread::main(void* thread)
{
    UpdateThread* thr = (UpdateThread*)thread;
    
    thr->run();
    return NULL;
}

void UpdateThread::run()
{
	_running = true;

    while(_running)
    {
		_engine->onTickUpdate();
    }
}

pid_t UpdateThread::getPid()
{
    if(_pid == 0)
        _pid = getpid();
    return _pid;
}

FLAKOR_NS_END
