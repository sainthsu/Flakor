#ifndef _FK_RESOURCE_H_
#define _FK_RESOURCE_H_

#include "core/resource/Resource.h"
#include "core/resource/ResourceManager.h"

Resource::Resource()
:_uri(NULL)
,_uid(ResourceManager::uniqueID++)
,_type(ResourceManager::UNKNOW)
,_state(INITED)
,_loader(NULL)
{}

Resource::~Resource()
{
    FK_SAFE_DELETE(_uri);
    FK_SAFE_DELETE(_loader);
}
    
bool Resource::load(bool async)
{
	ResourceManager* mgr = ResourceManager::thisManager();
	mgr->load(this,async);
}

bool Resource::unload()
{
	ResourceManager* mgr = ResourceManager::thisManager();
	mgr->unload(this);
}

ResourceState Resource::getState(void)
{
    return _state;
}

void Resource::setState(ResourceState state)
{
    _state = state;
}

Uri* Resource::getUri(void)
{
    return _uri;
}

void Resource::setUri(Uri* uri)
{
    _uri = uri;
}

const char* Resource::getFilename(void)
{
    return _uri->filename;
}

const char* Resource::getType(void)
{
	char* typeChar;
	switch(_type)
	{
		case:ResourceManager::IMAGE:
			typeChar =  ResourceManager::IMAGE_NAME;
			break;
		case:ResourceManager::TEXTURE:
			typeChar =  ResourceManager::TEXTURE_NAME;
			break;
		case:ResourceManager::MUSICE:
			typeChar =  ResourceManager::MUSICE_NAME;
			break;
		case:ResourceManager::SOUND:
			typeChar =  ResourceManager::SOUND_NAME;
			break;
		default:
			typeChar =  ResourceManager::UNKNOW_NAME;
			break;
		
	}

    return typeName;
}

void Resource::setType(int type)
{
	_type = type;
}

