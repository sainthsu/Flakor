/*filename 规则采用URI的定义
 */
#ifndef _FK_RESOURCE_MANAGER_H_
#define _FK_RESOURCE_MANAGER_H_

#include "target.h"
#include <unordered_set>
#include <map>
#include <queue>

#include <pthread.h>

#if FK_TARGET_PLATFORM == FK_PLATFORM_ANDROID
#include <android/asset_manager.h>
#endif

/*Images 通过 ImageLoader
· Textures 通过 TextureLoader
· BitmapFonts 通过BitmapFontLoader
· TextureAtlases 通过TextureAtlasLoader
· TiledAtlases 通过TiledAtlasLoader
· TileMapRenderers 通过 TileMapRendererLoader
· Music instances 通过 MusicLoader
· Sound instances 通过 SoundLoader*/

FLAKOR_NS_BEGIN

class Resource;
class LoaderThread;
class ILoader;
class Array;
class Uri;

class ResourceManager
{
    public:
        enum {
                UNKNOW,
                IMAGE,
                TEXTURE,
                MUSIC,
                SOUND
        };

		static const char* UNKNOW_NAME;
        static const char* IMAGE_NAME;
        static const char* TEXTURE_NAME;
        static const char* MUSIC_NAME;
        static const char* SOUND_NAME;
    
        static const int MAX_RESOURCE = 1024*5;
        static int uniqueID; //uid begin with 1
    
        /**
         * Path to this application's internal data directory.
         */
        const char* internalDataPath;
    
        /**
         * Path to this application's external (removable/mountable) data directory.
         */
        const char* externalDataPath;

        pthread_mutex_t mutex;
        pthread_cond_t cond;
		bool running;

        int msgread;
        int msgwrite;
        int waitLoads;//resource waiting for loading to memory by loadthread

        int threadNum;
        LoaderThread** threads;
		
    public:
        virtual ~ResourceManager();

        static ResourceManager* thisManager();
        //从资源文件加载纹理
		//Resource *CreateResource(const String *str,const char* type);
        Resource *createResource(const char *uriChar,const char* type);
		Resource *getResourceByUri(Uri* uri);
        Resource *getResourceByName(const char* name);
        Resource *getResourceById(int id);
		Resource *getWaitingRes();

        bool load(Resource* res,bool asyn);
        bool unload(Resource* res);
        bool reload(Resource* res,bool asyn);

        void registerLoader(const char* type,ILoader* loader);
        void unregisterLoader(const char *loader);

#if FK_TARGET_PLATFORM == FK_PLATFORM_ANDROID
        static void setAssetManager(AAssetManager *assetMgr);
        static AAssetManager* getAssetManager(void);
#endif
    
    protected:
        Array* _pendingResource;
        Array* _loadingResource;
        Array* _loadedResource;
    
		std::unordered_set<Resource*> _managedResource;
        std::map<const char*,ILoader*> _loaders;

		std::queue<Resource*> _resourceQueue;

#if FK_TARGET_PLATFORM == FK_PLATFORM_ANDROID
        static AAssetManager *assetManager;
#endif
    
    private:
        ResourceManager();
        void prepare();
};


FLAKOR_NS_END

#endif
