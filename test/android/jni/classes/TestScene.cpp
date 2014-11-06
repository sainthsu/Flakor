#include "TestScene.h"

bool TestScene::init()
{
	Sprite *sprite = Sprite::create("flakor_test.png");

    // position the sprite on the center of the screen
    sprite->setPosition(PointMake(200,200));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);

	return true;
}
