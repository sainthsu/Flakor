#pragma once
#include <assert.h>

namespace flakor
{
#define MALLOC_SIZE	128

	typedef struct tagDataHead
	{
		tagDataHead()
		{
			pHead = NULL;
			nLen = 0;
			pNext = NULL;
		}
		void ReSet()
		{
			pHead = NULL;
			nLen = 0;
			pNext = NULL;
		}
		char* pHead;
		unsigned int nLen;
		tagDataHead* pNext;
	}DATAHEAD;

	typedef struct tagDataMem
	{
		tagDataMem(unsigned int nSize)
		{
			if (0 == nSize)
			{
				assert(false);
				return ;
			}

			nMaxSize = nSize;
			pDataMem = (char*)malloc(nSize*sizeof(char));
			nDataPos = 0;

			if (NULL == pDataMem)
			{
				// CTwoQueues申请malloc内存失败。
				assert(false);
			}

			pListDataHead = NULL;
			pCurDataHead = NULL;
			pFreeDataHead = NULL;
		}
		~tagDataMem()
		{
			// 释放节点
			ReSet();
			while(NULL != pFreeDataHead)
			{
				DATAHEAD* pTempDataHead = pFreeDataHead;
				pFreeDataHead = pFreeDataHead->pNext;

				delete pTempDataHead;
				pTempDataHead = NULL;
			}

			free(pDataMem);
			pDataMem = NULL;
			nDataPos = 0;
		}
		bool ReAlloc()
		{
			for (unsigned short i=0; i<MALLOC_SIZE; ++i)
			{
				DATAHEAD* pTempDataHead = new DATAHEAD;
				//pTempDataHead->ReSet();	//构造时已经初始化

				if (NULL == pTempDataHead)
				{
					// 申请DATAHEAD内存失败。
					assert(false);
					return false;
				}

				pTempDataHead->pNext = pFreeDataHead;
				pFreeDataHead = pTempDataHead;
			}

			return true;
		}
		DATAHEAD* GetDataHead()
		{
			if (NULL != pFreeDataHead)
			{
				DATAHEAD* pTempDataHead = pFreeDataHead;
				pFreeDataHead = pFreeDataHead->pNext;

				return pTempDataHead;
			}

			if (ReAlloc())
			{
				if (NULL != pFreeDataHead)
				{
					DATAHEAD* pTempDataHead = pFreeDataHead;
					pFreeDataHead = pFreeDataHead->pNext;

					return pTempDataHead;
				}
			}

			// ASSERT("GetDataHead返回NULL。");
			assert(false);
			return NULL;
		}
		unsigned int GetFreeLen()	//空闲内存长度
		{
			return nMaxSize-nDataPos;
		}
		bool PushData(void* pData, unsigned int nLen)
		{
			if (nDataPos+nLen >= nMaxSize)
			{
				return false;
			}

			DATAHEAD* pTempDataHead = GetDataHead();

			if (NULL == pTempDataHead)
			{
				return false;
			}

			// 构造数据头结构
			pTempDataHead->pHead = (pDataMem+nDataPos);
			pTempDataHead->nLen = nLen;
			pTempDataHead->pNext = NULL;

			// 拷贝数据
			memcpy(pDataMem+nDataPos, pData, nLen);
			nDataPos += nLen;

			if (NULL == pListDataHead)
			{
				pListDataHead = pTempDataHead;
				pCurDataHead = pTempDataHead;
				return true;
			}
			else
			{
				pCurDataHead->pNext = pTempDataHead;
				pCurDataHead = pCurDataHead->pNext;
				return true;
			}
		}

		bool IsEmpty()	//判断是否有数据
		{
			return (NULL==pListDataHead)?true:false;
		}

		bool PrepareData(const void*& pData, unsigned int& nLen)	//准备一条数据
		{
			if (NULL != pListDataHead)
			{
				pData = pListDataHead->pHead;
				nLen = pListDataHead->nLen;
				return true;
			}
			else
			{
				return false;
			}
		}
		void ConfimData()	//删除一条数据
		{
			if (NULL == pListDataHead)
			{
				return ;
			}

			DATAHEAD* pTempDataHead = pListDataHead;
			pListDataHead = pListDataHead->pNext;

			pTempDataHead->ReSet();
			pTempDataHead->pNext = pFreeDataHead;
			pFreeDataHead = pTempDataHead;
		}
		void ReSet()	//重置内存存储对象
		{
			while(NULL != pListDataHead)
			{
				DATAHEAD* pTempDataHead = pListDataHead;
				pListDataHead = pListDataHead->pNext;

				pTempDataHead->ReSet();
				pTempDataHead->pNext = pFreeDataHead;
				pFreeDataHead = pTempDataHead;
			}

			nDataPos = 0;
			pCurDataHead = NULL;
		}

		char* pDataMem;			//数据内存区域
		unsigned int nDataPos;	//数据存储位置
		unsigned int nMaxSize;	//最大存储区域大小

		DATAHEAD* pListDataHead;
		DATAHEAD* pCurDataHead;
		DATAHEAD* pFreeDataHead;	//空闲头结构队列
	}DATAMEM;

	class CTwoQueues
	{
	public:
		CTwoQueues(void)
		{
			InitializeCriticalSection(&m_crit);
			m_pDataMemPush = NULL;
			m_pDataMemPop = NULL;
		}
		~CTwoQueues(void)
		{
			if (NULL != m_pDataMemPush)
			{
				delete m_pDataMemPush;
				m_pDataMemPush = NULL;
			}

			if (NULL != m_pDataMemPop)
			{
				delete m_pDataMemPop;
				m_pDataMemPop = NULL;
			}
			DeleteCriticalSection(&m_crit);
		}

	public:
		void Init(unsigned int nSize)
		{
			if (0 == nSize)
			{
				// 初始化CTwoQueues对象失败。
				assert(false);
				return ;
			}

			m_pDataMemPush = new DATAMEM(nSize);
			m_pDataMemPop = new DATAMEM(nSize);
		}

		bool PushData(void* pData, unsigned int nLen)
		{
			//unsigned int nFreeLen = m_pDataMemPush->GetFreeLen();

			bool bResult = false;

			EnterCriticalSection(&m_crit);
			bResult = m_pDataMemPush->PushData(pData, nLen);
			LeaveCriticalSection(&m_crit);

			return bResult;
		}
		bool PrepareData(const void*& pData, unsigned int& nLen)
		{
			bool bCanRead = true;
			if (m_pDataMemPop->IsEmpty())
			{
				// 队列没有数据了
				EnterCriticalSection(&m_crit);
				if (m_pDataMemPush->IsEmpty())
				{
					// Push队列为空
					LeaveCriticalSection(&m_crit);
					bCanRead = false;
				}
				else
				{
					m_pDataMemPop->ReSet();	//充值读取队列
					DATAMEM* pTempDataMem = m_pDataMemPush;
					m_pDataMemPush = m_pDataMemPop;
					m_pDataMemPop = pTempDataMem;
					LeaveCriticalSection(&m_crit);
					bCanRead = true;
				}
			}

			if (bCanRead)
			{
				return m_pDataMemPop->PrepareData(pData, nLen);
			}
			else
			{
				return false;
			}
		}

		void ConfimData()
		{
			m_pDataMemPop->ConfimData();
		}

	private:
		DATAMEM* m_pDataMemPush;
		DATAMEM* m_pDataMemPop;
		CRITICAL_SECTION m_crit;

	};
}

