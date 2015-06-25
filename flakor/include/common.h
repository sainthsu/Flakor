/****************************************************************************
Copyright (c) 2013-2014 flakor.org

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

#ifndef _FK_COMMON_H_
#define _FK_COMMON_H_

#include "targetMacros.h"

FLAKOR_NS_BEGIN

/**
 * @addtogroup platform
 * @{
 */

/// The max length of CCLog message.
static const int MAX_LOG_LENGTH = 16*1024;

/**
@brief Output Debug message.
*/
void FK_DLL Log(const char * pszFormat, ...) FK_FORMAT_PRINTF(1, 2);

/**
 * lua can not deal with ...
 */
void FK_DLL LuaLog(const char * pszFormat);

/**
 * KunKua log
 */

void FK_DLL KKLog(const char * pszFormat);

/**
@brief Pop out a message box
*/
void FK_DLL MessageBox(const char * pszMsg, const char * pszTitle);

/**
@brief Enum the language type supported now
*/
typedef enum __LanguageType
{
	LanguageChinese = 0,
    LanguageEnglish,
    LanguageFrench,
    LanguageItalian,
    LanguageGerman,
    LanguageSpanish,
    LanguageDutch,
    LanguageRussian,
    LanguageKorean,
    LanguageJapanese,
    LanguageHungarian,
    LanguagePortuguese,
    LanguageArabic
} LanguageType;

// end of platform group
/// @}

FLAKOR_NS_END

#endif    // _FK_COMMON_H_
