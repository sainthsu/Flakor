#include "MainGame.h"

using namespace flakor;

MainGame::MainGame()
:runningScene(nullptr)
{}

MainGame::~MainGame(){}

void MainGame::create()
{
    Matrix4 pMatrix = Matrix4::orthographic(540,800,-10, 10);
    GLMode(GL_PROJECTION);
    GLMultiply(&pMatrix);
    
    
    
    runningScene = new TestScene();
    LOGD("testscene out init");
    Sprite *sprite = Sprite::create("/storage/sdcard0/flakor_test.png");
    LOGD("testscene sprite");

    // position the sprite on the center of the screen
    sprite->setPosition(MakePoint(200,200));
    
    // add the sprite as a child to this layer
    runningScene->addChild(sprite, 0);
    
}

void MainGame::resize(int width, int height){}

void MainGame::render()
{
    LOGD("main game render!");
    
    runningScene->onVisit();
}

void MainGame::pause()
{
}

void MainGame::resume()
{
}

void MainGame::dispose()
{
}



