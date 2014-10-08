
#ifndef _FK_SCENE_H_
#define _FK_SCENE_H_

class Scene : public Entity
{
	public:
		Scene();
		~Scene();
		bool init();
		static Scene* create();
}

#endif
