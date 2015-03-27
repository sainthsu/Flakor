/****************************************************************************
 Copyright (c) 2010-2012 cocos2d-x.org
 Copyright (c) 2011      Zynga Inc.
 Copyright (c) 2013-2014 Chukong Technologies Inc.

 http://www.cocos2d-x.org

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
#ifndef _FK_FILEUTILS_H_
#define _FK_FILEUTILS_H_

#include "targetMacros.h"
#include "base/lang/Str.h"

FLAKOR_NS_BEGIN

/**
 * @addtogroup platform
 * @{
 */

//! @brief  Helper class to handle file operations
class FileUtils
{
public:
    FileUtils();
    /* override funtions */
    virtual String* getWritablePath() const;
    virtual String* getFullPathForDirectoryAndFilename(const String*& directory, const String*& filename);

    static String* getBundleFilePath(const String* fileName);
    void setBundle(NSBundle* bundle);
private:
    virtual bool isFileExistInternal(const String*& filePath);
    NSBundle* getBundle() const;
    NSBundle* _bundle;
};

// end of platform group
/// @}

FLAKOR_NS_END

#endif    // _FK_FILEUTILS_H_

