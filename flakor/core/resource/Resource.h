#ifndef _FK_RESOURCE_H_
#define _FK_RESOURCE_H_

#include "base/lang/Object.h"
#include <functional>

/**
 * inited : 默认值，资源已经创建；
 * loading：调用Load()之后，SetData()之前的状态，避免多次重复调用load();
 * Loaded: SetData()之后，并接通过验证;
 * LoadFailed：SetData()之后，但数据没有通过验证或者发生异常;
 * 这里的load指加载到内存
 */
typedef enum _ResourceState
{
	INITED,
	LOADING,
	LOADED,
	UNLOADED,
    FAILED,
	DESTROYED
} ResourceState;

FLAKOR_NS_BEGIN

class Uri;
class ResourceListener;
class ILoader;

class Resource : public Object
{
    protected:
        Uri* _uri;
        int _uid;
        int _type;
        ResourceState _state;
		ILoader* _loader;
		ResourceListener* _listener;
		
		std::function<void(Resource*)> _callback;

    public:
        Resource();
        virtual ~Resource();
    
        virtual bool load(bool async);
        virtual bool unload();

		inline int getUid() { return _uid; }
        ResourceState getState(void);
		void setState(ResourceState state);
        Uri* getUri(void);
		void setUri(Uri* uri);
        const char* getFilename(void);
    
        ILoader * getLoader(void);
        const char* getType(void);
		void setType(int type);
    
		ResourceListener* getListener();
		void setListener(ResourceListener* listener);

		inline void doCallback(){_callback(this);};
		inline void setCallback(const std::function<void(Resource*)>& callback) {_callback = callback;};
		void schedule();
};

class ResourceListener
{
	public:
		virtual void onBeginLoad(Resource *res) const = 0;
		virtual void onFinishLoad(Resource *res) const = 0;

		virtual void onBeginUnload(Resource *res) const = 0;
		virtual void onFinishUnLoad(Resource *res) const = 0;
};

FLAKOR_NS_END

#endif
