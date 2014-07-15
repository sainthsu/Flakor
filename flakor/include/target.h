/****************************************************************************
Copyright (c) 2013 flakor.org

http://www.flakor.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef _FK_TARGET_H_
#define _FK_TARGET_H_

/**
Config of flakor project, define which target platform to be run in.
*/

//////////////////////////////////////////////////////////////////////////
// pre configure
//////////////////////////////////////////////////////////////////////////

// define supported target platform macro which Flakor engine uses.
#define FK_PLATFORM_UNKNOWN            0
#define FK_PLATFORM_IOS                1
#define FK_PLATFORM_ANDROID            2
#define FK_PLATFORM_WIN32              3
#define FK_PLATFORM_MARMALADE          4
#define FK_PLATFORM_LINUX              5
#define FK_PLATFORM_BADA               6
#define FK_PLATFORM_BLACKBERRY         7
#define FK_PLATFORM_MAC                8
#define FK_PLATFORM_NACL               9
#define FK_PLATFORM_EMSCRIPTEN        10
#define FK_PLATFORM_TIZEN             11
#define FK_PLATFORM_WINRT             12
#define FK_PLATFORM_WP8               13

// Determine target platform by compile environment macro.
#define FK_TARGET_PLATFORM             FK_PLATFORM_UNKNOWN

// mac
#if defined(FK_TARGET_OS_MAC)
#undef  FK_TARGET_PLATFORM
#define FK_TARGET_PLATFORM         FK_PLATFORM_MAC
#endif

// iphone
#if defined(FK_TARGET_OS_IPHONE)
    #undef  FK_TARGET_PLATFORM
    #define FK_TARGET_PLATFORM         FK_PLATFORM_IOS
#endif

// android
#if defined(ANDROID)
    #undef  FK_TARGET_PLATFORM
    #define FK_TARGET_PLATFORM         FK_PLATFORM_ANDROID
#endif

// WinRT (Windows Store App)
#if defined(WINRT) && defined(_WINRT)
    #undef  FK_TARGET_PLATFORM
    #define FK_TARGET_PLATFORM			FK_PLATFORM_WINRT
#endif

// WP8 (Windows Phone 8 App)
#if defined(WP8) && defined(_WP8)
    #undef  FK_TARGET_PLATFORM
    #define FK_TARGET_PLATFORM			FK_PLATFORM_WP8
#endif

// win32
#if defined(WIN32) && defined(_WINDOWS)
    #undef  FK_TARGET_PLATFORM
    #define FK_TARGET_PLATFORM         FK_PLATFORM_WIN32
#endif

// linux
#if defined(LINUX)
    #undef  FK_TARGET_PLATFORM
    #define FK_TARGET_PLATFORM         FK_PLATFORM_LINUX
#endif

// marmalade
#if defined(MARMALADE)
#undef  FK_TARGET_PLATFORM
#define FK_TARGET_PLATFORM         FK_PLATFORM_MARMALADE
#endif

// bada
#if defined(SHP)
#undef  FK_TARGET_PLATFORM
#define FK_TARGET_PLATFORM         FK_PLATFORM_BADA
#endif

// qnx
#if defined(__QNX__)
    #undef  FK_TARGET_PLATFORM
    #define FK_TARGET_PLATFORM     FK_PLATFORM_BLACKBERRY
#endif

// native client
#if defined(__native_client__)
    #undef  FK_TARGET_PLATFORM
    #define FK_TARGET_PLATFORM     FK_PLATFORM_NACL
#endif

// Emscripten
#if defined(EMSCRIPTEN)
    #undef  FK_TARGET_PLATFORM
    #define FK_TARGET_PLATFORM     FK_PLATFORM_EMSCRIPTEN
#endif

// tizen
#if defined(TIZEN)
    #undef  FK_TARGET_PLATFORM
    #define FK_TARGET_PLATFORM     FK_PLATFORM_TIZEN
#endif

//////////////////////////////////////////////////////////////////////////
// post configure
//////////////////////////////////////////////////////////////////////////

// check user set platform
#if ! FK_TARGET_PLATFORM
    #error  "Cannot recognize the target platform; are you targeting an unsupported platform?"
#endif 

#if (FK_TARGET_PLATFORM == FK_PLATFORM_WIN32)
  #pragma warning (disable:4127)
#endif  // FK_PLATFORM_WIN32

#endif

