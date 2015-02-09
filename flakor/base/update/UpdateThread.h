#ifndef _FK_UPDATE_THREAD_H_
#define _FK_UPDATE_THREAD_H_

#include <sys/types.h>
#include <pthread.h>


FLAKOR_NS_BEGIN

class Engine;

class UpdateThread
{
    protected:
        pid_t _pid;
        pthread_t _thread;
        bool _running;

		Engine* _engine;
    public:
        UpdateThread();
        ~UpdateThread();

        static UpdateThread* create(Engine* engine);
        static void* run(void* thread);

		void start();
        pid_t getPid();
		inline void setEngine(Engine* engine) {_engine = engine;};
		inline Engine* getEngine(){return _engine;};
        inline bool isRunning() {return _running;};
        inline void setRun(bool run) {_running = run;};

};

FLAKOR_NS_END

#endif
