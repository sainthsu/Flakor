
#include "Object.h"

FLAKOR_NS_BEGIN

Object::Object(void)
{
}

Object::~Object(void)
{

}

Object* Object::copy()
{
}

bool Object::equal(const Object* pObject)
{
	return this == pObject;
}

void Object::finalize()
{

}

FLAKOR_NS_END
