
#ifndef _FK_EVENT_H_
#define _FK_EVENT_H_

FLAKOR_NS_BEGIN

typedef (void *)PROCEDURE(Event* event,...)

class Event::Object
{
	protected:
		Event* _parent;
		Array* roles;
		Array* procedures;
	public:
		Event();
		~Event();
}

FLAKOR_NS_END

#endif
