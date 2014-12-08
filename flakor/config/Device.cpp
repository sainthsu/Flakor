#include <sys/sysinfo.h>
#include "./Device.h"

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


 
int Device::getAvailableRAM()
{
        struct sysinfo si;
        sysinfo(&si);
        printf("Totalram:       %d\n", si.totalram);
        printf("Available:      %d\n", si.freeram);
}

FLAKOR_NS_END
