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
private:
    Device();
public:
    /**
     *  Gets the DPI of device
     *  @return The DPI of device.
     */
    static int getDPI();
};


FLAKOR_NS_END

#endif /* _FK_DEVICE_H_ */