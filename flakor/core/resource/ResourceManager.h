/*filename 规则采用URI的定义
 *本地：
 *asset://
 *local://
 *网络：
 *http://
 *
 */

#ifndef _FK_RESOURCE_MANAGER_H_
#define _FK_RESOURCE_MANAGER_H_

#include "IResource.h"

/*Pixmaps 通过 PixmapLoader
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
        static const char* TEXTURE;
        static const char* AUDIO;
	public:
	    //从资源文件加载纹理
		IResource *CreateResource(const char *filename,int id,type,void *param);
		IResource *getResourceByName();
		IResource *getResourceById();
        bool load();
		bool unload();
		

		static void setAssetManager(AAssetManager *assetMgr);
	protected:
		Array* _loadedResource;
 		Array* _loaders;
		static AAssetManager *assetManager;
}

FLAKOR_NS_END

#endif
