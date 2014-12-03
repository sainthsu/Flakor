#include "TestScene.h"

#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,"testscene",__VA_ARGS__)

bool TestScene::init()
{
    LOGD("testscene init");
	Sprite *sprite = Sprite::create("/storage/sdcard/flakor_test.png");

    // position the sprite on the center of the screen
    sprite->setPosition(MakePoint(200,200));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);

    Matrix4 pMatrix = Matrix4::orthographic(1080,1600,30,100);
    GLMode(GL_PROJECTION);
    GLMultiply(&pMatrix);
    GLGet(GL_PROJECTION,&pMatrix);
    
	return true;
}
