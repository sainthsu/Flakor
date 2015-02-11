#ifndef _FK_RUNNABLEHANDLER_H_
#define _FK_RUNNABLEHANDLER_H_

#include "base/interface/IUpdatable"


class RunnableHandler : public IUpdatable
{
public:
	Array* Runnables;

	void onUpdate(float secondsElapsed);
	void reset();
	void postRunnable(const Runnable* runnable);
};

#endif
