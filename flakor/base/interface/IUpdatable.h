#ifndef _FK_IUPDATABLE_H_
#define _FK_IUPDATABLE_H_

#include <functional>

FLAKOR_NS_BEGIN

typedef std::function<void(float)> UPDATE_FUNC;

class IUpdatable
{
	public:
		virtual void onUpdate(float delta) = 0;
		virtual void reset() = 0;
};

FLAKOR_NS_END

#endif

