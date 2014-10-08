#ifndef _FK_IRESOURCE_H_
#define _FK_IRESOURCE_H_

class IResource
{
	protected:
		String name;
		int id;
		String type;
        ResourceState state;
    public:
        bool loadData();
}

#endif