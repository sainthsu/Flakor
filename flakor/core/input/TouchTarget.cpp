/****************************************************************************
Copyright (c) 2013-2014 Saint

http://www.flakor.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "core/input/TouchTarget.h"
#include "2d/Entity.h"
#include "core/input/TouchTrigger.h"

FLAKOR_NS_BEGIN

/**
 * @addtogroup input
 * @{
 */
TouchTarget::TouchTarget()
    :_child(NULL)
    ,_pointerIdBits(0)
    ,_next(NULL)
{
}

TouchTarget* TouchTarget::obtain(Entity* child,int pointerIdBits)
{
    TouchTarget* target = new TouchTarget();
    target->_child = child;
    target->_pointerIdBits = pointerIdBits;
    return target;
}

void TouchTarget::clear()
{
    TouchTarget* target = this;
    if (target != NULL)
    {
        do
        {
            TouchTarget* next = target->_next;
            delete(next);
            target = next;
        }
        while (target != NULL);
        delete(this);
    }
}

TouchTarget* TouchTarget::getTouchTarget(Entity* child)
{
    for (TouchTarget* target = this; target != NULL; target = target->_next)
    {
        if (target->_child == child)
        {
            return target;
        }
    }
    return NULL;
}

TouchTarget* TouchTarget::addTouchTarget(Entity* child, int pointerIdBits)
{
    TouchTarget *target = TouchTarget::obtain(child, pointerIdBits);
    target->_next = this;
    return target;
}

void TouchTarget::removePointers(int idBitsToAssign)
{
    TouchTarget *predecessor = NULL;
    TouchTarget *target = this;
    while (target != NULL)
    {
        TouchTarget *next = target->_next;
        if ((target->_pointerIdBits & idBitsToAssign) != 0)
        {
            target->_pointerIdBits &= ~idBitsToAssign;
            if (target->_pointerIdBits == 0)
            {
                if (predecessor == NULL)
                {
                    //this = next;
                }
                else
                {
                    predecessor->_next = next;
                }

                target = next;
                continue;
             }
         }
         predecessor = target;
         target = next;
    }
}

void TouchTarget::cancelAndClear(TouchTrigger* touch)
{
      bool syntheticEvent = false;
      if (touch == NULL)
      {
           syntheticEvent = true;
      }

      for (TouchTarget *target = this; target != NULL; target = target->_next)
      {
            //resetCancelNextUpFlag(target.child);
            //dispatchTransformedTouchEvent(event, true, target.child, target.pointerIdBits);
      }
      clear();
}

/*
void TouchTarget::cancelTouchTarget(Entity* child)
{
    TouchTarget predecessor = null;
    TouchTarget target = mFirstTouchTarget;
    while (target != null)
    {
                TouchTarget next = target.next;
                if (target.child == view) {
                    if (predecessor == null) {
                        mFirstTouchTarget = next;
                    } else {
                        predecessor.next = next;
                    }
                    target.recycle();

                    long now = SystemClock.uptimeMillis();
                    MotionEvent event = MotionEvent.obtain(now, now,
                            MotionEvent.ACTION_CANCEL, 0.0f, 0.0f, 0);
                    event.setSource(InputDevice.SOURCE_TOUCHSCREEN);
                    view.dispatchTouchEvent(event);
                    event.recycle();
                    return;
                }
                predecessor = target;
                target = next;
    }
}*/

// end of input group
/// @}

FLAKOR_NS_END
