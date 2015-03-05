#ifndef _FK_DEVICE_H_
#define _FK_DEVICE_H_

#include "targetMacros.h"

FLAKOR_NS_BEGIN

/**
 @js NA
 @lua NA
 */
class Device
{
protected:
	int cpuCount;
	int ram;
	int screenWidth;
    int screenHeight;
    char lang[2];
    char country[2];
private:
    Device();
public:
    static Device *thisDevice(void);
	int getCpuCount();
    int getRAM();
};


FLAKOR_NS_END

#endif /* _FK_DEVICE_H_ */
