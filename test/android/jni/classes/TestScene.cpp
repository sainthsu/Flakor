#include "TestScene.h"

#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,"testscene",__VA_ARGS__)

bool TestScene::init()
{
    LOGD("testscene init");

	TestScene* s = this;
	Image* image = dynamic_cast<Image*>(ResourceManager::thisManager()->createResource("asset://flakor_test.png",ResourceManager::IMAGE_NAME));
	image->setCallback([=](Resource*){
		Texture2D* tex = new Texture2D();
		tex->initWithImage(image);
    	Sprite *sprite = Sprite::createWithTexture(tex); 
    	sprite->setPosition(PointMake(200,500));
	    s->addChild(sprite, 1);
	});
	image->load(true);
	
    
    Sprite *logo = Sprite::create("asset://flakor_logo.png");
    
    // position the sprite on the center of the screen
    logo->setPosition(PointMake(200,20));
    
    // add the sprite as a child to this layer
    this->addChild(logo,2);
    
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
