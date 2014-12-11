#ifndef _FK_ENGINE_H_
#define _FK_ENGINE_H_

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
#include <android/input.h>

#include "macros.h"

FLAKOR_NS_BEGIN

class Scene;
class Application;
class Game;

enum EngineState {
    STATE_INITAL,
    STATE_RUNNING,
    STATE_STOP,
    STATE_DESTROYED
};


class Engine
{
	public:
		Application* app;
        Game* game;
    
    	ASensorManager* sensorManager;
    	const ASensor* accelerometerSensor;
    	ASensorEventQueue* sensorEventQueue;

    	EGLDisplay display;
    	EGLSurface surface;
    	EGLContext context;
    	int32_t width;
    	int32_t height;

		enum EngineState state;
		long lastTick;

	public:
		void setApplication(Application* app);
		
		void create(void);
        void saveState(void **saveState,int32_t *size);
		void destroy(void);

		void drawFrame(void);
		void termDisplay(void);
		
		int32_t handleInput(AInputEvent* event);
		void handleCMD(int32_t cmd);

    private:
		int initDisplay(void);
};

FLAKOR_NS_END

#endif
