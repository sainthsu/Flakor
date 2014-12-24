#include "targetMacros.h"
#include "ResourceManager.h"

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

IResource *ResourceManager::CreateResource(const char* uri, const char* type, void* param)
{
  if(_loadedResource.contains(id))
     retrun loadedResource[id];
  else
  {
     loader = loaders[type];
     if(loader != null)
     {
       newRes = loader.CreateResource();
       loadedResource.add(newRes);
       return newRes;
     }
  }
}

CreateResource()
{
   return newRes = CreateResource();
}

bool ResourceManager::load(resource, bool asyn)
{
   if(asyn)
   {
        add to loadTaskQueue;
   }
   else
   {
      data = loader.load();
      resource.SetData(data);
   }
}

void ResourceManager::setAssetManager(AAssetManager *assetMgr)
{
    assetManager = assetMgr;
}

AAssetManager* ResourceManager::getAssetManager(void)
{
    return assetManager;
}

ResourceManager::parsePath(const char* filepath)
{

}


FLAKOR_NS_END
