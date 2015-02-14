#ifndef _FK_IUPDATABLE_H_
#define _FK_IUPDATABLE_H_

#include <functional>

FLAKOR_NS_BEGIN

typedef std::function<void(float)> DRAW_FUNC;

class IDrawable
{
	public:
        virtual void onDraw(float delta) = 0;
};

FLAKOR_NS_END

#endif

