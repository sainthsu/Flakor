/*
 * Copyright (C) 2014 Saint Hsu （saint@aliyun.com）
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

//BEGIN_INCLUDE
#include "Application.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

#ifdef __cplusplus
extern "C" {
#endif

/**
 * nativeActivity lifecycle
 */
static void onStart(ANativeActivity* activity)
{
    LOGV("Start: %p\n", activity);
    Application* app = (Application*)activity->instance;
    app->setActivityState(APP_CMD_START);
}

static void onPause(ANativeActivity* activity)
{
    LOGV("Pause: %p\n", activity);
    Application* app = (Application*)activity->instance;
    app->setActivityState(APP_CMD_PAUSE);
}

static void onStop(ANativeActivity* activity)
{
    LOGV("Stop: %p\n", activity);
    Application* app = (Application*)activity->instance;
    app->setActivityState(APP_CMD_STOP);
}

static void onResume(ANativeActivity* activity)
{
    LOGV("Resume: %p\n", activity);
    Application* app = (Application*)activity->instance;
    app->setActivityState(APP_CMD_RESUME);
}


static void onDestroy(ANativeActivity* activity) 
{
    LOGV("Destroy: %p\n", activity);
    Application* app = (Application*)activity->instance;
    app->free();
}

static void* onSaveInstanceState(ANativeActivity* activity, size_t* outLen)
{
    Application* app = (Application*)activity->instance;
    void* savedState = NULL;

    LOGV("SaveInstanceState: %p\n", activity);
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

static void onConfigurationChanged(ANativeActivity* activity)
{
    Application* app = (Application*)activity->instance;
    LOGV("ConfigurationChanged: %p\n", activity);
    app->writeCmd(APP_CMD_CONFIG_CHANGED);
}

static void onLowMemory(ANativeActivity* activity)
{
    Application* app = (Application*)activity->instance;
    LOGV("LowMemory: %p\n", activity);
    app->writeCmd(APP_CMD_LOW_MEMORY);
}

static void onWindowFocusChanged(ANativeActivity* activity, int focused)
{
    LOGV("WindowFocusChanged: %p -- %d\n", activity, focused);
    Application* app = (Application*)activity->instance;
    app->writeCmd(focused ? APP_CMD_GAINED_FOCUS : APP_CMD_LOST_FOCUS);
}

static void onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window)
{
    LOGV("NativeWindowCreated: %p -- %p\n", activity, window);
    Application* app = (Application*)activity->instance;

    app->setWindow(window);
}

static void onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window)
{
    LOGV("NativeWindowDestroyed: %p -- %p\n", activity, window);
    Application* app = (Application*)activity->instance;

    app->setWindow(NULL);
}

static void onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue)
{
    LOGV("InputQueueCreated: %p -- %p\n", activity, queue);
    Application* app = (Application*)activity->instance;

    app->setInput(queue);
}

static void onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue)
{
    LOGV("InputQueueDestroyed: %p -- %p\n", activity, queue);
    Application* app = (Application*)activity->instance;
    
    app->setInput(NULL);
}

/*create activity*/
void ANativeActivity_onCreate(ANativeActivity* activity,
        void* savedState, size_t savedStateSize) 
{
    LOGV("Creating: %p\n", activity);
    
    activity->callbacks->onStart = onStart;
    activity->callbacks->onResume = onResume;
    activity->callbacks->onSaveInstanceState = onSaveInstanceState;
    activity->callbacks->onPause = onPause;
    activity->callbacks->onStop = onStop;
    activity->callbacks->onDestroy = onDestroy;
    
    activity->callbacks->onWindowFocusChanged = onWindowFocusChanged;
    activity->callbacks->onNativeWindowCreated = onNativeWindowCreated;
    activity->callbacks->onNativeWindowResized = NULL;
    activity->callbacks->onNativeWindowRedrawNeeded = NULL;
    activity->callbacks->onNativeWindowDestroyed = onNativeWindowDestroyed;
    
    activity->callbacks->onInputQueueCreated = onInputQueueCreated;
    activity->callbacks->onInputQueueDestroyed = onInputQueueDestroyed;

    activity->callbacks->onContentRectChanged = NULL;
    
    activity->callbacks->onConfigurationChanged = onConfigurationChanged;
    activity->callbacks->onLowMemory = onLowMemory;

    activity->instance = Application::create(activity, savedState, savedStateSize);
}


#ifdef __cplusplus
}
#endif
//END_INCLUDE(all)
