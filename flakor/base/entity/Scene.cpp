#include "./Scene.h"

Scene::Scene()
{
	relateAnchorPoint = false;
	setAnchorPoint(0.5,0.5);
}

Scene::~Scene()
{
}

bool Scene::init()
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
