#ifndef _FK_ONTOUCHEVENT_H_
#define _FK_ONTOUCHEVENT_H_

class Entity;
class TouchTrigger;

FLAKOR_NS_BEGIN

class OnTouchEvent
{
public:
  OnTouchEvent();
  ~OnTouchEvent();

  virtual bool onTouch(Entity* entity,TouchTrigger* trigger);
};

FLAKOR_NS_END

#endif // _FK_ONTOUCHEVENT_H_
