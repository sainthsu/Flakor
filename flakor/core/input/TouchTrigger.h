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

#ifndef _FK_TOUCHTRIGGER_H_
#define _FK_TOUCHTRIGGER_H_

#include "base/lang/Object.h"
#include <vector>

FLAKOR_NS_BEGIN

/**
 * @addtogroup input
 * @{
 */
class Touch;

class TouchTrigger : public Object
{
public:
    /**最大的同时触摸点 */
    static const int MAX_TOUCHES = 15;

    /**触摸动作  touch action */
    enum class TouchAction {
        NONE,
        DOWN,
        MOVE,
        UP,
        CANCEL
    };

    std::vector<Touch*> _touches;

    TouchTrigger();
    inline void setAction(const TouchAction action) {_action = action;};
    inline TouchAction getAction() const { return _action;};
    inline const std::vector<Touch*>& getTouches() const { return _touches; };

private:
    TouchAction _action;
};

// end of input group
/// @}

FLAKOR_NS_END

#endif  // _FK_TOUCHTRIGGER_H_
