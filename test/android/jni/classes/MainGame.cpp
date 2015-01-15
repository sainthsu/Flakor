#include "MainGame.h"

using namespace flakor;

MainGame::MainGame()
:runningScene(nullptr)
{}

MainGame::~MainGame()
{
}

void MainGame::create()
{
	    LOGD("main game create!");
    Matrix4 pMatrix = Matrix4::orthographic(1080,1776,-1028, 1028);
    GLMode(GL_PROJECTION);
    GLMultiply(&pMatrix);
    
    runningScene = TestScene::create();
    
}

void MainGame::resize(int width, int height)
{
        LOGD("main game resize!");
}

void MainGame::render()
{
    LOGD("main game render!");
    
    runningScene->onVisit();
}

void MainGame::pause()
{
    LOGD("main game pause!");
}

void MainGame::resume()
{
    LOGD("main game resume!");
}

void MainGame::dispose()
{
    LOGD("main game dispose!");
}



