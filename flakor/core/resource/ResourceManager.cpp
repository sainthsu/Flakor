#include "targetMacros.h"
#include "core/resource/ResourceManager.h"
#include "core/resource/Resource.h"
#include "core/resource/ImageLoader.h"
#include "core/resource/Uri.h"
#include "base/lang/Array.h"

FLAKOR_NS_BEGIN

const char* ResourceManager::UNKNOW_NAME = "unknow";
const char* ResourceManager::IMAGE_NAME = "image";
const char* ResourceManager::TEXTURE_NAME = "texture";
const char* ResourceManager::MUSIC_NAME = "music";
const char* ResourceManager::SOUND_NAME = "sound";
int ResourceManager::uniqueID = 1;

static ResourceManager* resMgr = NULL;

ResourceManager::ResourceManager()
:running(false)
{
    //_loaders = new std::map<const char*,ILoader*>();
    _pendingResource = Array::createWithCapacity(4);
    _pendingResource->retain();
    _loadingResource = Array::createWithCapacity(4);
    _loadingResource->retain();
    _loadedResource = Array::createWithCapacity(4);
    _loadedResource->retain();

	ImageLoader* imgLoader = new ImageLoader();
	registerLoader(IMAGE_NAME,imgLoader);
    
}

ResourceManager::~ResourceManager()
{
    _loaders.clear();
	
}

ResourceManager* ResourceManager::thisManager(void)
{
    if (resMgr == NULL) {
        resMgr = new ResourceManager();
    }
    
    return resMgr;
}

Resource *ResourceManager::getResourceByName(const char* name)
{
	
}

Resource *ResourceManager::getResourceById(int id)
{
	
}

Resource *ResourceManager::getWaitingRes()
{
	
}


Resource *ResourceManager::CreateResource(const char* uriChar, const char* type)
{
	Uri *uri = Uri::parse(uriChar);
	
  if(_loadedResource->contains(id))
     return _loadedResource[id];
  else
  {
     ILoader* loader = _loaders[type];
     if(loader != NULL)
     {
       Resource* newRes = loader->CreateRes(uri);
       _pendingResource->addObject(newRes);
       return newRes;
     }
  }
}

bool ResourceManager::load(Resource* res, bool asyn)
{
   if(asyn)
   {
        //add to loadTaskQueue
   }
   else
   {
      _pendingResource->removeObject(res);
      _loadingResource->addObject(res);

	  ILoader* loader = _loaders[res->getType()];
      return loader->load(res);
   }
}

bool ResourceManager::unload(Resource* res)
{

}

bool ResourceManager::reload(Resource* res,bool asyn)
{

}

void ResourceManager::registerLoader(const char* type, ILoader* loader)
{
	_loaders.insert(std::map<const char*,ILoader*>::value_type(type,loader));
}

void ResourceManager::unregisterLoader(const char *loader)
{
	_loaders.erase(loader);
}

void ResourceManager::prepare()
{
    // Wait for thread to start.
    pthread_mutex_lock(&mutex);
    while (!running) {
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);
}

void ResourceManager::setAssetManager(AAssetManager *assetMgr)
{
    assetManager = assetMgr;
}

AAssetManager* ResourceManager::getAssetManager(void)
{
    return assetManager;
}


FLAKOR_NS_END
