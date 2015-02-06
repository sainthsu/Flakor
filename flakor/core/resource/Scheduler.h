#ifndef _FK_SCHEDULER_H_
#define _FK_SCHEDULER_H_

#include <queue>

FLAKOR_NS_BEGIN

class Resource;

class Scheduler
{
	public:
		Scheduler();
		~Scheduler();

		static Scheduler* thisScheduler();

		void update(float delta);//in GL thread

		void schedule(Resource* res);//in load thread
	protected:
		std::queue<Resource*>* _queue;
};

FLAKOR_NS_END

#endif
