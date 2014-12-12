#ifndef _FK_IMATCHER_H_
#define _FK_IMATCHER_H_

FLAKOR_NS_BEGIN

template <typename T>
class IMatcher
{
	virtual ~IMatcher() = 0 {};
	virtual bool matches(T item) = 0;
};

FLAKOR_NS_END

#endif
