
#ifndef _FK_EVENT_H_
#define _FK_EVENT_H_

FLAKOR_NS_BEGIN

typedef (void *)PROCEDURE(Event* event,...)

class Event: public Object
{
	protected:
		Event* _parent;
		Array* roles;
		Array* procedures;
	public:
		Event();
		~Event();
		doProcedure(const char* name);
}

FLAKOR_NS_END

#endif
