#include "./Device.h"
#include "jni/DPIJni.h"

FLAKOR_NS_BEGIN

int Device::getDPI()
{
    static int dpi = -1;
    if (dpi == -1)
    {
        dpi = (int)getDPIJNI();
    }
    return dpi;
}

FLAKOR_NS_END
