#ifndef _FK_LOADER_THREAD_H_
#define _FK_LOADER_THREAD_H_

#include <sys/types.h>
#include <pthread.h>


FLAKOR_NS_BEGIN

class LoaderThread
{
    protected:
        pid_t _pid;
        pthread_t _thread;
        bool _running;
    public:
        LoaderThread();
        ~LoaderThread();

        static LoaderThread* create();
        static void* run(void* thread);

		void start();
        pid_t getPid();
        inline bool isRunning() {return _running;};
        inline void setRun(bool run) {_running = run;};

};

#endif

FLAKOR_NS_END
