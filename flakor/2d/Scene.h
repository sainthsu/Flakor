/**
  * Copyright Saint Hsu (saint@aliyun.com)
  * /
#ifndef _FK_SCENE_H_
#define _FK_SCENE_H_

#include "2d/Entity.h"

FLAKOR_NS_BEGIN

class TouchTarget;

class Scene : public Entity
{
	protected:
                //The entity contained within this Scene that has or contains focus.
                Entity* focused;
                TouchTarget *firstTouchTarget;
	public:
		Scene();
		~Scene();
		static Scene* create();
                virtual bool  dispatchTouchTrigger(TouchTrigger* trigger);
                virtual bool onTouchTrigger(TouchTrigger* trigger);
};

FLAKOR_NS_END

#endif
