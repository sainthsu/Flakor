#include "MainGame.h"

using namespace flakor;

MainGame::MainGame()
:runningScene(nullptr)
{
}

MainGame::~MainGame()
{
}

void MainGame::create()
{
	FKLOG("main game create!");
    
    runningScene = TestScene::create();
    
}

void MainGame::resize(int width, int height)
{
        FKLOG("main game resize!");
}

void MainGame::update(float delta)
{
   FKLOG("main game update!");
    
    runningScene->update(delta);
}

void MainGame::render()
{
    FKLOG("main game render!");
    
    runningScene->onVisit();
}

void MainGame::pause()
{
    FKLOG("main game pause!");
}

void MainGame::resume()
{
    FKLOG("main game resume!");
}

void MainGame::dispose()
{
    FKLOG("main game dispose!");
}


bool MainGame::onTouch(TouchTrigger* touch)
{
     return runningScene->onTouchTrigger(touch);
}

