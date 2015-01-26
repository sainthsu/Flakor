#include "targetMacros.h"
#include "core/resource/Resource.h"
#include "core/resource/Uri.h"
#include "core/resource/ILoader.h"
#include "core/resource/ResourceManager.h"


FLAKOR_NS_BEGIN

Resource::Resource()
:_uri(NULL)
,_uid(ResourceManager::uniqueID++)
,_type(ResourceManager::UNKNOW)
,_state(INITED)
,_loader(NULL)
,_listener(NULL)
{}

Resource::~Resource()
{
    FK_SAFE_DELETE(_uri);
    FK_SAFE_DELETE(_loader);
}
    
bool Resource::load(bool async)
{
	ResourceManager* mgr = ResourceManager::thisManager();
	return mgr->load(this,async);
}

bool Resource::unload()
{
	ResourceManager* mgr = ResourceManager::thisManager();
	return mgr->unload(this);
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
    return _uri->filename->getCString();
}

const char* Resource::getType(void)
{
	const char* typeChar;
	switch(_type)
	{
		case ResourceManager::IMAGE:
			typeChar =  ResourceManager::IMAGE_NAME;
			break;
		case ResourceManager::TEXTURE:
			typeChar =  ResourceManager::TEXTURE_NAME;
			break;
		case ResourceManager::MUSIC:
			typeChar =  ResourceManager::MUSIC_NAME;
			break;
		case ResourceManager::SOUND:
			typeChar =  ResourceManager::SOUND_NAME;
			break;
		default:
			typeChar =  ResourceManager::UNKNOW_NAME;
			break;
		
	}

    return typeChar;
}

void Resource::setType(int type)
{
	_type = type;
}

ResourceListener* Resource::getListener()
{
	return _listener;
}

void Resource::setListener(ResourceListener* listener)
{
	_listener = listener;
}

FLAKOR_NS_END

