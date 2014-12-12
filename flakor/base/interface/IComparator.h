#ifndef _FK_ICOMPARATOR_H_
#define _FK_ICOMPARATOR_H_

template <class T>
class IComparator
{
	public:
		virtual ~IComparator() = 0 {};

		int compare(T t,T t1);
		bool equals(Object* o);
}

#endif
