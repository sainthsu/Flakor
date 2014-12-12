/*****************************************************
 * Copyright (C) 2014 Saint Hsu （saint@aliyun.com）
 * www.flakor.org
 *****************************************************/

//BEGIN_INCLUDE
#ifndef _FK_ACTIVITYCALLBACK_H_
#define _FK_ACTIVITYCALLBACK_H_

#ifdef __cplusplus
extern "C" {
#endif

//useless file
/**
 * nativeActivity lifecycle
 */
static void onStart(ANativeActivity* activity);

static void onPause(ANativeActivity* activity);

static void onStop(ANativeActivity* activity);

static void onResume(ANativeActivity* activity);


static void onDestroy(ANativeActivity* activity);

static void* onSaveInstanceState(ANativeActivity* activity, size_t* outLen);

static void onWindowFocusChanged(ANativeActivity* activity, int focused);

static void onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window);

static void onNativeWindowResized(ANativeActivity* activity, ANativeWindow* window);

static void onNativeWindowRedrawNeeded(ANativeActivity* activity, ANativeWindow* window);

static void onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window);

static void onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue);

static void onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue);

static void onContentRectChanged(ANativeActivity* activity, const ARect* rect);

static void onConfigurationChanged(ANativeActivity* activity);

static void onLowMemory(ANativeActivity* activity);


#ifdef __cplusplus
}
#endif

#endif
//END_INCLUDE
