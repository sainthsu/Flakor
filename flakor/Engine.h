
#ifndef _FK_ENGINE_H_
#define _FK_ENGINE_H_


class Engine
{
	protected:
		struct android_app* app;

    	ASensorManager* sensorManager;
    	const ASensor* accelerometerSensor;
    	ASensorEventQueue* sensorEventQueue;

    	int animating;
    	EGLDisplay display;
    	EGLSurface surface;
    	EGLContext context;
    	int32_t width;
    	int32_t height;
}


#endif
