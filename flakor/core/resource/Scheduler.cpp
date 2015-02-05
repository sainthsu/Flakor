#include "targetMacros.h"
#include "core/resource/Scheduler.h"
#include "core/resource/Resource.h"

FLAKOR_NS_BEGIN

static Scheduler* sch = NULL;

Scheduler::Scheduler()
{
	_queue = new std::queue<Resource*>();
}

Scheduler::~Scheduler()
{
	delete(_queue);
}


Scheduler* Scheduler::thisScheduler()
{
	if (sch == NULL) {
        sch = new Scheduler();
    }
    
    return sch;
}

void Scheduler::update(float delta)
{
	Resource* res = NULL;
	for(int i=0;i < _queue->size(); i++)
	{
		res = _queue->front();
		res->doCallback();
		_queue->pop();
	}
    
}

void Scheduler::schedule(Resource* res)
{
	_queue->push(res);
}
	
FLAKOR_NS_END

