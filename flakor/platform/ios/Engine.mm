//
//  Engine.cpp
//  flakor_lib
//
//  Created by Saint Hsu on 19/3/15.
//  Copyright (c) 2015 Saint Hsu. All rights reserved.
//

#include "platform/ios/Engine.h"
#include "platform/Game.h"
#include "platform/ios/GLContext.h"
#include "core/resource/Scheduler.h"
#include "core/resource/ResourceManager.h"
#include "core/input/TouchPool.h"
#include "base/update/UpdateThread.h"
#include "math/GLMatrix.h"
#import "platform/ios/DrawCaller.h"

FLAKOR_NS_BEGIN

static Engine* mainEngine = NULL;

Engine::Engine()
:state(STATE_INITAL)
,totalUpdated(0)
,totalFrames(0)
{
    lastTick = new struct timeval;
    schedule = Scheduler::thisScheduler();
    //updateThread = UpdateThread::create(this);
    //pthread_mutex_init(&mutex, NULL);
    touchPool = new TouchPool();
    game = Game::thisGame();
}

Engine::~Engine()
{
    FK_SAFE_DELETE(updateThread);
    FK_SAFE_DELETE(schedule);
    FK_SAFE_DELETE(touchPool);
}

Engine* Engine::getInstance()
{
    if (mainEngine == NULL) {
        mainEngine = new Engine();
    }
    
    return mainEngine;
}

void Engine::setGame(Game* game)
{
    this->game = game;
}

void Engine::run()
{
    game->create();
    updateViewport();
    [[DrawCaller sharedDirectorCaller] startMainLoop];
}

//run in update thread
void Engine::onTickUpdate()
{
    //FKLOG("updateThread :frame:%d;update:%d",totalFrames,totalUpdated);
    if(totalFrames < totalUpdated)
    {
        //FKLOG("updateThread schedule update!!!");
        return;
    }
    
    this->calculateDeltaTime();
    // skip one flame when _deltaTime equal to zero.
    if(deltaTime < FLT_EPSILON)
    {
        return;
    }
    
    /*
    if (this->game != NULL)
    {
        //LOGW("Engine tickupdate!!!");
        TouchTrigger* trigger = touchPool->getCurrentTouch();
        if(trigger != NULL)
            game->onTouch(trigger);
        //game->update(deltaTime);
    }*/
    
    while(pthread_mutex_trylock(&mutex) == EBUSY)
    {
        //FKLOG("updateThread clear memory!!!");
    }
    
    //FKLOG("updateThread swap memory!!!");
    totalUpdated++;
    pthread_mutex_unlock(&mutex);
}

void Engine::updateViewport()
{
    int32_t width = glContext->getScreenWidth();
    int32_t height = glContext->getScreenHeight();
    
    //Note that screen size might have been changed
    glViewport( 0, 0, width, height );
    Matrix4 pMatrix = Matrix4::orthographic(width,height,-width/2, width/2);
    GLMode(GL_PROJECTION);
    GLMultiply(&pMatrix);
}

/**
* Just the current frame in the display.
*/
void Engine::drawFrame()
{
    FKLOG("DrawThread :frame:%d;update:%d",totalFrames,totalUpdated);
    /*if(totalFrames > totalUpdated)
    {
        schedule->update(deltaTime);
        return;
    }*/
    schedule->update(deltaTime);
    
    pthread_mutex_lock(&mutex);
    // Just clear the screen with a color.
    glClearColor(1.f, 1.f,1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    
    if (this->game != NULL)
    {
        
        this->game->render();
        totalFrames++;
    }
    
    // Swap
    glContext->swap();
    
    pthread_mutex_unlock(&mutex);
}

/**
 * Process the next input event.
 */
int32_t Engine::handleTouch(TouchTrigger::TouchAction action,int count,intptr_t ids[],float xs[],float ys[])
{
    return touchPool->handleTouch(action,count,ids,xs,ys);
}

void Engine::calculateDeltaTime()
{
    struct timeval now;
    
    if (gettimeofday(&now, NULL) != 0)
    {
        FKLOG("error in gettimeofday");
        deltaTime = 0;
        return;
    }
    
    // new delta time. Re-fixed issue #1277
    if (nextDeltaTimeZero)
    {
        deltaTime = 0;
        nextDeltaTimeZero = false;
    }
    else
    {
        deltaTime = (now.tv_sec - lastTick->tv_sec) + (now.tv_usec - lastTick->tv_usec) / 1000000.0f;
        deltaTime = MAX(0, deltaTime);
    }
    
#if FLAKOR_DEBUG
    // If we are debugging our code, prevent big delta time
    if (deltaTime > 0.2f)
    {
        deltaTime = 1 / 60.0f;
    }
#endif
    
    *lastTick = now;
}


FLAKOR_NS_END
