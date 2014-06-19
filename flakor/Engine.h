#ifndef _FK_ENGINE_H_
#define _FK_ENGINE_H_

#include <jni.h>

#include <EGL/egl.h>
#include <GLES/gl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>

FLAKOR_NS_BEGIN

class Engine
{
	protected:
		struct android_app* app;

    	ASensorManager* sensorManager;
    	const ASensor* accelerometerSensor;
    	ASensorEventQueue* sensorEventQueue;

    	EGLDisplay display;
    	EGLSurface surface;
    	EGLContext context;
    	int32_t width;
    	int32_t height;
		int running;
	public:
		int initDisplay(void);
		void create(void);
		void drawFrame(void);
		void termDisplay(void);
		int32_t handleInput(AInputEvent* event);
		void handleCMD(int32_t cmd);
}

FLAKOR_NS_END

#endif
