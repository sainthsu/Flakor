#ifndef _FK_ONTOUCHEVENT_H_
#define _FK_ONTOUCHEVENT_H_

class Entity;
class TouchTrigger;

class OnTouchEvent
{
public:
  OnTouchEvent();
  ~OnTouchEvent();

  virtual onTouch(Entity* entity,TouchTrigger* trigger);
};

#endif // _FK_ONTOUCHEVENT_H_
