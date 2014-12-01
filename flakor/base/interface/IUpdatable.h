#ifndef _FK_IUPDATABLE_H_
#define _FK_IUPDATABLE_H_

FLAKOR_NS_BEGIN


class IUpdatable
{
	public:
		virtual void setIngnoreUpdate(bool ingnore) = 0;
		virtual bool getIngnoreUpdate() = 0;

		virtual void onUpdate(float delta) = 0;
		virtual void reset() = 0;
};

FLAKOR_NS_END

#endif

