#ifdef _FK_IUPDATABLE_H_
#define _FK_IUPDATABLE_H_

class IUpdatable
{
	public:
		virtual void setIngnoreUpdate(bool ingnore);
		virtual bool getIngnoreUpdate();

		virtual void onUpdate(float delta);
		virtual void reset();
}

#endif

