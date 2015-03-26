#ifndef _FK_ILOADER_H_
#define _FK_ILOADER_H_

#include "targetMacros.h"

FLAKOR_NS_BEGIN

class Uri;
class Resource;

class ILoader
{
public:
	virtual ILoader* createLoader() = 0;
	virtual Resource* createRes(Uri* uri) = 0;
	virtual bool load(Resource* res) = 0;
    
    virtual ~ILoader(){};
};

FLAKOR_NS_END

#endif
