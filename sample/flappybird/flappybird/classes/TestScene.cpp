#include "TestScene.h"

bool TestScene::init()
{
	TestScene* s = this;
	Image* image = dynamic_cast<Image*>(ResourceManager::thisManager()->createResource("bundle://bg.png",ResourceManager::IMAGE_NAME));
	image->setCallback([=](Resource*){
		Texture2D* tex = new Texture2D();
		tex->initWithImage(image);
    	Sprite *sprite = Sprite::createWithTexture(tex);
    	sprite->setContentSize(SizeMake(2000,1800));
	    s->addChild(sprite, 1);
	});
	image->load(true);
	
    
    Sprite *logo = Sprite::create("bundle://bird.png");
    
    // position the sprite on the center of the screen
    logo->setPosition(PointMake(200,20));
    
    // add the sprite as a child to this layer
    this->addChild(logo,2);
    
    SimpleAudioEngine::getInstance()->playBackgroundMusic("bgm.mp3");
    
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
