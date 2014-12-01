#include "mainGame.h"

using namespace flakor;

MainGame::MainGame(){}
MainGame::~MainGame(){}

void MainGame::create ()
{
    runningScene = TestScene::create();
}

void MainGame::resize (int width, int height){}

void MainGame::render ()
{
    runningScene->onVisit();
}

void MainGame::pause ()
{
}

void MainGame::resume ()
{
}

void MainGame::dispose ()
{
}



