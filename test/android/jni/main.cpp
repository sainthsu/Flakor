#include "Flakor.h"
#include <android/log.h>

#define  LOG_TAG    "main"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

using namespace flakor;

void printLog()
{
	LOGD("LOG PRINT!");
}


