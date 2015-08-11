#include "TestScene.h"

using namespace flakor;

class MainGame : private Game
{
public:
    MainGame();
    virtual ~MainGame();
    
    virtual void create ();
    virtual void resize (int width, int height);
    
    virtual void update (float delta);
    virtual void render ();
    virtual void pause ();
    virtual void resume ();
    virtual void dispose ();
protected:
    Scene* runningScene;
};


