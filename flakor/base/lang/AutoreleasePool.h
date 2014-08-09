/****************************************************************************
Copyright (c) 2014 flakor.org

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
#ifndef _FK_AUTORELEASEPOOL_H_
#define _FK_AUTORELEASEPOOL_H_

#include "Object.h"
#include "Array.h"

FLAKOR_NS_BEGIN

/**
 * @addtogroup lang
 * @{
 * @js NA
 * @lua NA
 */

class AutoreleasePool : public Object
{
    Array*    m_pManagedObjectArray;    
public:
    AutoreleasePool(void);
    ~AutoreleasePool(void);

    void addObject(Object *pObject);
    void removeObject(Object *pObject);

    void clear();
};

/**
 * @js NA
 * @lua NA
 */
class PoolManager
{
    Array*    m_pReleasePoolStack;    
    AutoreleasePool*  m_pCurReleasePool;

    AutoreleasePool* getCurReleasePool();
public:
    PoolManager();
    ~PoolManager();
    void finalize();
    void push();
    void pop();

    void removeObject(Object* pObject);
    void addObject(Object* pObject);

    static PoolManager* sharedPoolManager();
    static void purgePoolManager();

    friend class AutoreleasePool;
};

// end of base_nodes group
/// @}

FLAKOR_NS_END

#endif //_FK_AUTORELEASEPOOL_H_
