#ifndef _FK_ROLE_H_

#include "./entity/Entity.h"

FLAKOR_NS_BEGIN

typedef (void *)ACTION(Role* role,...)

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
		void addAction(char* name,ACTION action);
		doAction(char* name);
}

FLAKOR_NS_END

#endif
