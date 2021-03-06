#include <jni.h>

#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/resource.h>

#include <android/log.h>
#include "platform/android/Engine.h"
#include "platform/android/ActivityCallback.cpp"

#define TLOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "threaded_app", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "threaded_app", __VA_ARGS__))

/* For debug builds, always enable the debug traces in this library */
#ifndef NDEBUG
#  define LOGV(...)  ((void)__android_log_print(ANDROID_LOG_VERBOSE, "threaded_app", __VA_ARGS__))
#else
#  define LOGV(...)  ((void)0)
#endif

FLAKOR_NS_BEGIN

//process input event
static void process_input(Application* app, struct android_poll_source* source) {
    AInputEvent* event = NULL;
    while (AInputQueue_getEvent(app->inputQueue, &event) >= 0)
    {
        LOGV("New input event: type=%d\n", AInputEvent_getType(event));
        if (AInputQueue_preDispatchEvent(app->inputQueue, event)) {
            continue;
        }
        int32_t handled = 0;
        handled = app->onInputEvent(event);
        AInputQueue_finishEvent(app->inputQueue, event, handled);
    }
}

//process activity state cmd
static void process_cmd(Application* app, struct android_poll_source* source)
{
    int8_t cmd = app->readCmd();
    app->preExecCmd(cmd);
    app->onExecCmd(cmd);
    app->postExecCmd(cmd);
}

int8_t Application::readCmd()
{
    int8_t cmd;
    if (read(this->msgread, &cmd, sizeof(cmd)) == sizeof(cmd))
    {
        switch (cmd)
        {
            case APP_CMD_SAVE_STATE:
                this->freeSavedState();
                break;
        }
        return cmd;
    }
    else
    {
        LOGE("No data on command pipe!");
    }
    return -1;
}

void Application::preExecCmd(int8_t cmd)
{
    switch (cmd)
    {
        case APP_CMD_INPUT_CHANGED:
            LOGV("APP_CMD_INPUT_CHANGED\n");
            pthread_mutex_lock(&this->mutex);
            if (this->inputQueue != NULL) {
                AInputQueue_detachLooper(this->inputQueue);
            }
            
            this->inputQueue = this->pendingInputQueue;
            if (this->inputQueue != NULL)
            {
                LOGV("Attaching input queue to looper");
                AInputQueue_attachLooper(this->inputQueue,
                                         this->looper, LOOPER_ID_INPUT, NULL,
                                         &this->inputPollSource);
            }
            pthread_cond_broadcast(&this->cond);
            pthread_mutex_unlock(&this->mutex);
            break;
            
        case APP_CMD_INIT_WINDOW:
            LOGV("APP_CMD_INIT_WINDOW\n");
            pthread_mutex_lock(&this->mutex);
            this->window = this->pendingWindow;
            pthread_cond_broadcast(&this->cond);
            pthread_mutex_unlock(&this->mutex);
            break;
            
        case APP_CMD_TERM_WINDOW:
            LOGV("APP_CMD_TERM_WINDOW\n");
            pthread_cond_broadcast(&this->cond);
            break;
            
        case APP_CMD_RESUME:
        case APP_CMD_START:
        case APP_CMD_PAUSE:
        case APP_CMD_STOP:
            LOGV("activityState=%d\n", cmd);
            pthread_mutex_lock(&this->mutex);
            this->activityState = cmd;
            pthread_cond_broadcast(&this->cond);
            pthread_mutex_unlock(&this->mutex);
            break;
            
        case APP_CMD_CONFIG_CHANGED:
            LOGV("APP_CMD_CONFIG_CHANGED\n");
            AConfiguration_fromAssetManager(this->config,
                                            this->activity->assetManager);
            this->printConfig();
            break;
            
        case APP_CMD_DESTROY:
            LOGV("APP_CMD_DESTROY\n");
            this->destroyRequested = 1;
            break;
    }
}

void Application::onExecCmd(int32_t cmd)
{
    if(this->engine != NULL)
    {
        this->engine->handleCMD(cmd);
        return;
    }
    /*
    switch (cmd)
    {
        case APP_CMD_SAVE_STATE:
            // The system has asked us to save our current state.  Do so.
            this->engine->saveState(&this->app->savedState,
                            &this->app->savedStateSize);
            
            break;
        case APP_CMD_INIT_WINDOW:
            // The window is being shown, get it ready.
            if (this->app->window != NULL) {
                this->engine->game = Game::thisGame();
                this->engine->initDisplay();
                LOGW("game create!!!");
                ResourceManager::setAssetManager(this->app->activity->assetManager);
                this->game->create();
                this->drawFrame();
                this->state = STATE_RUNNING;
                updateThread->start();
            }
            break;
        case APP_CMD_RESUME:
            // Wait for first update is done.
            
            break;
        case APP_CMD_PAUSE:
            this->state = STATE_STOP;
            this->game->pause();
            break;
        case APP_CMD_TERM_WINDOW:
            // The window is being hidden or closed, clean it up.
            this->termDisplay();
            hasFocus = false;
            break;
        case APP_CMD_GAINED_FOCUS:
            // When our app gains focus, we start monitoring the accelerometer.
            if (this->accelerometerSensor != NULL)
            {
                ASensorEventQueue_enableSensor(this->sensorEventQueue,
                                               this->accelerometerSensor);
                // We'd like to get 60 events per second (in us).
                ASensorEventQueue_setEventRate(this->sensorEventQueue,
                                               this->accelerometerSensor, (1000L/60)*1000);
            }
            hasFocus = true;
            break;
        case APP_CMD_LOST_FOCUS:
            // When our app loses focus, we stop monitoring the accelerometer.
            // This is to avoid consuming battery while not being used.
            if (this->accelerometerSensor != NULL) {
                ASensorEventQueue_disableSensor(this->sensorEventQueue,
                                                this->accelerometerSensor);
            }
            // Also stop animating.
            hasFocus = false;
            this->state = STATE_STOP;
            this->drawFrame();
            
            break;
        case APP_CMD_LOW_MEMORY:
            //Free up GL resources
            this->trimMemory();
            break;
    }*/
}

void Application::postExecCmd(int8_t cmd)
{
    switch (cmd)
    {
        case APP_CMD_TERM_WINDOW:
            LOGV("APP_CMD_TERM_WINDOW\n");
            pthread_mutex_lock(&this->mutex);
            this->window = NULL;
            pthread_cond_broadcast(&this->cond);
            pthread_mutex_unlock(&this->mutex);
            break;
            
        case APP_CMD_SAVE_STATE:
            LOGV("APP_CMD_SAVE_STATE\n");
            pthread_mutex_lock(&this->mutex);
            this->stateSaved = 1;
            pthread_cond_broadcast(&this->cond);
            pthread_mutex_unlock(&this->mutex);
            break;
            
        case APP_CMD_RESUME:
            this->freeSavedState();
            break;
    }
}

int32_t Application::onInputEvent(AInputEvent* event)
{
    if (this->engine != NULL)
    {
        int32_t eventType = AInputEvent_getType(event);
        if (eventType == AINPUT_EVENT_TYPE_MOTION)
        {
            int pointerNumber = AMotionEvent_getPointerCount( event );

            intptr_t ids[pointerNumber];
            float xs[pointerNumber];
            float ys[pointerNumber];


            for (int i = 0; i < pointerNumber; i++) {
                ids[i] = (intptr_t)AMotionEvent_getPointerId(event, i );

                xs[i] = AMotionEvent_getX( event, i );
                ys[i] = AMotionEvent_getY( event, i );;
            }

            int32_t action = AMotionEvent_getAction(event);
            unsigned int flags = action & AMOTION_EVENT_ACTION_MASK;
            switch( flags )
            {
                case AMOTION_EVENT_ACTION_POINTER_DOWN:
                    {
                    int indexPointerDown = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK)
                            >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
                    intptr_t idPointerDown = AMotionEvent_getPointerId(event,indexPointerDown);
                    float xPointerDown = AMotionEvent_getX(event,indexPointerDown);
                    float yPointerDown = AMotionEvent_getY(event,indexPointerDown);
                    LOGV("POINTER_DOWN\n");

                    return this->engine->handleTouch(TouchTrigger::TouchAction::DOWN,1,&idPointerDown,&xPointerDown,&yPointerDown);
                    }
                case AMOTION_EVENT_ACTION_DOWN:
                    {
                    intptr_t idDown = ids[0];
                    float xDown = xs[0];
                    float yDown = ys[0];
                    LOGV("DOWN\n");
                    return this->engine->handleTouch(TouchTrigger::TouchAction::DOWN,1,&idDown,&xDown,&yDown);

                    }
                case AMOTION_EVENT_ACTION_POINTER_UP:
                    {
                    int indexUp = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK)
                            >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
                    intptr_t idUp = AMotionEvent_getPointerId(event,indexUp);
                    float xUp = AMotionEvent_getX(event,indexUp);
                    float yUp = AMotionEvent_getY(event,indexUp);
                    LOGV("POINTER_UP\n");

                    return this->engine->handleTouch(TouchTrigger::TouchAction::UP,1,&idUp,&xUp,&yUp);
                    }
                case AMOTION_EVENT_ACTION_UP:
                    {
                    int64_t eventTime = AMotionEvent_getEventTime(event);
                    int64_t downTime = AMotionEvent_getDownTime(event);

                    intptr_t idUp = ids[0];
                    float xUp = xs[0];
                    float yUp = ys[0];
                    LOGV("UP\n");
                    return this->engine->handleTouch(TouchTrigger::TouchAction::UP,1,&idUp,&xUp,&yUp);
                    }
                case AMOTION_EVENT_ACTION_MOVE:
                LOGV("MOVE\n");
                    return this->engine->handleTouch(TouchTrigger::TouchAction::MOVE,pointerNumber,ids,xs,ys);
                case AMOTION_EVENT_ACTION_CANCEL:
                LOGV("CANCEL\n");
                    return this->engine->handleTouch(TouchTrigger::TouchAction::CANCEL,pointerNumber,ids,xs,ys);
            }


        }
        else if(eventType == AINPUT_EVENT_TYPE_KEY) //消息来源于物理键盘或虚拟键盘
        {
            //return this->engine->handleKey(event);
        }
        
    }
    return 0;
}

void Application::freeSavedState()
{
    pthread_mutex_lock(&this->mutex);
    if (this->savedState != NULL)
    {
        free(this->savedState);
        this->savedState = NULL;
        this->savedStateSize = 0;
    }
    pthread_mutex_unlock(&this->mutex);
}

void Application::destroy()
{
    LOGV("android_app_destroy!");
    this->freeSavedState();
    pthread_mutex_lock(&this->mutex);
    if (this->inputQueue != NULL) {
        AInputQueue_detachLooper(this->inputQueue);
    }
    AConfiguration_delete(this->config);

    delete(this->engine);
    this->destroyed = 1;
    pthread_cond_broadcast(&this->cond);
    pthread_mutex_unlock(&this->mutex);
    
    // Can't touch android_app object after this.
}

//run on main thread
void Application::writeCmd(int8_t cmd)
{
    if (write(this->msgwrite, &cmd, sizeof(cmd)) != sizeof(cmd)) {
        LOGE("Failure writing android application cmd: %s\n", strerror(errno));
    }
}
//run on main thread
void Application::setInput(AInputQueue* inputQueue)
{
    pthread_mutex_lock(&this->mutex);
    this->pendingInputQueue = inputQueue;
    this->writeCmd(APP_CMD_INPUT_CHANGED);
    
    while (this->inputQueue != this->pendingInputQueue)
    {
        pthread_cond_wait(&this->cond, &this->mutex);
    }
    pthread_mutex_unlock(&this->mutex);
}

//run one main thread
void Application::setWindow(ANativeWindow* window)
{
    pthread_mutex_lock(&this->mutex);
    if (this->pendingWindow != NULL)
    {
        this->writeCmd(APP_CMD_TERM_WINDOW);
    }
    this->pendingWindow = window;
    if (window != NULL)
    {
        this->writeCmd(APP_CMD_INIT_WINDOW);
    }
    while (this->window != this->pendingWindow) {
        pthread_cond_wait(&this->cond, &this->mutex);
    }
    pthread_mutex_unlock(&this->mutex);
}

//run on main thread
void Application::setActivityState(int8_t cmd)
{
    pthread_mutex_lock(&this->mutex);
    this->writeCmd(cmd);
    while (this->activityState != cmd)
    {
        pthread_cond_wait(&this->cond, &this->mutex);
    }
    pthread_mutex_unlock(&this->mutex);
}

//run on main thread
void Application::freePipe()
{
    pthread_mutex_lock(&this->mutex);
    this->writeCmd(APP_CMD_DESTROY);
    //wait for child thread to do destroy action
    while (!this->destroyed)
    {
        pthread_cond_wait(&this->cond, &this->mutex);
    }
    pthread_mutex_unlock(&this->mutex);
    
    close(this->msgread);
    close(this->msgwrite);
    pthread_cond_destroy(&this->cond);
    pthread_mutex_destroy(&this->mutex);

    delete(this);
}

// 参考：http://zh.wikipedia.org/wiki/%E7%A7%BB%E5%8A%A8%E8%AE%BE%E5%A4%87%E7%BD%91%E7%BB%9C%E4%BB%A3%E7%A0%81
//MCC：Mobile Country Code，移动国家码，MCC的资源由国际电联（ITU）统一分配和管理，唯一识别移动用户所属的国家，共3位，中国为460;
//MNC（Mobile Network Code，移动网络号码），用于识别移动客户所属的移动网络。由二个十进制数组成，编码范围为十进制的00－99，例如中国移动的MNC为00、02和07 ，中国联通的MNC为01、06，中国电信的MNC为03,05.中国铁通为20。
void Application::printConfig()
{
    char lang[2], country[2];
    AConfiguration_getLanguage(this->config, lang);
    AConfiguration_getCountry(this->config, country);
    
    LOGV("Config: mcc=%d mnc=%d lang=%c%c cnt=%c%c orien=%d touch=%d dens=%d "
         "keys=%d nav=%d keysHid=%d navHid=%d sdk=%d size=%d long=%d "
         "modetype=%d modenight=%d",
         AConfiguration_getMcc(this->config),
         AConfiguration_getMnc(this->config),
         lang[0], lang[1], country[0], country[1],
         AConfiguration_getOrientation(this->config),
         AConfiguration_getTouchscreen(this->config),
         AConfiguration_getDensity(this->config),
         AConfiguration_getKeyboard(this->config),
         AConfiguration_getNavigation(this->config),
         AConfiguration_getKeysHidden(this->config),
         AConfiguration_getNavHidden(this->config),
         AConfiguration_getSdkVersion(this->config),
         AConfiguration_getScreenSize(this->config),
         AConfiguration_getScreenLong(this->config),
         AConfiguration_getUiModeType(this->config),
         AConfiguration_getUiModeNight(this->config));
} 

// --------------------------------------------------------------------
// Native activity interaction (called from main thread)
// --------------------------------------------------------------------
Application* Application::create(ANativeActivity* activity,
                                 void* savedState, size_t savedStateSize)
{
	
    Application* app = new Application();
    app->activity = activity;

    activity->callbacks->onStart = onStart;
    activity->callbacks->onResume = onResume;
    activity->callbacks->onSaveInstanceState = onSaveInstanceState;
    activity->callbacks->onPause = onPause;
    activity->callbacks->onStop = onStop;
    activity->callbacks->onDestroy = onDestroy;
    
    activity->callbacks->onWindowFocusChanged = onWindowFocusChanged;
    activity->callbacks->onNativeWindowCreated = onNativeWindowCreated;
    activity->callbacks->onNativeWindowResized = onNativeWindowResized;
    activity->callbacks->onNativeWindowRedrawNeeded = onNativeWindowRedrawNeeded;
    activity->callbacks->onNativeWindowDestroyed = onNativeWindowDestroyed;
    
    activity->callbacks->onInputQueueCreated = onInputQueueCreated;
    activity->callbacks->onInputQueueDestroyed = onInputQueueDestroyed;
    
    activity->callbacks->onContentRectChanged = onContentRectChanged;
    
    activity->callbacks->onConfigurationChanged = onConfigurationChanged;
    activity->callbacks->onLowMemory = onLowMemory;
    
    
    pthread_mutex_init(&app->mutex, NULL);
    pthread_cond_init(&app->cond, NULL);
    
    if (savedState != NULL)
    {
        app->savedState = malloc(savedStateSize);
        app->savedStateSize = savedStateSize;
        memcpy(app->savedState, savedState, savedStateSize);
    }
    
    int msgpipe[2];
    if (pipe(msgpipe))
    {
        LOGE("could not create pipe: %s", strerror(errno));
        return NULL;
    }
    
    app->msgread = msgpipe[0];
    app->msgwrite = msgpipe[1];
    
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&app->thread, &attr, Application::threadEntry, app);
    
    // Wait for thread to start.
    pthread_mutex_lock(&app->mutex);
    while (!app->running) {
        pthread_cond_wait(&app->cond, &app->mutex);
    }
    pthread_mutex_unlock(&app->mutex);

    return app;
}

/* new thread entry */
void* Application::threadEntry(void* param)
{
    Application* app = (Application*)param;
    
    app->config = AConfiguration_new();
    AConfiguration_fromAssetManager(app->config, app->activity->assetManager);
    
    //print config info
    app->printConfig();
    
    /*
     *设置主循环和输入命令控制
     */
    app->cmdPollSource.id = LOOPER_ID_MAIN;
    app->cmdPollSource.app = app;
    app->cmdPollSource.process = process_cmd;
    app->inputPollSource.id = LOOPER_ID_INPUT;
    app->inputPollSource.app = app;
    app->inputPollSource.process = process_input;
    
    ALooper* looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
    ALooper_addFd(looper, app->msgread, LOOPER_ID_MAIN, ALOOPER_EVENT_INPUT, NULL,
                  &app->cmdPollSource);
    app->looper = looper;
    
    pthread_mutex_lock(&app->mutex);
    app->running = 1;
    pthread_cond_broadcast(&app->cond);
    pthread_mutex_unlock(&app->mutex);
    
    app->main();
    app->destroy();
    return NULL;
}

/**
 * This is the main entry point of a native application that is using
 *  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void Application::main()
{
    
    Engine *engine = new Engine();
    // Make sure glue isn't stripped.
    this->dummy();
    
    this->engine = engine;
    this->userData = NULL;
    
    //app->onAppCmd = engine_handle_cmd;
    //app->onInputEvent = engine_handle_input;
	
    engine->app = this;
    
    // Prepare to monitor accelerometer
    engine->sensorManager = ASensorManager_getInstance();
    engine->accelerometerSensor = ASensorManager_getDefaultSensor(engine->sensorManager,
                                                                  ASENSOR_TYPE_ACCELEROMETER);
    engine->sensorEventQueue = ASensorManager_createEventQueue(engine->sensorManager,
                                                               this->looper, LOOPER_ID_USER, NULL, NULL);
    
    if (this->savedState != NULL) {
        // We are starting with a previous saved state; restore from it.
        engine->initFromState(this->savedState,this->savedStateSize);
    }
    
    // loop waiting for stuff to do.
    
    while (1)
    {
        // Read all pending events.
        int ident;
        int events;
        struct android_poll_source* source;
        
        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        while ((ident=ALooper_pollAll(engine->state==STATE_RUNNING ? 0 : -1, NULL, &events,
                                      (void**)&source)) >= 0)
        {
            
            // Process this event.
            if (source != NULL) {
                source->process(this, source);
            }
            
            // If a sensor has data, process it now.
            if (ident == LOOPER_ID_USER)
            {
                if (engine->accelerometerSensor != NULL)
                {
                    ASensorEvent event;
                    while (ASensorEventQueue_getEvents(engine->sensorEventQueue,
                                                       &event, 1) > 0)
                    {
                        /*LOGE("accelerometer: x=%f y=%f z=%f",
                             event.acceleration.x, event.acceleration.y,
                             event.acceleration.z);*/
                    }
                }
            }
            
            // Check if we are exiting.
            //if destroy requested,stop render
            if (this->destroyRequested != 0) 
			{
                engine->termDisplay();
                return;
            }
        }
        
        if (engine->isFocus())
        {
            // Done with events; draw next animation frame.
           	
            // Drawing is throttled to the screen update rate, so there
            // is no need to do timing here.
            engine->drawFrame();
        }
    }
}

void Application::dummy()
{
    
}

FLAKOR_NS_END
