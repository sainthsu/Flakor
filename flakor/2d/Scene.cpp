#include "targetMacros.h"
#include "2d/Scene.h"

FLAKOR_NS_BEGIN

Scene::Scene()
{
	relativeAnchorPoint = false;
	setAnchorPoint(PointMake(0.5,0.5));
}

Scene::~Scene()
{

}


Scene* Scene::create()
{
	Scene *pRet = new Scene();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		FK_SAFE_DELETE(pRet);
		return NULL;
	}

}

bool Scene::dispatchTouchTrigger(TouchTrigger* trigger)
{
     bool handled = false;
     boolean handled = false;
            if (onFilterTouchEventForSecurity(ev)) {
                final int action = ev.getAction();
                final int actionMasked = action & MotionEvent.ACTION_MASK;

                // Handle an initial down.
                if (actionMasked == MotionEvent.ACTION_DOWN) {
                    // Throw away all previous state when starting a new touch gesture.
                    // The framework may have dropped the up or cancel event for the previous gesture
                    // due to an app switch, ANR, or some other state change.
                    cancelAndClearTouchTargets(ev);
                    resetTouchState();
                }

                // Check for interception.
                final boolean intercepted;
                if (actionMasked == MotionEvent.ACTION_DOWN
                        || mFirstTouchTarget != null) {
                    final boolean disallowIntercept = (mGroupFlags & FLAG_DISALLOW_INTERCEPT) != 0;
                    if (!disallowIntercept) {
                        intercepted = onInterceptTouchEvent(ev);
                        ev.setAction(action); // restore action in case it was changed
                    } else {
                        intercepted = false;
                    }
                } else {
                    // There are no touch targets and this action is not an initial down
                    // so this view group continues to intercept touches.
                    intercepted = true;
                }

                // Check for cancelation.
                final boolean canceled = resetCancelNextUpFlag(this)
                        || actionMasked == MotionEvent.ACTION_CANCEL;

                // Update list of touch targets for pointer down, if needed.
                final boolean split = (mGroupFlags & FLAG_SPLIT_MOTION_EVENTS) != 0;
                TouchTarget newTouchTarget = null;
                boolean alreadyDispatchedToNewTouchTarget = false;
                if (!canceled && !intercepted) {
                    if (actionMasked == MotionEvent.ACTION_DOWN
                            || (split && actionMasked == MotionEvent.ACTION_POINTER_DOWN)
                            || actionMasked == MotionEvent.ACTION_HOVER_MOVE) {
                        final int actionIndex = ev.getActionIndex(); // always 0 for down
                        final int idBitsToAssign = split ? 1 << ev.getPointerId(actionIndex)
                                : TouchTarget.ALL_POINTER_IDS;

                        // Clean up earlier touch targets for this pointer id in case they
                        // have become out of sync.
                        removePointersFromTouchTargets(idBitsToAssign);

                        final int childrenCount = mChildrenCount;
                        if (newTouchTarget == null && childrenCount != 0) {
                            final float x = ev.getX(actionIndex);
                            final float y = ev.getY(actionIndex);
                            // Find a child that can receive the event.
                            // Scan children from front to back.
                            final ArrayList<View> preorderedList = buildOrderedChildList();
                            final boolean customOrder = preorderedList == null
                                    && isChildrenDrawingOrderEnabled();
                            final View[] children = mChildren;
                            for (int i = childrenCount - 1; i >= 0; i--) {
                                final int childIndex = customOrder
                                        ? getChildDrawingOrder(childrenCount, i) : i;
                                final View child = (preorderedList == null)
                                        ? children[childIndex] : preorderedList.get(childIndex);
                                if (!canViewReceivePointerEvents(child)
                                        || !isTransformedTouchPointInView(x, y, child, null)) {
                                    continue;
                                }

                                newTouchTarget = getTouchTarget(child);
                                if (newTouchTarget != null) {
                                    // Child is already receiving touch within its bounds.
                                    // Give it the new pointer in addition to the ones it is handling.
                                    newTouchTarget.pointerIdBits |= idBitsToAssign;
                                    break;
                                }

                                resetCancelNextUpFlag(child);
                                if (dispatchTransformedTouchEvent(ev, false, child, idBitsToAssign)) {
                                    // Child wants to receive touch within its bounds.
                                                                                                          = ev.getDownTime();
                                    if (preorderedList != null) {
                                        // childIndex points into presorted list, find original index
                                        for (int j = 0; j < childrenCount; j++) {
                                            if (children[childIndex] == mChildren[j]) {
                                                mLastTouchDownIndex = j;
                                                break;
                                            }
                                        }
                                    } else {
                                        mLastTouchDownIndex = childIndex;
                                    }
                                    mLastTouchDownX = ev.getX();
                                    mLastTouchDownY = ev.getY();
                                    newTouchTarget = addTouchTarget(child, idBitsToAssign);
                                    alreadyDispatchedToNewTouchTarget = true;
                                    break;
                                }
                            }
                            if (preorderedList != null) preorderedList.clear();
                        }

                        if (newTouchTarget == null && mFirstTouchTarget != null) {
                            // Did not find a child to receive the event.
                            // Assign the pointer to the least recently added target.
                            newTouchTarget = mFirstTouchTarget;
                            while (newTouchTarget.next != null) {
                                newTouchTarget = newTouchTarget.next;
                            }
                            newTouchTarget.pointerIdBits |= idBitsToAssign;
                        }
                    }
                }

                // Dispatch to touch targets.
                if (mFirstTouchTarget == null) {
                    // No touch targets so treat this as an ordinary view.
                    handled = dispatchTransformedTouchEvent(ev, canceled, null,
                            TouchTarget.ALL_POINTER_IDS);
                } else {
                    // Dispatch to touch targets, excluding the new touch target if we already
                    // dispatched to it.  Cancel touch targets if necessary.
                    TouchTarget predecessor = null;
                    TouchTarget target = mFirstTouchTarget;
                    while (target != null) {
                        final TouchTarget next = target.next;
                        if (alreadyDispatchedToNewTouchTarget && target == newTouchTarget) {
                            handled = true;
                        } else {
                            final boolean cancelChild = resetCancelNextUpFlag(target.child)
                                    || intercepted;
                            if (dispatchTransformedTouchEvent(ev, cancelChild,
                                    target.child, target.pointerIdBits)) {
                                handled = true;
                            }
                            if (cancelChild) {
                                if (predecessor == null) {
                                    mFirstTouchTarget = next;
                                } else {
                                    predecessor.next = next;
                                }
                                target.recycle();
                                target = next;
                                continue;
                            }
                        }
                        predecessor = target;
                        target = next;
                    }
                }

                // Update list of touch targets for pointer up or cancel, if needed.
                if (canceled
                        || actionMasked == MotionEvent.ACTION_UP
                        || actionMasked == MotionEvent.ACTION_HOVER_MOVE) {
                    resetTouchState();
                } else if (split && actionMasked == MotionEvent.ACTION_POINTER_UP) {
                    final int actionIndex = ev.getActionIndex();
                    final int idBitsToRemove = 1 << ev.getPointerId(actionIndex);
                    removePointersFromTouchTargets(idBitsToRemove);
                }
            }

            if (!handled && mInputEventConsistencyVerifier != null) {
                mInputEventConsistencyVerifier.onUnhandledEvent(ev, 1);
            }
            return handled;
}

bool Scene::onTouchTrigger(TouchTrigger* trigger)
{
    return true;
}

FLAKOR_NS_END
