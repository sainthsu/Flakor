
#include "ResourceManager.h"

FLAKOR_NS_BEGIN


IResource *ResourceManager::CreateResource(name, id, "texture", textureParam)
{

  if(loadedResource.contains(id))
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

//ver 1:
CreateResource(……, bool asyn)
{
   newRes = CreateResource();
   if(asyn)
         add newRes to loadTaskQueue;
   else
   {
     data = loader.Load();
     newRes.SetData(data);
   } 
   return newRes;
}

//ver 2:
CreateResource()
{
   return newRes = CreateResource();
}

ResourceManager.Load(resource, bool asyn)
{
   if(asyn)
     add to loadTaskQueue;
   else
   {
      data = loader.load();
      resource.SetData(data);
   }
}

Resource.Load(bool asyn)
{
   ResourceManager.Load(this,asyn);
}

FLAKOR_NS_END
