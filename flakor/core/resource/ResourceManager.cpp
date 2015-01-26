#include "targetMacros.h"
#include "core/resource/ResourceManager.h"
#include "core/resource/Resource.h"
#include "core/resource/LoaderThread.h"
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
#if FK_TARGET_PLATFORM == FK_PLATFORM_ANDROID
AAssetManager* ResourceManager::assetManager = NULL;
#endif
static ResourceManager* resMgr = NULL;

ResourceManager::ResourceManager()
:running(false)
,threadNum(0)
,threads(NULL)
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

Resource *ResourceManager::getResourceByUri(Uri* uri)
{
	Uri* u;
	Resource* r = NULL;

	for(Resource* res : _managedResource)
	{
		u = res->getUri();
		if(u->equal(uri))
		{
			r = res;
			break;
		}	
	}

	return r;
}

Resource *ResourceManager::getResourceByName(const char* name)
{
    Resource* r = NULL;
    const char* u;
    
    for(Resource* res : _managedResource)
    {
        u = res->getFilename();
        if(strcmp(u,name) == 0)
        {
            r = res;
            break;
        }	
    }
    
    return r;
}

Resource *ResourceManager::getResourceById(int uid)
{
    Resource* r = NULL;
    unsigned int u;
    
    for(Resource* res : _managedResource)
    {
        u = res->getUid();
        if(u == uid)
        {
            r = res;
            break;
        }	
    }
    
    return r;
}

Resource *ResourceManager::getWaitingRes()
{
    Resource * res = _resourceQueue.front();
    _resourceQueue.pop();
    
    return res;
}


Resource *ResourceManager::createResource(const char* uriChar, const char* type)
{
	FKLOG("res create");
	Uri *uri = Uri::parse(uriChar);

	Resource* newRes = getResourceByUri(uri);
	
  	if(newRes == NULL)
  	{
    	ILoader* loader = _loaders[type];
    	if(loader != NULL)
    	{
    	   newRes = loader->createRes(uri);
    	   _pendingResource->addObject(newRes);
		   _managedResource.insert(newRes);
    	}
  	}

	return newRes;
}

bool ResourceManager::load(Resource* res, bool asyn)
{
   if(asyn)
   {
        //add to loadTaskQueue
       _resourceQueue.push(res);
       waitLoads++;
       if (threads == NULL) {
           
           threadNum = 2;
           threads = (LoaderThread**)malloc(sizeof(LoaderThread *)*2);
           threads[0] = new LoaderThread();
           threads[0]->start();
           threads[1] = new LoaderThread();
           threads[1]->start();
       }
       return true;
   }
   else
   {
      _pendingResource->removeObject(res);
      _loadingResource->addObject(res);

	  FKLOG("res->getType %s",res->getType());
	  ILoader* loader = _loaders[res->getType()];
      return loader->load(res);
   }
}

bool ResourceManager::unload(Resource* res)
{
	//res->setState();
    return true;
}

bool ResourceManager::reload(Resource* res,bool asyn)
{
	this->unload(res);
	return this->load(res,asyn);
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

#if FK_TARGET_PLATFORM == FK_PLATFORM_ANDROID

void ResourceManager::setAssetManager(AAssetManager *assetMgr)
{
	if(assetMgr == NULL)
	{
		return;
	}
    ResourceManager::assetManager = assetMgr;
}

AAssetManager* ResourceManager::getAssetManager(void)
{
    return ResourceManager::assetManager;
}

#endif
FLAKOR_NS_END
