#include <android/log.h>

#include "classes/TestScene.h"

#define  LOG_TAG    "MainGame"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

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

    virtual bool onTouch(TouchTrigger* touch);
protected:
    Scene* runningScene;
};


