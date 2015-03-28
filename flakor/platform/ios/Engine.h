//
//  Engine.h
//  IOS
//
//  Created by Saint Hsu on 18/3/15.
//  Copyright (c) 2015 Saint Hsu. All rights reserved.
//

#ifndef _FK_ENGINE_H_
#define _FK_ENGINE_H_

#include "core/input/TouchTrigger.h"

FLAKOR_NS_BEGIN

class Scene;
class Application;
class Game;
class Scheduler;
class GLContext;
class UpdateThread;
class TouchPool;

enum EngineState {
    STATE_INITAL,
    STATE_RUNNING,
    STATE_STOP,
    STATE_DESTROYED
};


class Engine
{
protected:
    Application* app;
    Game* game;
    GLContext *glContext;
    
    pthread_mutex_t mutex;//draw lock
    
    enum EngineState state;
    struct timeval* lastTick;
    /* delta time since last tick to main loop */
    float deltaTime;
    /* whether or not the next delta time will be zero */
    bool nextDeltaTimeZero;
    
    /* How many frames were updated since the engine started */
    unsigned int totalUpdated;
    /* How many frames were drawed since the engine started */
    unsigned int totalFrames;
    float secondsPerFrame;
    
    Scheduler* schedule;
    TouchPool* touchPool;
    UpdateThread* updateThread;
    
public:
    Engine();
    ~Engine();
    
    static Engine* getInstance();
    
    void run();
    void onTickUpdate();
    void drawFrame(void);
    
    void setGLContext(GLContext* context) {glContext = context;};
    GLContext* getGLContext(){return glContext;};
    void setGame(Game* game);
    
    void saveState(void **savedState,size_t *size);
    void initFromState(void *savedState,size_t size);
    
    void termDisplay(void);
    void trimMemory();
    void destroy(void);
    
    void updateViewport();
    
    int32_t handleTouch(TouchTrigger::TouchAction action,int count,intptr_t ids[],float xs[],float ys[]);
    
protected:
    /** calculates delta time since last time it was called */
    void calculateDeltaTime();
private:
    int initDisplay(void);
};

FLAKOR_NS_END

#endif
