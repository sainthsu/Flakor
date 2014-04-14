#ifndef _FK_OBJECT_H_
#define _FK_OBJECT_H_

class Object
{
	public:
		Object(void);
		virtual ~Object(void);
		Object* copy();
		virtual bool equal(const Object* pObject);
		virtual void finalize();
}
#endif
