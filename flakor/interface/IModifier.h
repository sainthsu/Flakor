#ifndef _FK_IMODIFIER_H_
#define _FK_IMODIFIER_H_

FLAKOR_NS_BEGIN

template<class T>
class ModifierListener<T>
{

		// ===========================================================
		// Methods
		// ===========================================================

		public void onModifierStarted(final ModifierInterface<T> pModifier, final T pItem);
		public void onModifierFinished(final ModifierInterface<T> pModifier, final T pItem);
};

/**
 * Modifier interface that affects Entity's properties
 */
class IModifier
{
public:

	virtual void reset();

	virtual bool isFinished();
	virtual bool isAutoUnregisterWhenFinished();
	virtual void setAutoUnregisterWhenFinished(final boolean removeWhenFinished);

	virtual ModifierInterface<T> deepCopy() throws DeepCopyNotSupportedException;

	virtual float getSecondsElapsed();
	virtual float getDuration();

	virtual float onUpdate(const float secondsElapsed, const T item);

	virtual void addModifierListener(final ModifierListener<T> pModifierListener);
	virtual bool removeModifierListener(final ModifierListener<T> pModifierListener);
};

FLKAOR_NS_END

#endif
