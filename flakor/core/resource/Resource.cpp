#ifndef _FK_RESOURCE_H_
#define _FK_RESOURCE_H_

#include "core/resource/Resource.h"
#include "core/resource/ResourceManager.h"

Resource::Resource()
:uri(NULL)
,uid(ResourceManager::uniqueID++)
,type(ResourceManager::UNKNOW)
,state(INITED)
{}

Resource::~Resource()
{
    FK_SAFE_DELETE(uri);
}
    
bool ResourceloadData(bool async)
{

}

ResourceState ResourcegetState(void)
{
    return state;
}

Uri* Resource::getUri(void)
{
    return uri;
}

const char* Resource::getFilename(void)
{
    return uri->filename;
}

const char* Resource::getType(void)
{
    return
}

