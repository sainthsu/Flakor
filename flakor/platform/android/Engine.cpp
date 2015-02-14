#include "Engine.h"
#include "Application.h"
#include "platform/Game.h"
#include "core/opengl/GLContext.h"
#include "core/resource/Scheduler.h"
#include "core/resource/ResourceManager.h"
#include "base/update/UpdateThread.h"
#include "math/GLMatrix.h"

#include <unistd.h>

#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "engine", __VA_ARGS__))

FLAKOR_NS_BEGIN

Engine::Engine()
:state(STATE_INITAL)
,totalUpdated(0)
,totalFrames(0)
{
    lastTick = new struct timeval;
	schedule = Scheduler::thisScheduler();
	glContext = GLContext::GetInstance();
	updateThread = UpdateThread::create(this);
	pthread_mutex_init(&mutex, NULL);
}

Engine::~Engine()
{
	FK_SAFE_DELETE(updateThread);
	FK_SAFE_DELETE(schedule);
}

//run in update thread
void Engine::onTickUpdate()
{
	FKLOG("updateThread :frame:%d;update:%d",totalFrames,totalUpdated);
	if(totalFrames < totalUpdated)
	{
			FKLOG("updateThread schedule update!!!");
			usleep(40);
			return;
	}

	this->calculateDeltaTime();
	usleep(100);
	// skip one flame when _deltaTime equal to zero.
	if(deltaTime < FLT_EPSILON)
	{
	     return;
	}
	
	if (this->game != NULL)
	{
		LOGW("Engine tickupdate!!!");
		game->update(deltaTime);
	}

	while(pthread_mutex_trylock(&mutex) == EBUSY)
	{
			FKLOG("updateThread clear memory!!!");
			usleep(40);
	}

    FKLOG("updateThread swap memory!!!");
	totalUpdated++;
    pthread_mutex_unlock(&mutex);
}


int Engine::initDisplay(void)
{
	if( !initialized)
    {
        glContext->Init( app->window );
        //LoadResources();
        initialized = true;
    }
    else
    {
        // initialize OpenGL ES and EGL
        if( EGL_SUCCESS != glContext->Resume( app->window ))
        {
            //UnloadResources();
            //LoadResources();
        }
    }

    // Initialize GL state.
    glEnable( GL_CULL_FACE );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );

    updateViewport();

    return 0;
}

void Engine::updateViewport()
{
	int32_t width = glContext->GetScreenWidth();
	int32_t height = glContext->GetScreenHeight();
	//Note that screen size might have been changed
    glViewport( 0, 0, width, height );
	Matrix4 pMatrix = Matrix4::orthographic(width,height,-width/2, width/2);
    GLMode(GL_PROJECTION);
    GLMultiply(&pMatrix);
}

/**
 * Just the current frame in the display.
 */
void Engine::drawFrame()
{
    
	FKLOG("DrawThread :frame:%d;update:%d",totalFrames,totalUpdated);
	if(totalFrames > totalUpdated)
	{
		schedule->update(deltaTime);
		return;
	}

	pthread_mutex_lock(&mutex);
    // Just clear the screen with a color.
    glClearColor(1.f, 1.f,1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (this->game != NULL)
	{
		this->game->render();
		totalFrames++;
	}

    // Swap
    if( EGL_SUCCESS != glContext->Swap())
    {
        
    }
	
    pthread_mutex_unlock(&mutex);
}

/**
 * Tear down the EGL context currently associated with the display.
 */
void Engine::termDisplay()
{
    glContext->Suspend();
}

void Engine::saveState(void **savedState,size_t *size)
{
	
}

void Engine::initFromState(void *savedState,size_t size)
{
	if (savedState != NULL) {
        // We are starting with a previous saved state; restore from it.
        
    }
}

void Engine::trimMemory()
{
    LOGW( "Trimming memory" );
    glContext->Invalidate();
}

void Engine::destroy()
{
	
}

/**
 * Process the next input event.
 */
int32_t Engine::handleInput(AInputEvent* event)
{
    int32_t eventType = AInputEvent_getType(event);
    if (eventType == AINPUT_EVENT_TYPE_MOTION)
	{
        int pointerNumber = AMotionEvent_getPointerCount( event );
        
        int32_t ids[pointerNumber];
        float xs[pointerNumber];
        float ys[pointerNumber];
        
        for (int i = 0; i < pointerNumber; i++) {
            ids[i] = AMotionEvent_getPointerId(event, i );
            xs[i] = AMotionEvent_getX( event, i );
            ys[i] = AMotionEvent_getY( event, i );;
        }

        
        int32_t action = AMotionEvent_getAction(event);
        unsigned int flags = action & AMOTION_EVENT_ACTION_MASK;
        switch( flags )
        {
            case AMOTION_EVENT_ACTION_DOWN:
                /*int idDown = pMotionEvent.getPointerId(0);
                float xDown = xs[0];
                float yDown = ys[0];*/
                break;
            case AMOTION_EVENT_ACTION_UP:
            {
                int64_t eventTime = AMotionEvent_getEventTime(event);
                int64_t downTime = AMotionEvent_getDownTime(event);
                
                break;
            }
        }

        
        return 1;
    }
    else if(eventType == AINPUT_EVENT_TYPE_KEY) //消息来源于物理键盘或虚拟键盘
    {
    
    }
    return 0;
}

//-------------------------------------------------------------------------
//Sensor handlers
//-------------------------------------------------------------------------
void Engine::initSensors()
{
    sensorManager = ASensorManager_getInstance();
    accelerometerSensor = ASensorManager_getDefaultSensor( sensorManager,
            ASENSOR_TYPE_ACCELEROMETER );
    sensorEventQueue = ASensorManager_createEventQueue( sensorManager, app->looper,
            LOOPER_ID_USER, NULL, NULL );
}

void Engine::processSensors( int32_t id )
{
    // If a sensor has data, process it now.
    if( id == LOOPER_ID_USER )
    {
        if( accelerometerSensor != NULL )
        {
            ASensorEvent event;
            while( ASensorEventQueue_getEvents(sensorEventQueue, &event, 1 ) > 0 )
            {
            }
        }
    }
}

void Engine::resumeSensors()
{
    // When our app gains focus, we start monitoring the accelerometer.
    if( accelerometerSensor != NULL )
    {
        ASensorEventQueue_enableSensor( sensorEventQueue, accelerometerSensor );
        // We'd like to get 60 events per second (in us).
        ASensorEventQueue_setEventRate( sensorEventQueue, accelerometerSensor,
                (1000L / 60) * 1000 );
    }
}

void Engine::suspendSensors()
{
    // When our app loses focus, we stop monitoring the accelerometer.
    // This is to avoid consuming battery while not being used.
    if( accelerometerSensor != NULL )
    {
        ASensorEventQueue_disableSensor( sensorEventQueue, accelerometerSensor );
    }
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
    }
}

void Engine::calculateDeltaTime()
{
    struct timeval now;

    if (gettimeofday(&now, NULL) != 0)
    {
        FKLOG("error in gettimeofday");
        deltaTime = 0;
        return;
    }

    // new delta time. Re-fixed issue #1277
    if (nextDeltaTimeZero)
    {
        deltaTime = 0;
        nextDeltaTimeZero = false;
    }
    else
    {
        deltaTime = (now.tv_sec - lastTick->tv_sec) + (now.tv_usec - lastTick->tv_usec) / 1000000.0f;
        deltaTime = MAX(0, deltaTime);
    }

#if FLAKOR_DEBUG
    // If we are debugging our code, prevent big delta time
    if (deltaTime > 0.2f)
    {
        deltaTime = 1 / 60.0f;
    }
#endif

    *lastTick = now;
}

FLAKOR_NS_END

