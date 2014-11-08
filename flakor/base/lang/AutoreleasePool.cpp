/****************************************************************************
Copyright (c) 2010 flakor.org

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
#include "macros.h"
#include "AutoreleasePool.h"


FLAKOR_NS_BEGIN

static PoolManager* s_pPoolManager = NULL;

AutoreleasePool::AutoreleasePool(void)
{
    m_pManagedObjectArray = new Array();
    m_pManagedObjectArray->init();
}

AutoreleasePool::~AutoreleasePool(void)
{
    FK_SAFE_DELETE(m_pManagedObjectArray);
}

void AutoreleasePool::addObject(Object* pObject)
{
    m_pManagedObjectArray->addObject(pObject);

    FKAssert(pObject->m_uReference > 1, "reference count should be greater than 1");
    ++(pObject->m_uAutoReleaseCount);
    pObject->release(); // no ref count, in this case autorelease pool added.
}

void AutoreleasePool::removeObject(Object* pObject)
{
    for (unsigned int i = 0; i < pObject->m_uAutoReleaseCount; ++i)
    {
        m_pManagedObjectArray->removeObject(pObject, false);
    }
}

void AutoreleasePool::clear()
{
    if(m_pManagedObjectArray->count() > 0)
    {
        //AutoreleasePool* pReleasePool;
#ifdef _DEBUG
        int nIndex = m_pManagedObjectArray->count() - 1;
#endif

        Object* pObj = NULL;
        FK_ARRAY_FOREACH_REVERSE(m_pManagedObjectArray, pObj)
        {
            if(!pObj)
                break;

            --(pObj->m_uAutoReleaseCount);
            //(*it)->release();
            //delete (*it);
#ifdef _DEBUG
            nIndex--;
#endif
        }

        m_pManagedObjectArray->removeAllObjects();
    }
}


//--------------------------------------------------------------------
//
// PoolManager
//
//--------------------------------------------------------------------

PoolManager* PoolManager::sharedPoolManager()
{
    if (s_pPoolManager == NULL)
    {
        s_pPoolManager = new PoolManager();
    }
    return s_pPoolManager;
}

void PoolManager::purgePoolManager()
{
    FK_SAFE_DELETE(s_pPoolManager);
}

PoolManager::PoolManager()
{
    m_pReleasePoolStack = new Array();    
    m_pReleasePoolStack->init();
    m_pCurReleasePool = 0;
}

PoolManager::~PoolManager()
{
    
     finalize();
 
     // we only release the last autorelease pool here 
     m_pCurReleasePool = 0;
     m_pReleasePoolStack->removeObjectAtIndex(0);
 
     FK_SAFE_DELETE(m_pReleasePoolStack);
}

void PoolManager::finalize()
{
    if(m_pReleasePoolStack->count() > 0)
    {
        //CCAutoreleasePool* pReleasePool;
        Object* pObj = NULL;
        FK_ARRAY_FOREACH(m_pReleasePoolStack, pObj)
        {
            if(!pObj)
                break;
            AutoreleasePool* pPool = (AutoreleasePool*)pObj;
            pPool->clear();
        }
    }
}

void PoolManager::push()
{
    AutoreleasePool* pPool = new AutoreleasePool();       //ref = 1
    m_pCurReleasePool = pPool;

    m_pReleasePoolStack->addObject(pPool);                   //ref = 2

    pPool->release();                                       //ref = 1
}

void PoolManager::pop()
{
    if (! m_pCurReleasePool)
    {
        return;
    }

     int nCount = m_pReleasePoolStack->count();

    m_pCurReleasePool->clear();
 
      if(nCount > 1)
      {
        m_pReleasePoolStack->removeObjectAtIndex(nCount-1);

//         if(nCount > 1)
//         {
//             m_pCurReleasePool = m_pReleasePoolStack->objectAtIndex(nCount - 2);
//             return;
//         }
        m_pCurReleasePool = (AutoreleasePool*)m_pReleasePoolStack->objectAtIndex(nCount - 2);
    }

    /*m_pCurReleasePool = NULL;*/
}

void PoolManager::removeObject(Object* pObject)
{
    FKAssert(m_pCurReleasePool, "current auto release pool should not be null");

    m_pCurReleasePool->removeObject(pObject);
}

void PoolManager::addObject(Object* pObject)
{
    getCurReleasePool()->addObject(pObject);
}


AutoreleasePool* PoolManager::getCurReleasePool()
{
    if(!m_pCurReleasePool)
    {
        push();
    }

    FKAssert(m_pCurReleasePool, "current auto release pool should not be null");

    return m_pCurReleasePool;
}

FLAKOR_NS_END
