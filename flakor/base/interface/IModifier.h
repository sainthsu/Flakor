#ifndef _FK_IMODIFIER_H_
#define _FK_IMODIFIER_H_

FLAKOR_NS_BEGIN

template<class T>
class ModifierListener<T>
{

		// ===========================================================
		// Methods
		// ===========================================================
public:
		virtual ~ModifierListener() = 0 {};

		virtual void onModifierStarted(ModifierInterface<T> pModifier,  T pItem) = 0;
		virtual void onModifierFinished(ModifierInterface<T> pModifier,  T pItem) = 0;
};

/**
 * Modifier interface that affects Entity's properties
 */
class IModifier
{
public:
	virtual ~IModifier() = 0 {};	

	virtual void reset() = 0;

	virtual bool isFinished()= 0;
	virtual bool isAutoUnregisterWhenFinished()= 0;
	virtual void setAutoUnregisterWhenFinished(boolean removeWhenFinished) = 0;

	virtual ModifierInterface<T> deepCopy() = 0;

	virtual float getSecondsElapsed() = 0;
	virtual float getDuration() = 0;

	virtual float onUpdate(const float secondsElapsed, const T item)= 0;

	virtual void addModifierListener(ModifierListener<T> pModifierListener)= 0;
	virtual bool removeModifierListener(ModifierListener<T> pModifierListener)= 0;
};

FLAKOR_NS_END

#endif
