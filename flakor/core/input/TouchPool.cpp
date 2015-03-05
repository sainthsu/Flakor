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

FLAKOR_NS_BEGIN

TouchPool::TouchPool()
:_indexBitsUsed(0)
{};

int TouchPool::getUnUsedIndex()
{
    int i;
    int temp = _indexBitsUsed;

    for (i = 0; i < EventTrigger::MAX_TOUCHES; i++) {
          if (! (temp & 0x00000001)) {
               _indexBitsUsed |= (1 <<  i);
               return i;
          }

          temp >>= 1;
    }

    // all bits are used
    return -1;

}

std::vector<Touch*> TouchPoolgetAllTouchesVector()
{
    std::vector<Touch*> ret;
    int i;
    int temp = g_indexBitsUsed;

    for (i = 0; i < EventTouch::MAX_TOUCHES; i++) {
        if ( temp & 0x00000001) {
               ret.push_back(g_touches[i]);
        }
        temp >>= 1;
    }
    return ret;
}

void TouchPool::removeUsedIndexBit(int index)
{
    if (index < 0 || index >= EventTouch::MAX_TOUCHES)
    {
         return;
    }

    unsigned int temp = 1 << index;
    temp = ~temp;
    _indexBitsUsed &= temp;
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

             unusedIndex = getUnUsedIndex();

             // The touches is more than MAX_TOUCHES ?
             if (unusedIndex == -1) {
                   FKLOG("The touches is more than MAX_TOUCHES, unusedIndex = %d", unusedIndex);
                   continue;
             }

             Touch* touch = _touches[unusedIndex] = new (std::nothrow) Touch();
             touch->setTouchInfo(unusedIndex, x, y);

             FKLOGINFO("x = %f y = %f", touch->getLocationInView().x, touch->getLocationInView().y);

             _touchIdReorderMap.insert(std::make_pair(id, unusedIndex));
             touchTrigger._touches.push_back(touch);
         }
        else
        {
            Touch* touch = _touches[iter->second];
            if (touch)
            {
                  touch->setTouchInfo(iter->second, x, y);

                  touchEvent._touches.push_back(touch);

                  if(action == TouchTrigger::TouchAction::DOWN || action == TouchTrigger::TouchAction::CANCLE)
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
                return;
            }

        }

    }

    if (touchTrigger._touches.size() == 0)
    {
         FKLOG("touchesBegan: size = 0");
         return;
    }

    touchTrigger._action = action;
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->dispatchEvent(&touchEvent);

    if(action == TouchTrigger::TouchAction::DOWN || action == TouchTrigger::TouchAction::CANCLE)
    {
        for (auto& touch : touchTrigger._touches)
        {
            // release the touch object.
            touch->release();
        }
    }

}

FLAKOR_NS_END

