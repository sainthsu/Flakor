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
    
    runningScene = TestScene::create();
    
}

void MainGame::resize(int width, int height)
{
        LOGD("main game resize!");
}

void MainGame::update(float delta)
{
    LOGD("main game update!");
    
    runningScene->update(delta);
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


bool MainGame::onTouch(TouchTrigger* touch)
{
     return runningScene->onTouchTrigger(touch);
}

