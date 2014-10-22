#ifndef _FK_ROLE_H_

#include "./entity/Entity.h"

FLAKOR_NS_BEGIN

class Role : public Entity
{
	protected:
        Array* attributes;
        Array* actions;
	public:
		Role();
		~Role();
        bool init();
        Role *create();
}

FLAKOR_NS_END

#endif
