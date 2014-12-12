#include "macros.h"
#include "Game.h"

FLAKOR_NS_BEGIN

Game *Game::game = 0;

Game::Game()
{
	FKAssert(! game, "");
    game = this;
}

Game::~Game()
{
	FKAssert(! game, "");
    game = NULL;
}

Game* Game::thisGame()
{
	FKAssert(! game, "");
    return game;
}

FLAKOR_NS_END
