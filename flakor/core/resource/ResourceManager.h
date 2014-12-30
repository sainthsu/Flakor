/*filename 规则采用URI的定义
 */
#ifndef _FK_RESOURCE_MANAGER_H_
#define _FK_RESOURCE_MANAGER_H_

#include <map>
#include "Resource.h"

/*Images 通过 ImageLoader
· Textures 通过 TextureLoader
· BitmapFonts 通过BitmapFontLoader
· TextureAtlases 通过TextureAtlasLoader
· TiledAtlases 通过TiledAtlasLoader
· TileMapRenderers 通过 TileMapRendererLoader
· Music instances 通过 MusicLoader
· Sound instances 通过 SoundLoader*/

FLAKOR_NS_BEGIN

class Reource;

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

		static const char* UNKOWN_NAME = "unknow";
        static const char* IMAGE_NAME = "image";
        static const char* TEXTURE_NAME = "texture";
        static const char* MUSIC_NAME = "music";
        static const char* SOUND_NAME = "sound";
    
        static const int MAX_RESOURCE = 1024*5;
        static int uniqueID = 1; //uid begin with 1
    
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

        int msgread;
        int msgwrite;
        int waitLoads;//resource waiting for loading to memory by loadthread

        int threadNum;
        LoadThread** thread;
		
    public:
        virtual ~RescourceManager();

        static RescourceManager* thisManager();
        //从资源文件加载纹理
        Resource *CreateResource(const char *uri,const char* type);
        Resource *getResourceByName(const char* name);
        Resource *getResourceById(int id);

        bool load(Resource* res);
        bool unload(Resource* res);
        bool reload(Resource* res,bool asyn);

        void registerLoader(const char* type,Loader loader);
        void unregisterLoader(const char *loader);

        static void setAssetManager(AAssetManager *assetMgr);
        static AAssetManager* getAssetManager(void);
    
    protected:
        Array* _pendingRecource;
        Array* _loadingResource;
        Array* _loadedResource;
    
        map<const char*,Loader*> _loaders;

        static AAssetManager *assetManager;
    private:
        RescourceManager();
        void prepare();
}



FLAKOR_NS_END

#endif
