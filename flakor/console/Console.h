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

#ifndef _FK_COMSOLE_H_
#define _FK_COMSOLE_H_

#include "PlatformMacros.h"

FLAKOR_NS_BEGIN

/**
 * @addtogroup platform
 * @{
 */

/// The max length of Log message.
static const int kMaxLogLen = 16*1024;

class Console
{
	/**
	@brief Output Debug message.
	*/
	void log(const char * pszFormat, ...) FK_FORMAT_PRINTF(1, 2);

	/**
	* lua can not deal with ...
	*/
	void luaLog(const char * pszFormat);

	/**
	* kunkua can not deal with ...
	*/
	void kunkuaLog(const char * pszFormat);

}

/**
@brief Enum the language type supported now
*/
typedef enum {
    kLanguageEnglish = 0,
    kLanguageChinese,
    kLanguageFrench,
    kLanguageItalian,
    kLanguageGerman,
    kLanguageSpanish,
    kLanguageRussian,
    kLanguageKorean,
    kLanguageJapanese,
    kLanguageHungarian,
    kLanguagePortuguese,
    kLanguageArabic
} LanguageType;

// end of platform group
/// @}

FLAKOR_NS_END

#endif    // _FK_CONSOLE_H_
