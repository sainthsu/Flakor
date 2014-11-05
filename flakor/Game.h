#ifndef _FK_GAME_H_
#define _FK_GAME_H_
#include "base/interface/IGame.h"

FLAKOR_NS_BEGIN

Game *Game::game = 0;
class Game : public IGame
{
	public:
		Game();
		virtual ~Game();
		/**
    	@brief    Get current Game instance.
    	@return Current Game instance pointer.
    	*/
    	static Game* getGame();
	protected:
		static Game *game;
}

FLAKOR_NS_END

#endif
