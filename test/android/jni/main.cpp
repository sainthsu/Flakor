/*****************************************************
 * Copyright (C) 2014 Saint Hsu （saint@aliyun.com）
 * www.flakor.org
 *****************************************************/

//BEGIN_INCLUDE

#include "Flakor.h"
#include "classes/MainGame.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

#ifdef __cplusplus
extern "C" {
#endif

using namespace flakor;

/*create activity*/
void ANativeActivity_onCreate(ANativeActivity* activity,
        void* savedState, size_t savedStateSize) 
{
    LOGI("Creating: %p\n", activity);
    MainGame *game = new MainGame();

    activity->instance = Application::create(activity, savedState, savedStateSize);
}


#ifdef __cplusplus
}
#endif
