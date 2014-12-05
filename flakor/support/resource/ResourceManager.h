#ifndef _FK_RESOURCE_MANAGER_H_
#define _FK_RESOURCE_MANAGER_H_

#include "IResource.h"

FLAKOR_NS_BEGIN

class ResourceManager
{
    
	public:
	    //从资源文件加载纹理
		IResource *CreateResource(const char *filename,int id,type,void *param);
		IResource *getResourceByName();
		IResource *getResourceById();
		bool unload();
		

		static void setAssetManager(AAssetManager *assetMgr);
	protected:
		Array* _loadedResource;
 		Array* _loaders;
		static AAssetManager *assetManager;
}

FLAKOR_NS_END

#endif
