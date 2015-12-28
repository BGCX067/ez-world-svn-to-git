#ifndef __MEMORYCHUNK_H__
#define __MEMORYCHUNK_H__
#include "xlist.h"

namespace MemPool
{

/*!\class CList
 * \brief 
 *
 * It is responsible for assistant Memory manager.
 */
template <class T>
class CMemoryChunk
{
	typedef struct _node_
	{
		T		*		data;
		_node_	*		pnext;
		_node_	*		pprev;
		unsigned int	nextfree;
	}st_node;
	/* 链表信息 */
	typedef struct _list_
	{
		unsigned int	m_free;				//可用结点编号
		unsigned int	m_totel;			//当前长度
		unsigned int	m_nMaxCount;		//最大长度
		st_node	*		m_pArray;			//指向已分配节点
		st_node *		m_pHead;			//指向第一个已分配节点的前一个
		st_node *		m_pThrough;			//用于叠代不适用多个叠代器同时叠代的情况
		st_node *		m_pTail;			//指向最后一个已分配的节点
		/// 是否已经申请空间
		inline BOOL	IsCreated()
		{
			return (m_pArray != NULL);
		}
		/// 检测是否在当前的链中
		inline bool InLink(T *pt)
		{
			return (st_node *)pt>m_pArray && (st_node *)pt<=m_pArray+m_nMaxCount;
		}
		/// 创建指定大小的链表
		bool Create(int iMaxCount,int nPerSize)
		{
			m_nMaxCount = iMaxCount;
			m_pArray = new st_node[m_nMaxCount+1];
			for (unsigned int i=0;i<=m_nMaxCount;i++)
			{
				(m_pArray+i)->data = new T[nPerSize];
			}
			return Clean();
		}
		/// 销毁链表
		void Destroy()
		{
			Clean();
			if( m_pArray != NULL )
			{
				for (unsigned int i=0;i<=m_nMaxCount;i++)
				{
					delete [] ((m_pArray+i)->data);
				}
				delete []m_pArray;
				m_pArray = NULL;
			}
			m_nMaxCount = 0;
		}
		/// 创建后的清除
		bool Clean()
		{
			if(m_pArray == NULL)
			{
				return false;
			}
			unsigned int i = 0;
			for(i=0;i<=m_nMaxCount;i++)
			{
				m_pArray[i].pnext = NULL;
				m_pArray[i].pprev = NULL;
				m_pArray[i].nextfree = i + 1;
			}
			m_pArray[m_nMaxCount].nextfree = 0;
			m_pArray[m_nMaxCount].pnext = NULL;
			m_pArray[m_nMaxCount].pprev = NULL;
			m_free = 1;
			m_pHead = &m_pArray[0];
			m_pTail = m_pHead;
			m_pThrough = m_pHead;
			m_totel = 0;
			return true;
		}
		/// 取得当前链表已用大小
		inline unsigned int GetCount()
		{
			return m_totel;
		}
		/// 取得当前链表的总大小
		inline unsigned int GetMaxCount()
		{
			return m_nMaxCount;
		}
		/// 复原
		inline int Reset()
		{
			if(IsCreated())
			{
				m_pThrough = m_pHead;
				return 1;
			}
			return 0;
		}
		/// 申请数据空间
		inline unsigned int New(T ** t)
		{
			if(IsCreated() )
			{
				unsigned int id = AllocId();
				if( id > 0 && id <= m_nMaxCount )
				{
					*t = m_pArray[id].data;
					m_pTail->pnext = &m_pArray[id];
					m_pArray[id].pprev = m_pTail;
					m_pArray[id].pnext = NULL;
					m_pTail = &m_pArray[id];
					m_totel ++;
					return id;
				}
			}
			return 0;
		}
		/// 回收数据空间
		inline int Del(unsigned int id)
		{
			int nRet = 0;
			assert(id&&id <= m_nMaxCount);
			if(IsCreated() && (id != 0) && (id <= m_nMaxCount))
			{
				if(m_pArray[id].pprev != NULL)
				{
					if(m_pThrough == &m_pArray[id])
						m_pThrough = m_pArray[id].pprev;
					m_pArray[id].pprev->pnext = m_pArray[id].pnext;

					if(m_pArray[id].pnext != NULL)
						m_pArray[id].pnext->pprev = m_pArray[id].pprev;
					else
						m_pTail = m_pTail->pprev;

					m_pArray[id].pprev = NULL;
					m_pArray[id].pnext = NULL;
					ResaveId( id );
					m_totel --;

					nRet = 1;
				}//else _asm int 3;
			}
			return nRet;
		}
		/// 取得指定位置的数据
		inline T *Get(unsigned int id)
		{
			if(IsCreated() && (id != 0) && (id <= m_nMaxCount))
			{
				if(m_pArray[id].pnext == NULL && m_pArray[id].pprev == NULL)
					return NULL;
				else
					return m_pArray[id].data;
			}
			return NULL;
		}
		/// 申请空间位置
		unsigned int AllocId()
		{
			unsigned int ret = 0;
			if(IsCreated())
			{
				ret = m_free;
				if(ret != 0)
				{
					m_free = m_pArray[ret].nextfree;
				}
			}
			return ret;
		}
		/// 释放空间位置
		int ResaveId(unsigned int id)
		{
			int nRet = 0;
			if(IsCreated() && (id <= m_nMaxCount) && (id != 0))
			{
				m_pArray[id].nextfree = m_free;
				m_free = id;
				nRet = 1;
			}
			return nRet;
		}

	}st_list;
public:
	CMemoryChunk()
	:m_bThreadSafe(false)
	,m_plst(NULL)
	,m_nCnt(0)
	,m_nBlockSize(0)
	,m_nBlockCnt(0)
	,m_nThough(0){}
	BOOL Create(int nMaxCount,int nPerSize)
	{
		m_nBlockCnt = nMaxCount;
		m_nBlockSize = nPerSize;
		// 初始化临界对象
		//InitializeCriticalSection( &m_CriticalSection );
		return TRUE;
	}
	VOID Destroy()
	{
		for(unsigned int i=0;i<m_nCnt;i++)
		{
			m_plst[i].Destroy();
		}
		if(m_plst)
		{
			delete []m_plst;
			m_plst = NULL;
		}
		//移除临界对象
		//DeleteCriticalSection(&m_CriticalSection);	
	}
	// 创建后的清除
	BOOL Clean()
	{
		//cLock lock(&m_CriticalSection);
		if(m_nCnt)
		{
			for(unsigned int i=0;i<m_nCnt;i++)
			{
				m_plst[i].Clean();
			}
		}
		m_nCnt = 0;
		m_nBlockSize = 0;
		m_nBlockCnt = 0;
		m_nThough = 0;
		return TRUE;
	}
	virtual ~CMemoryChunk()
	{
		Destroy();
	}
	inline unsigned int GetCount()
	{
		unsigned int nSum = 0;
		for(unsigned int i=0;i<m_nCnt;i++)
		{
			nSum += m_plst[i].GetCount();
		}
		return nSum;
	}
	inline unsigned int GetMaxCount()
	{
		unsigned int nMaxCount = 0;
		for(unsigned int i=0;i<m_nCnt;i++)
		{
			nMaxCount += m_plst[i].GetMaxCount();
		}
		return nMaxCount;
	}
	int Reset()
	{
		//cLock lock(&m_CriticalSection);
		for(unsigned int i=0;i<m_nCnt;i++)
		{
			m_plst[i].Reset();
		}
		return 1;
	}
	inline VOID Lock()
	{
		//EnterCriticalSection(&m_CriticalSection);
	}
	inline VOID	Unlock()
	{
		//LeaveCriticalSection(&m_CriticalSection);	
	}
	unsigned int New(T ** t)
	{
		//cLock lock(&m_CriticalSection);
		unsigned int nRet = 0;
		for(unsigned int i=0;i<m_nCnt;i++)
		{
			if( (nRet=m_plst[i].New(t)) )
			{
				nRet = (i<<16) | (nRet&0xffff);
				break;
			}
		}
		if(!nRet && Enlarge())
		{
			nRet = m_plst[m_nCnt-1].New(t);
			if (nRet)
			{
				nRet = ((m_nCnt-1)<<16) | (nRet&0xffff);
			}
		}
		return nRet;
	}
	int	Del(unsigned int id)
	{
		//cLock lock(&m_CriticalSection);
		int nRet = 0;
		unsigned int n1d = (id&0xffff0000)>>16;
		unsigned int n2d = id&0xffff;
		if(n1d >= 0 && n1d < m_nCnt)
		{
			nRet = m_plst[n1d].Del(n2d);
		}
		return nRet;
	}
	T * Get(unsigned int id)
	{
		//cLock lock(&m_CriticalSection);
		T* pRet = NULL;
		unsigned int n1d = (id&0xffff0000)>>16;
		unsigned int n2d = id&0xffff;
		if(n1d >= 0 && n1d < m_nCnt)
		{
			pRet = m_plst[n1d].Get(n2d);
		}
		return pRet;
	}
private:
	bool Enlarge()
	{
		++m_nCnt;
		//__assert(m_nCnt<10);
		st_list *pList = new st_list[m_nCnt];
		ZeroMemory(pList,sizeof(st_list)*m_nCnt);
		if(m_plst)
		{
			memcpy(pList,m_plst,sizeof(st_list)*(m_nCnt-1));
			delete []m_plst;
		}
		m_plst = pList;
		m_plst[m_nCnt-1].Create(m_nBlockCnt,m_nBlockSize);

		//_printf("Logic engine: MemoryPool Alloc %d kb!\n",(m_nBlockCnt+1)*m_nBlockSize/1024);

		return true;
	}
private:
	//CRITICAL_SECTION m_CriticalSection;	//线程同步
	bool			m_bThreadSafe;		//线程安全开关
	st_list		*	m_plst;				//三维链头
	unsigned int	m_nCnt;				//三维链表大小
	unsigned int	m_nBlockSize;		//二维块大小
	unsigned int	m_nBlockCnt;		//二维块数量
	unsigned int	m_nThough;			//当前所在的二维块

};
}
#endif