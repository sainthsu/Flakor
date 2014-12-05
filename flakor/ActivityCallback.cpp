/*****************************************************
 * Copyright (C) 2014 Saint Hsu （saint@aliyun.com）
 * www.flakor.org
 *****************************************************/

//BEGIN_INCLUDE
#include "macros.h"
#include "Application.h"


#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

#ifdef __cplusplus
extern "C" {
#endif

using namespace flakor;
/**
 * nativeActivity lifecycle
 */
static void onStart(ANativeActivity* activity)
{
    LOGI("Start: %p\n", activity);
    Application* app = (Application*)activity->instance;
    app->setActivityState(APP_CMD_START);
}

static void onPause(ANativeActivity* activity)
{
    LOGI("Pause: %p\n", activity);
    Application* app = (Application*)activity->instance;
    app->setActivityState(APP_CMD_PAUSE);
}

static void onStop(ANativeActivity* activity)
{
    LOGI("Stop: %p\n", activity);
    Application* app = (Application*)activity->instance;
    app->setActivityState(APP_CMD_STOP);
}

static void onResume(ANativeActivity* activity)
{
    LOGI("Resume: %p\n", activity);
    Application* app = (Application*)activity->instance;
    app->setActivityState(APP_CMD_RESUME);
}


static void onDestroy(ANativeActivity* activity) 
{
    LOGI("Destroy: %p\n", activity);
    Application* app = (Application*)activity->instance;
    app->free();
}

static void* onSaveInstanceState(ANativeActivity* activity, size_t* outLen)
{
    Application* app = (Application*)activity->instance;
    void* savedState = NULL;

    LOGI("SaveInstanceState: %p\n", activity);
    pthread_mutex_lock(&app->mutex);
    app->stateSaved = 0;
    app->writeCmd(APP_CMD_SAVE_STATE);
    
    while (!app->stateSaved) {
        pthread_cond_wait(&app->cond, &app->mutex);
    }

    if (app->savedState != NULL)
    {
        savedState = app->savedState;
        *outLen = app->savedStateSize;
        app->savedState = NULL;
        app->savedStateSize = 0;
    }

    pthread_mutex_unlock(&app->mutex);

    return savedState;
}

static void onWindowFocusChanged(ANativeActivity* activity, int focused)
{
    LOGI("WindowFocusChanged: %p -- %d\n", activity, focused);
    Application* app = (Application*)activity->instance;
    app->writeCmd(focused ? APP_CMD_GAINED_FOCUS : APP_CMD_LOST_FOCUS);
}

static void onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window)
{
    LOGI("NativeWindowCreated: %p -- %p\n", activity, window);
    Application* app = (Application*)activity->instance;

    app->setWindow(window);
}

static void onNativeWindowResized(ANativeActivity* activity, ANativeWindow* window)
{
	LOGI("NativeWindowResized: %p -- %p\n", activity, window);
}

static void onNativeWindowRedrawNeeded(ANativeActivity* activity, ANativeWindow* window)
{
	LOGI("NativeWindowCRedrawNeeded: %p -- %p\n", activity, window);
}

static void onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window)
{
    LOGI("NativeWindowDestroyed: %p -- %p\n", activity, window);
    Application* app = (Application*)activity->instance;

    app->setWindow(NULL);
}

static void onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue)
{
    LOGI("InputQueueCreated: %p -- %p\n", activity, queue);
    Application* app = (Application*)activity->instance;

    app->setInput(queue);
}

static void onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue)
{
    LOGI("InputQueueDestroyed: %p -- %p\n", activity, queue);
    Application* app = (Application*)activity->instance;
    
    app->setInput(NULL);
}

static void onContentRectChanged(ANativeActivity* activity, const ARect* rect)
{
	LOGI("ContentRectChanged: %p -- %p\n", activity, rect);
}

static void onConfigurationChanged(ANativeActivity* activity)
{
    Application* app = (Application*)activity->instance;
    LOGI("ConfigurationChanged: %p\n", activity);
    app->writeCmd(APP_CMD_CONFIG_CHANGED);
}

static void onLowMemory(ANativeActivity* activity)
{
    Application* app = (Application*)activity->instance;
    LOGI("LowMemory: %p\n", activity);
    app->writeCmd(APP_CMD_LOW_MEMORY);
}


#ifdef __cplusplus
}
#endif
//END_INCLUDE(all)
