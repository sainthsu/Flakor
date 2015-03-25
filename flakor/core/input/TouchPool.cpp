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


#include "core/input/TouchPool.h"
#include "core/input/Touch.h"
#include "core/input/TouchTarget.h"

FLAKOR_NS_BEGIN

unsigned int TouchPool::_indexBitsUsed = 0;

TouchPool::TouchPool()
:_splitTouchTrigger(true)
,_focused(NULL)
,_firstTouchTarget(NULL)
,_dispatching(false)
{
    _indexBitsUsed = 0;
}

int TouchPool::getUnUsedIndex()
{
    int i;
    int temp = _indexBitsUsed;

    for (i = 0; i < TouchTrigger::MAX_TOUCHES; i++) {
          if (! (temp & 0x00000001)) {
               _indexBitsUsed |= (1 <<  i);
               return i;
          }

          temp >>= 1;
    }

    // all bits are used
    return -1;
}

std::vector<Touch*> TouchPool::getAllTouchesVector()
{
    std::vector<Touch*> ret;
    int i;
    int temp = TouchPool::_indexBitsUsed;

    for (i = 0; i < TouchTrigger::MAX_TOUCHES; i++) {
        if ( temp & 0x00000001) {
               ret.push_back(_touches[i]);
        }
        temp >>= 1;
    }
    return ret;
}

void TouchPool::removeUsedIndexBit(int index)
{
    if (index < 0 || index >= TouchTrigger::MAX_TOUCHES)
    {
         return;
    }

    unsigned int temp = 1 << index;
    temp = ~temp;
    _indexBitsUsed &= temp;
}

bool TouchPool::registerEntity(Entity* entity)
{
    auto iterator = _entities->find(entity);
    if (iterator == _entities->end()) {
        auto result = _entities->insert(entity);
        return result.second;
    }
    
    return false;
}

bool TouchPool::removeEntity(Entity* entity)
{
    auto iterator = _entities->find(entity);
    if (iterator != _entities->end()) {
        size_t result = _entities->erase(entity);
        return result == 1;
    }
    
    return false;
}

bool TouchPool::handleTouch(TouchTrigger::TouchAction action,int num,intptr_t ids[],float xs[],float ys[])
{
    intptr_t id = 0;
    float x = 0.0f;
    float y = 0.0f;
    int unusedIndex = 0;
    TouchTrigger touchTrigger;

    for(int i=0;i<num;i++)
    {
        id = ids[i];
        x = xs[i];
        y = ys[i];


        auto iter = _touchIdReorderMap.find(id);

        // it is a new touch
        if (iter == _touchIdReorderMap.end())
        {
             if(action != TouchTrigger::TouchAction::DOWN)
             {
                 FKLOG("if the index doesn't exist, it is an error");
                 continue;
             }

            FKLOG("id = %ld", id);
            unusedIndex = getUnUsedIndex();

             // The touches is more than MAX_TOUCHES ?
             if (unusedIndex == -1) {
                   FKLOG("The touches is more than MAX_TOUCHES, unusedIndex = %d", unusedIndex);
                   continue;
             }

             Touch* touch = _touches[unusedIndex] = new (std::nothrow) Touch();
             touch->setTouchInfo(unusedIndex, x, y);

             FKLOG("x = %f y = %f", touch->getLocationInView().x, touch->getLocationInView().y);

             _touchIdReorderMap.insert(std::make_pair(id, unusedIndex));
             touchTrigger._touches.push_back(touch);

         }
        else
        {
            FKLOG("unusedindex = %d", iter->second);
            FKLOG("other x = %f y = %f", x, y);
            Touch* touch = _touches[iter->second];
            if (touch)
            {
                  touch->setTouchInfo(iter->second, x, y);

                  touchTrigger._touches.push_back(touch);

                  if(action == TouchTrigger::TouchAction::UP || action == TouchTrigger::TouchAction::CANCEL)
                  {
                      _touches[iter->second] = nullptr;
                      removeUsedIndexBit(iter->second);

                      _touchIdReorderMap.erase(id);

                  }
            }
            else
            {
                // It is error, should return.
                FKLOG("Moving touches with id: %ld error", (long int)id);
                return false;
            }

        }

    }

    if (touchTrigger._touches.size() == 0)
    {
         FKLOG("touches: size = 0");
         return false;
    }

    touchTrigger.setAction(action);
    
    bool result = dispatchTouch(&touchTrigger);
	
    if(action == TouchTrigger::TouchAction::DOWN || action == TouchTrigger::TouchAction::CANCEL)
    {
        for (auto& touch : touchTrigger._touches)
        {
            // release the touch object.
            touch->release();
        }
    }
    return result;
}

bool TouchPool::dispatchTouch(TouchTrigger *trigger)
{
    bool handled = false;
    

    TouchTrigger::TouchAction action = trigger->getAction();
        
    // Handle an initial down.
    if (action == TouchTrigger::TouchAction::DOWN)
    {
            // Throw away all previous state when starting a new touch gesture.
            // The framework may have dropped the up or cancel event for the previous gesture
            // due to an app switch, ANR, or some other state change.
            _firstTouchTarget->cancelAndClear(trigger);
            _firstTouchTarget->clear();
            /*resetCancelNextUpFlag(this);
             mGroupFlags &= ~FLAG_DISALLOW_INTERCEPT;
             mNestedScrollAxes = SCROLL_AXIS_NONE;*/
     }
        
     // Check for cancelation.
     bool canceled = resetCancelNextUpFlag(this)|| action == TouchTrigger::TouchAction::CANCEL;
        
     // Update list of touch targets for pointer down, if needed.
     bool split = _splitTouchTrigger;
     TouchTarget *newTouchTarget = NULL;
     bool alreadyDispatchedToNewTouchTarget = false;
     if (!canceled)
     {
            if (action == TouchTrigger::TouchAction::DOWN)
            {
                // always 0 for down
                int idBitsToAssign = split ? trigger->_touches[0]->getID(): TouchTarget::ALL_POINTER_IDS;
                
                // Clean up earlier touch targets for this pointer id in case they
                // have become out of sync.
                _firstTouchTarget->removePointers(idBitsToAssign);
                
                for(Entity* e :_entities)
                {
                   handled = e->dispatchTouch(trigger);
                   if(handled)
                       break;
                }

                if(handled)
                    alreadyDispatchedToNewTouchTarget = true;

                if (newTouchTarget == NULL && _firstTouchTarget != NULL) {
                    // Did not find a child to receive the event.
                    // Assign the pointer to the least recently added target.
                    newTouchTarget = _firstTouchTarget;
                    while (newTouchTarget->_next != NULL)
                    {
                        newTouchTarget = newTouchTarget->_next;
                    }
                    newTouchTarget->_pointerIdBits |= idBitsToAssign;
                }
            }
     }
        
     // Dispatch to touch targets.
     if (_firstTouchTarget == NULL)
     {
            // No touch targets so treat this as an ordinary view.
             for(Entity* e :_entities)
             {
                handled = e->dispatchTouch(trigger);
                if(handled)
                    break;
             }
     }
     else
     {
         // Dispatch to touch targets, excluding the new touch target if we already
         // dispatched to it.  Cancel touch targets if necessary.
            /*TouchTarget *predecessor = NULL;
            TouchTarget *target = _firstTouchTarget;
            while (target != NULL)
            {
                TouchTarget *next = target->_next;
                if (alreadyDispatchedToNewTouchTarget && target == newTouchTarget) {
                    handled = true;
                }
                else
                {
                    bool cancelChild = resetCancelNextUpFlag(target->_child) || intercepted;
                    if (dispatchTransformedTouchEvent(ev, cancelChild,
                                                      target.child, target.pointerIdBits)) {
                        handled = true;
                    }
                    if (cancelChild)
                    {
                        if (predecessor == NULL)
                        {
                            _firstTouchTarget = next;
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
            }*/
    }
        
    // Update list of touch targets for pointer up or cancel, if needed.
    if (canceled
        || action == TouchTrigger::TouchAction::UP)
    {
            _firstTouchTarget->cancelAndClear(trigger);
            _firstTouchTarget->clear();
            /*resetCancelNextUpFlag(this);
             mGroupFlags &= ~FLAG_DISALLOW_INTERCEPT;
             mNestedScrollAxes = SCROLL_AXIS_NONE;*/
    }
    else if (split && action == TouchTrigger::TouchAction::UP)
    {
            int idBitsToRemove = trigger->_touches[0]->getID();
            _firstTouchTarget->removePointers(idBitsToRemove);
    }
    
    return handled;

}

FLAKOR_NS_END

