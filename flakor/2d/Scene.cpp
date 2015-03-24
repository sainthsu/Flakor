#include "targetMacros.h"
#include "2d/Scene.h"
#include "core/input/TouchTarget.h"

FLAKOR_NS_BEGIN

Scene::Scene()
{
	relativeAnchorPoint = false;
	setAnchorPoint(PointMake(0.5,0.5));
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

bool Scene::dispatchTouchTrigger(TouchTrigger* trigger)
{
    return false;
}

bool Scene::onTouchTrigger(TouchTrigger* trigger)
{
    return true;
}

FLAKOR_NS_END
