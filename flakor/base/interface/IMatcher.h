#ifndef _FK_IMATCHER_H_
#define _FK_IMATCHER_H_

FLAKOR_NS_BEGIN

template <typename T>
class IMatcher
{
	virtual bool matches(T item);
};

FLAKOR_NS_END

#endif
