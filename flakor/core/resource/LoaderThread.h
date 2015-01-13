#ifndef _FK_LOADER_THREAD_H_
#define _FK_LOADER_THREAD_H_

#include <pthread.h>

FLAKOR_NS_BEGIN

class LoaderThread
{
    protected:
        pid_t _pid;
        pthread_t _thread;
    public:
        LoaderThread();
        ~LoaderThread();

        static LoaderThread* create();
        static void* run(void* param);

		void start();
        pid_t getPid();

};

#endif

FLAKOR_NS_END
