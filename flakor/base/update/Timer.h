#ifndef _FK_TIMER_H_
#define _FK_TIMER_H_

#include "base/interface/IUpdatable"


class Timer : public IUpdatable
{
protected:
    Timer();
public:
    /** get interval in seconds */
    inline float getInterval() const { return _interval; };
    /** set interval in seconds */
    inline void setInterval(float interval) { _interval = interval; };

    void setupTimerWithInterval(float seconds, unsigned int repeat, float delay);

    virtual void trigger() = 0;
    virtual void cancel() = 0;

    /** triggers the timer */
    void update(float dt) override;
    void reset() override;

protected:
    Scheduler* _scheduler; // weak ref
    float _elapsed;
    bool _runForever;
    bool _useDelay;
    unsigned int _timesExecuted;
    unsigned int _repeat; //0 = once, 1 is 2 x executed
    float _delay;
    float _interval;

};

#endif
