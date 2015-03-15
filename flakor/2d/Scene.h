
#ifndef _FK_SCENE_H_
#define _FK_SCENE_H_

#include "2d/Entity.h"

FLAKOR_NS_BEGIN

class Scene : public Entity
{
	public:
		Scene();
		~Scene();
		static Scene* create();
        virtual bool onTouchTrigger(TouchTrigger* trigger);
};

FLAKOR_NS_END

#endif
