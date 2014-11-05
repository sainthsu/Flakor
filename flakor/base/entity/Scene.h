
#ifndef _FK_SCENE_H_
#define _FK_SCENE_H_

FLAKOR_NS_BEGIN
class Entity;

class Scene : public Entity
{
	public:
		Scene();
		~Scene();
		virtual bool init();
		static Scene* create();
};

FLAKOR_NS_END

#endif
