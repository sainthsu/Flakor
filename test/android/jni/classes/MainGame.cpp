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
    Matrix4 pMatrix = Matrix4::orthographic(1080,1776,-1028, 1028);
    GLMode(GL_PROJECTION);
    GLMultiply(&pMatrix);
    
    runningScene = TestScene::create();
    
}

void MainGame::resize(int width, int height)
{
    
}

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



