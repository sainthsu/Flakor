#ifndef _FK_RUNNABLEHANDLER_H_
#define _FK_RUNNABLEHANDLER_H_

#include "base/interface/IUpadatabel"


class RunnableHandler : public IUpdatable
{
public:
	Array* Runnables;
	void onUpdate(final float secondsElapsed);
	void reset();
	void postRunnable(const Runnable* runnable);
};

#endif
