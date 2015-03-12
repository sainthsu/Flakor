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

#ifndef _FK_TOUCHPOOL_H_
#define _FK_TOUCHPOOL_H_

#include "base/lang/Object.h"
#include "core/input/TouchTrigger.h"
#include <stddef.h>

FLAKOR_NS_BEGIN

/**
 * @addtogroup input
 * @{
 */

class TouchPool
{
protected:
    static unsigned int g_indexBitsUsed = 0;
    Touch* _touches[TouchTrigger::MAX_TOUCHES];
    // System touch pointer ID (It may not be ascending order number) <-> Ascending order number from 0
    std::map<intptr_t, int> _touchIdReorderMap;

	TouchTrigger* currentTrigger;
public:
    TouchPool();

    int getUnUsedIndex();
    std::vector<Touch*> getAllTouchesVector();
    void removeUsedIndexBit(int index);
    Touch* find(intptr_t pointId);

    bool handleTouch(TouchTrigger::TouchAction action,int count,intptr_t ids[],float xs[],float ys[]);

	inline TouchTrigger* getCurrentTouch(){ return currentTrigger;};
};

// end of input group
/// @}

FLAKOR_NS_END

#endif  // _FK_TOUCH_H_
