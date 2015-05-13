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
#import <Foundation/Foundation.h>

#include "tool/utility/ios/FileUtils.h"

FLAKOR_NS_BEGIN

static NSFileManager* nsFileManager = [NSFileManager defaultManager];

String* FileUtils::getWritablePath() const
{
    // save to document folder
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    const char* strRet = [documentsDirectory UTF8String];
    String *ret = String::create(strRet);
    ret->append("/");
    return ret;
}

bool FileUtils::isFileExistInternal(const String* fileName) const
{
    std::string filePath(fileName->getCString());
    if (filePath.empty())
    {
        return false;
    }

    bool ret = false;

    
    if (filePath[0] != '/')
    {
        std::string path;
        std::string file;
        size_t pos = filePath.find_last_of("/");
        if (pos != std::string::npos)
        {
            file = filePath.substr(pos+1);
            path = filePath.substr(0, pos+1);
        }
        else
        {
            file = filePath;
        }

        NSString* fullpath = [getBundle() pathForResource:[NSString stringWithUTF8String:file.c_str()]
                                                             ofType:nil
                                                        inDirectory:[NSString stringWithUTF8String:path.c_str()]];
        if (fullpath != nil) {
            ret = true;
        }
    }
    else
    {
        // Search path is an absolute path.
        if ([nsFileManager fileExistsAtPath:[NSString stringWithUTF8String:filePath.c_str()]]) {
            ret = true;
        }
    }

    return ret;
}

String* FileUtils::getFullPathForDirectoryAndFilename(const String* directory, const String* filename)
{
    if ((directory->getCString())[0] != '/')
    {
        NSString* fullpath = [getBundle() pathForResource:[NSString stringWithUTF8String:filename->getCString()]
                                                             ofType:nil
                                                        inDirectory:[NSString stringWithUTF8String:directory->getCString()]];
        if (fullpath != nil) {
            return String::create([fullpath UTF8String]);
        }
    }
    else
    {
        String* fullPath = String::create(directory->getCString());
        fullPath->append(filename);
        // Search path is an absolute path.
        if ([nsFileManager fileExistsAtPath:[NSString stringWithUTF8String:fullPath->getCString()]]) {
            return fullPath;
        }
    }
    return NULL;
}

String* FileUtils::getBundleFilePath(const String* fileName)
{
    return FileUtils::getBundleFilePath(fileName->getCString());
}

String* FileUtils::getBundleFilePath(const char* fileName)
{
    std::string filePath(fileName);
    if (filePath.empty())
    {
        return NULL;
    }
    
    String* ret = NULL;
    
    
    if (filePath[0] != '/')
    {
        std::string path;
        std::string file;
        size_t pos = filePath.find_last_of("/");
        if (pos != std::string::npos)
        {
            file = filePath.substr(pos+1);
            path = filePath.substr(0, pos+1);
        }
        else
        {
            file = filePath;
        }
        
        NSBundle* bundle = [NSBundle mainBundle];
        NSString* fullpath = [bundle pathForResource:[NSString stringWithUTF8String:file.c_str()]
                                                   ofType:nil
                                              inDirectory:[NSString stringWithUTF8String:path.c_str()]];
        if (fullpath != nil) {
            ret = String::create([fullpath UTF8String]);
        }
    }
    else
    {
        // Search path is an absolute path.
        if ([nsFileManager fileExistsAtPath:[NSString stringWithUTF8String:filePath.c_str()]]) {
            ret = String::create(fileName);
        }
    }
    
    return ret;

}

FileUtils::FileUtils()
{
    _bundle = [NSBundle mainBundle];
}


void FileUtils::setBundle(NSBundle* bundle)
{
    _bundle = bundle;
}

NSBundle* FileUtils::getBundle() const
{
    return _bundle;
}

FLAKOR_NS_END

