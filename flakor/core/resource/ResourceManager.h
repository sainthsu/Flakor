/*filename 规则采用URI的定义
 *本地：
 *asset://
 *local://
 *网络：
 *http://
 */

#ifndef _FK_RESOURCE_MANAGER_H_
#define _FK_RESOURCE_MANAGER_H_

#include "IResource.h"

/*Images 通过 ImageLoader
· Textures 通过 TextureLoader
· BitmapFonts 通过BitmapFontLoader
· TextureAtlases 通过TextureAtlasLoader
· TiledAtlases 通过TiledAtlasLoader
· TileMapRenderers 通过 TileMapRendererLoader
· Music instances 通过 MusicLoader
· Sound instances 通过 SoundLoader*/

FLAKOR_NS_BEGIN

class ResourceManager
{
    public:
		enum {
			IMAGE,
			TEXTURE,
			MUSIC,
			SOUND
		}

		enum {
			ASSET,
			LOCAL,
			INTERNET
		}

		static const char* IMAGE = "image";
        static const char* TEXTURE = "texture";
        static const char* MUSIC = "music";
		static const char* SOUND = "sound";
		
	public:
		virtual ~RescourceManager();

		static RescourceManager* thisManager();
	    //从资源文件加载纹理
		IResource *CreateResource(const char *filename,const char* type,void *param);
		IResource *getResourceByName(const char* name);
		IResource *getResourceById(int id);

        bool load(IResource* res);
		bool unload(IResource* res);
		bool reload(IResource* res);

		void registerLoader(Loader loader);
		void unregisterLoader(const char *loader);

		static void setAssetManager(AAssetManager *assetMgr);
	protected:
		Array* _loadedResource;
 		Array* _loaders;

		static AAssetManager *assetManager;
	private:
		RescourceManager();
}

static RescourceManager* resMgr;

FLAKOR_NS_END

#endif
