#ifndef _FK_TARGET_ASSERT_H_
#define _FK_TARGET_ASSERT_H_

#include <android/log.h>

#define FK_DLL

#define FK_NO_MESSAGE_PSEUDOASSERT(cond)                        \
    if (!(cond)) {                                              \
        __android_log_print(ANDROID_LOG_ERROR,                  \
                            "flakor assert",                 \
                            "%s function:%s line:%d",           \
                            __FILE__, __FUNCTION__, __LINE__);  \
    }

#define FK_MESSAGE_PSEUDOASSERT(cond, msg)                          \
    if (!(cond)) {                                                  \
        __android_log_print(ANDROID_LOG_ERROR,                      \
                            "flakor assert",                     \
                            "file:%s function:%s line:%d, %s",      \
                            __FILE__, __FUNCTION__, __LINE__, msg); \
    }

#define FK_ASSERT(cond) FK_NO_MESSAGE_PSEUDOASSERT(cond)

#define FK_UNUSED_PARAM(unusedparam) (void)unusedparam

#endif
