#include "Engine.h"
#include "Application.h"
#include "Game.h"

#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "engine", __VA_ARGS__))

FLAKOR_NS_BEGIN

int Engine::initDisplay(void)
{
    if (this->context != EGL_NO_CONTEXT)
    {
        surface = eglCreateWindowSurface( this->display, this->config, this->app->window, NULL );
    }
    else
    {
	// initialize OpenGL ES and EGL

    /*
     * Here specify the attributes of the desired configuration.
     * Below, we select an EGLConfig with at least 8 bits per color
     * component compatible with on-screen windows
     */
    const EGLint attribs[] = {
            EGL_RENDERABLE_TYPE,EGL_OPENGL_ES2_BIT, //Request opengl ES2.0
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_DEPTH_SIZE, 24,
            EGL_NONE
    };
    //set gl version to 2.0
    const EGLint glversion[] = {
        EGL_CONTEXT_CLIENT_VERSION,2,EGL_NONE};
    
    EGLint w, h, dummy, format;
    EGLint numConfigs;
    EGLConfig config;
    EGLSurface surface;
    EGLContext context;

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display, 0, 0);

    /* Here, the application chooses the configuration it desires. In this
     * sample, we have a very simplified selection process, where we pick
     * the first EGLConfig that matches our criteria */
    eglChooseConfig(display, attribs, &config, 1, &numConfigs);

    if( !numConfigs )
    {
            //Fall back to 16bit depth buffer
            const EGLint attribs[] = {
                EGL_RENDERABLE_TYPE,EGL_OPENGL_ES2_BIT, //Request opengl ES2.0
                EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                EGL_BLUE_SIZE, 8,
                EGL_GREEN_SIZE, 8,
                EGL_RED_SIZE, 8,
                EGL_DEPTH_SIZE, 16,
                EGL_NONE };
            eglChooseConfig( display, attribs, &config, 1, &numConfigs );
    }
    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
     * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
     * As soon as we picked a EGLConfig, we can safely reconfigure the
     * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

    ANativeWindow_setBuffersGeometry(this->app->window, 0, 0, format);

    surface = eglCreateWindowSurface(display, config, this->app->window, NULL);
    context = eglCreateContext(display, config, NULL, glversion);
    

	eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

    this->display = display;
    this->context = context;
    this->surface = surface;
	this->config = config;
    this->width = w;
    this->height = h;

    }
    
    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        LOGW("Unable to eglMakeCurrent");
        return -1;
    }

    
    // Initialize GL state.
    glEnable(GL_CULL_FACE);
    glViewport(0,0,this->width,this->height);
    glDisable(GL_DEPTH_TEST);

    return 0;
}

/**
 * Just the current frame in the display.
 */
void Engine::drawFrame() {
    if (this->display == NULL) {
        // No display.
        return;
    }
    
    // Just fill the screen with a color.
     glClearColor(0.f, 0.f,0.f, 1);
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (this->game != NULL)
	{
        LOGW("Game render!!!");
		this->game->render();
	}
	

    eglSwapBuffers(this->display, this->surface);
}

/**
 * Tear down the EGL context currently associated with the display.
 */
void Engine::termDisplay()
{
     if( surface != EGL_NO_SURFACE )
    {
        eglDestroySurface( display, surface );
        this->surface = EGL_NO_SURFACE;
    }
}

void Engine::saveState(void **saveState,size_t *size)
{

}

void Engine::destroy()
{
	if (this->display != EGL_NO_DISPLAY)
    {
        eglMakeCurrent(this->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (this->context != EGL_NO_CONTEXT)
        {
            eglDestroyContext(this->display, this->context);
        }
        if (this->surface != EGL_NO_SURFACE) {
            eglDestroySurface(this->display, this->surface);
        }
        eglTerminate(this->display);
    }
    this->state = STATE_DESTROYED;
    this->display = EGL_NO_DISPLAY;
    this->context = EGL_NO_CONTEXT;
    this->surface = EGL_NO_SURFACE;
}

/**
 * Process the next input event.
 */
int32_t Engine::handleInput(AInputEvent* event)
{
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
	{
        this->state = STATE_RUNNING;
        
        //engine->state.x = AMotionEvent_getX(event, 0);
        //engine->state.y = AMotionEvent_getY(event, 0);
        
        return 1;
    }
    return 0;
}

/**
 * Process the next main command.
 */
void Engine::handleCMD(int32_t cmd)
{
    switch (cmd)
    {
        case APP_CMD_SAVE_STATE:
            // The system has asked us to save our current state.  Do so.
            this->saveState(&this->app->savedState,
                            &this->app->savedStateSize);
		
            break;
        case APP_CMD_INIT_WINDOW:
            // The window is being shown, get it ready.
            if (this->app->window != NULL) {
                this->game = Game::thisGame();
                this->initDisplay();
                LOGW("game create!!!");
                this->game->create();
                this->drawFrame();
                this->state = STATE_RUNNING;
            }
            break;
        case APP_CMD_PAUSE:
            this->state = STATE_STOP;
			this->game->pause();
            break;
        case APP_CMD_TERM_WINDOW:
            // The window is being hidden or closed, clean it up.
            this->termDisplay();
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
            break;
        case APP_CMD_LOST_FOCUS:
            // When our app loses focus, we stop monitoring the accelerometer.
            // This is to avoid consuming battery while not being used.
            if (this->accelerometerSensor != NULL) {
                ASensorEventQueue_disableSensor(this->sensorEventQueue,
                        this->accelerometerSensor);
            }
            // Also stop animating.
            this->state = STATE_STOP;
            this->drawFrame();
            break;
    }
}

FLAKOR_NS_END

