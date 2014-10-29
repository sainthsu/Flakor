#ifndef _FK_RESOURCE_MANAGER_H_
#define _FK_RESOURCE_MANAGER_H_

#include "IResource.h"

class ResourceManager
{
    
public:
    //从资源文件加载纹理
	IResource CreateResource(name,id,type,T resourceParam);
    
}

CreateResoure(name, id, “texture”, null); //从资源文件加载纹理
CreateResource(name, id, “texture”, textureParam); //直接创建空白纹理

ResourceParam是一个泛型参数，传递关于特定资源的参数，比如创建一张空白纹理，resourceParam需要包含纹理大小，格式等信息：
CreateResource检查特定id的资源是否已经存在，如果是，则直接返回已加载资源。上面的伪代码中name和id看起来有些重复，这样写的原因是提醒大家name不一定等于id。直接把资源名作为id当然是可行的，很多引擎就这样做，因此必须保证美术在创建资源时没有重复名称。此外，应该避免用文件路径作为id，因为真正的资源文件路径很可能会变化，路径信息应该通过查询资源数据库得到。也可以考虑用自定义的GUID，不过这也需要在off-line阶段有工具为资源生成合适的GUID。

如果ResourceManager发现要创建的资源不存在，则先检查是否支持所需要创建的资源类型。有些引擎中，ResourceManager直接负责加载所有不同类型的资源，这种设计最大的缺点就是难以扩展，每次添加新类型就不得不修改整个ResourceManager。更好的设计是使用不同对象创建不同类型的资源，我把这些对象称为ResourceLoader。ResourceLoader把自己注册到manager中，manager用dictionary，以type为key保存注册过的loader。

CreateResource()
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

异步加载资源是任何现代游戏引擎必不可少的特性，资源加载通常涉及到IO，解压，资源组合等一系列费时的操作，如果直接在主线程中执行，那结果显然惨不忍睹。上面的模型很容易添加对异步加载的支持。为了让资源创建不阻塞主线程，并尽可能保证无论同步还是异步加载，接口代码都不需要做改动，我们把资源分为create和initialize2个阶段。在create阶段，我们只创建一个”空”对象，并不加载实际资源数据，这一步和new普通对象并没有区别，非常快，因此不会引起阻塞；initialize阶段再加载实际资源数据。主线程调用CreateResource之后，马上得到一个新对象，继续往下执行。ResourceManager异步执行initialize，资源加载完之后，通过回调函数传递给resource。当然，需要添加一个标记，让上层系统知道资源是否”可用”。

IResource
{
   name;
   type;
   resourceState;
   LoadData();
   SetData(T data, U resourceParam);
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

上面列出了两种稍微有些区别的异步加载方式， 可以看到，无论异步还是同步，用户编写代码的方式都不需要改变。可以用一个简单的bool变量来表示resourceState，但资源状态并非只有2种，ogre中定义了4种不同状态，我也定义了4种，稍微有些不同：

unload : 默认值，资源已经创建，但还没有初始化；

loading：调用Load()之后，SetData()之前的状态，避免多次重复调用load();

Loaded: SetData()之后，并接通过验证；

LoadFailed：SetData()之后，但数据没有通过验证或者发生异常；

#endif
 

资源加载之后，相关的问题就是卸载。与创建相同，资源的卸载也必须通过ResourceManager完成，卸载方式也分为unload和destroy两种，前者仅卸载资源数据，后者则彻底删除整个对象。这里关键的问题并不是如何卸载，而是何时卸载资源。依赖用户手动卸载对象显然是困难的，传统的卸载方式有2中，一种是通过引用计数，判断对象是否有效；另一种则是把资源做为逻辑group，通过显示调用，卸载整个group数据。两种方法各有优缺，前者允许单独卸载某个资源，后者实现起来则比较容易。对group方式，只需为IResource添加一个groupId，在resourcemanager中保存group集合即可，这种方法非常适合管理静态场景数据。引用计数方式则是传统的重载操作符，实现addRef，removeRef这里不仔细介绍。
