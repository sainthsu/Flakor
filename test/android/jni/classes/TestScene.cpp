#include "TestScene.h"

#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,"testscene",__VA_ARGS__)

bool TestScene::init()
{
    LOGD("testscene init");
	Sprite *sprite = Sprite::create("asset://flakor_test.png");

    // position the sprite on the center of the screen
    sprite->setPosition(PointMake(200,200));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
	return true;
}

TestScene* TestScene::create()
{
	TestScene *pRet = new TestScene();
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
