/**
  * Copyright Saint Hsu (saint@aliyun.com)
  */
#ifndef _FK_SCENE_H_
#define _FK_SCENE_H_

#include "2d/Entity.h"

FLAKOR_NS_BEGIN

class TouchTarget;

class Scene : public Entity
{
	public:
		Scene();
		~Scene();
		static Scene* create();
        virtual bool  dispatchTouchTrigger(TouchTrigger* trigger) override;
        virtual bool  onInterceptTouchTrigger(TouchTrigger* trigger);
        virtual bool onTouchTrigger(TouchTrigger* trigger) override;
};

FLAKOR_NS_END

#endif
