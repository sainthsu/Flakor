/*
 * Copyright 2013-2015 Saint Hsu(saint@aliyun.com)
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
 */

//--------------------------------------------------------------------------------
// GLContext.h
//--------------------------------------------------------------------------------
#ifndef _FK_GLCONTEXT_H_
#define _FK_GLCONTEXT_H_

#include "core/opengl/GL.h"

FLAKOR_NS_BEGIN

//--------------------------------------------------------------------------------
// Constants
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
// Class
//--------------------------------------------------------------------------------

/******************************************************************
 * OpenGL context handler
 * The class handles OpenGL and EGL context based on Android activity life cycle
 * The caller needs to call corresponding methods for each activity life cycle events as it's done in sample codes.
 *
 * Also the class initializes OpenGL ES3 when the compatible driver is installed in the device.
 * getGLVersion() returns 3.0~ when the device supports OpenGLES3.0
 *
 * Thread safety: OpenGL context is expecting used within dedicated single thread,
 * thus GLContext class is not designed as a thread-safe
 */

class GLContext
{
private:
    //EGL configurations
    
    void *_glView;
    
    //Screen parameters
    int32_t _screenWidth;
    int32_t _screenHeight;
    int32_t _colorSize;
    int32_t _depthSize;

    //Flags
    bool _initialized;
    
    bool _es3Supported;
    float _glVersion;

    void terminate();

    GLContext( GLContext const& );
    void operator=( GLContext const& );
    GLContext();
    virtual ~GLContext();
public:
    static GLContext* getInstance()
    {
        //Singleton
        static GLContext instance;
        return &instance;
    }

    bool initWithEAGLView(void *eaglview);
    void* getEAGLView(){return _glView;};
    void swap();
    bool invalidate();
    bool isInited() { return _initialized;};
    

    int32_t getScreenWidth()
    {
        return _screenWidth;
    }
    int32_t getScreenHeight()
    {
        return _screenHeight;
    }

    int32_t getBufferColorSize()
    {
        return _colorSize;
    }

    int32_t getBufferDepthSize()
    {
        return _depthSize;
    }

    float getGLVersion()
    {
        return _glVersion;
    }

    bool checkExtension( const char* extension );
};

FLAKOR_NS_END   //namespace flakor

#endif /* _FK_GLCONTEXT_H_ */
