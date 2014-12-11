#include "targetMacros.h"
#include "base/entity/Scene.h"

FLAKOR_NS_BEGIN

Scene::Scene()
{
	relativeAnchorPoint = false;
	setAnchorPoint(MakePoint(0.5,0.5));
}

Scene::~Scene()
{
}


Scene* Scene::create()
{
	Scene *pRet = new Scene();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		FK_SAFE_DELETE(pRet);
		return NULL;
	}

}

FLAKOR_NS_END
