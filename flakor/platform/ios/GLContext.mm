/*
 * Copyright 2014  Saint Hsu
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
 */

//--------------------------------------------------------------------------------
// GLContext.cpp
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// includes
//--------------------------------------------------------------------------------
#include "macros.h"
#include "platform/ios/GLContext.h"
#include "platform/ios/EAGLView.h"

#include <unistd.h>
#include <string>

FLAKOR_NS_BEGIN

//--------------------------------------------------------------------------------
// eGLContext
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
// Ctor
//--------------------------------------------------------------------------------
GLContext::GLContext():
_screenWidth( 0 ),
_screenHeight( 0 ),
_es3Supported( false ),
_initialized( false ),
_glView(NULL)
{
}

//--------------------------------------------------------------------------------
// Dtor
//--------------------------------------------------------------------------------
GLContext::~GLContext()
{
    terminate();
}

bool GLContext::initWithEAGLView(void *eaglview)
{
    if( _initialized)
        return true;

    //
    //Initialize EGL
    //
    _glView = eaglview;
    EAGLView *glView = (EAGLView*) _glView;
    
    _screenWidth  = [glView getWidth];
    _screenHeight  = [glView getHeight];
    //    _scaleX = _scaleY = [glview contentScaleFactor];

    _initialized = true;

    return true;
}


void GLContext::swap()
{
    EAGLView *glView = (EAGLView*) _glView;
    [glView swapBuffers];
}

void GLContext::terminate()
{
    if( _glView != NULL )
    {
        // destroy EAGLView
        EAGLView *eaglview = (EAGLView*) _glView;
        
        [eaglview removeFromSuperview];
    }

}

bool GLContext::invalidate()
{
    terminate();

    _initialized = false;
    return true;
}

bool GLContext::checkExtension( const char* extension )
{
    if( extension == NULL )
        return false;

    std::string extensions = std::string( (char*) glGetString( GL_EXTENSIONS ) );
    std::string str = std::string( extension );
    str.append( " " );

    size_t pos = 0;
    if( extensions.find( extension, pos ) != std::string::npos )
    {
        return true;
    }

    return false;
}

FLAKOR_NS_END   //namespace flakor
