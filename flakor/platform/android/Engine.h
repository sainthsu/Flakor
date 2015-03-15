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

#include <pthread.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android/input.h>

#include "macros.h"
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
	public:
		Application* app;
        Game* game;
    
    	ASensorManager* sensorManager;
    	const ASensor* accelerometerSensor;
    	ASensorEventQueue* sensorEventQueue;

		GLContext* glContext;
		bool initialized;
		bool hasFocus;

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
		inline void setApplication(Application* app) {this->app = app;};
		
		void onTickUpdate();
		void drawFrame(void);

        void saveState(void **savedState,size_t *size);
		void initFromState(void *savedState,size_t size);
		void termDisplay(void);
		void trimMemory();
		void destroy(void);

		void updateViewport();

		bool inline isFocus() {return hasFocus;};
		void initSensors();
		void processSensors( int32_t id );
		void resumeSensors();
		void suspendSensors();
		
		int32_t handleTouch(TouchTrigger::TouchAction action,int count,intptr_t ids[],float xs[],float ys[]);
		void handleCMD(int32_t cmd);

	protected:
		/** calculates delta time since last time it was called */    
    	void calculateDeltaTime();
    private:
		int initDisplay(void);
};

FLAKOR_NS_END

#endif
