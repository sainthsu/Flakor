#ifndef _FK_ILOADER_H_
#define _FK_ILOADER_H_

#include "core/resource/Uri.h"
#include "core/resource/Resource.h"

class ILoader
{
public:
	ILoader* createLoader() = 0;
	Resource* createRes(Uri* uri) = 0;
	bool Load(Resource* res) = 0;
}


#endif
