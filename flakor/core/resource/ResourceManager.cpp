#include "targetMacros.h"
#include "core/resource/ResourceManager.h"
#include "core/resource/Loader.h"

FLAKOR_NS_BEGIN

static RescourceManager* resMgr == NULL;

RescourceManager::RescourceManager
{
    _loader = new map<const char*,Loader*>();
    _pendingResource = Array::createWithCapacity(4);
    _pendingResource->retain();
    _loadingResource = Array::createWithCapacity(4);
    _loadingResource->retain();
    _loadedResource = Array::createWithCapacity(4);
    _loadedResource->retain();
    
}

RescourceManager::~RescourceManager
{
    
}

RescourceManager* RescourceManager::thisManager(void)
{
    if (resMgr == NULL) {
        resMgr = new RescourceManager();
    }
    
    return resMgr;
}

Resource *RescourceManager::getResourceByName(const char* name)
{

}

Resource *RescourceManager::getResourceById(int id)
{

}

Resource *ResourceManager::CreateResource(const char* uriChar, const char* type, void* param)
{
	Uri *uri = Uri::parse(uriChar);
	
  if(_loadedResource.contains(id))
     retrun _loadedResource[id];
  else
  {
     Loader* loader = loaders[type];
     if(loader != NULL)
     {
       Resource* newRes = loader->Create(uri);
       _pendingResource->add(newRes);
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
      _pendingRecource->remove(res);
      _loadingResource->add(res);

	  Loader* loader = loaders[res->getType()];
      return loader->load(resource);
   }
}

bool ResourceManager::unload(Resource* res)
{

}

bool ResourceManager::reload(Resource* res,bool asyn)
{

}

void ResourceManager::registerLoader(Loader* loader)
{

}

void ResourceManager::unregisterLoader(const char *loader)
{

}

void ResourceManager::prepare()
{
    // Wait for thread to start.
    pthread_mutex_lock(&app->mutex);
    while (!app->running) {
        pthread_cond_wait(&app->cond, &app->mutex);
    }
    pthread_mutex_unlock(&app->mutex);
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
