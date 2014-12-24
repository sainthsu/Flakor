#ifndef _FK_RESOURCE_H_
#define _FK_RESOURCE_H_

#include "base/lang/FKString.h"

/**
 * inited : 默认值，资源已经创建；
 * loading：调用Load()之后，SetData()之前的状态，避免多次重复调用load();
 * Loaded: SetData()之后，并接通过验证;
 * LoadFailed：SetData()之后，但数据没有通过验证或者发生异常;
 * 这里的load指加载到内存
 */
typedef enum {
	INITED,
	LOADING,
	LOADED,
	UNLOADED,
        FAILED,
	DESTROYED
} ResoureState;

class IResource : Object
{
    protected:
        const char* uri;
        const char* filename;
        int uid;
        int type;
        ResourceState state;
    public:
        Resource();
        virtual ~Resource();
    
        virtual bool loadData(bool async);
        ResourceState getState(void);
        const char* getUri(void);
        const char* getFilename(void);
        const char* getType(void);
}

#endif
