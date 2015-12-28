#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stddef.h>
#include "vwassert.h"

/** \ingroup vwutil
  * \brief
  *	构造函数锁定critical section,析构函数解锁critical section
  * Describe:
  */
/*

/** \ingroup vwutil
  * \brief
  *	总共可分配得单元数在初始化时决定,不可扩展，带构造和析构。每个分配单元为T。
  * Describe:
  */
template < class T >
class CUnitAllocator
{
	typedef struct	_node_
	{
		T				data;
		_node_	*		pnext;
		_node_	*		pprev;
		//unsigned int	nextfree;
	}st_node;
	//CRITICAL_SECTION m_CriticalSection;
	unsigned int	m_allocated;
	st_node	*		m_pArray;

	st_node *		m_pHead;		//指向第一个已分配节点的前一个
	st_node *		m_pTail;		//指向最后一个已分配的节点

	st_node *		m_pfree;

	unsigned		m_nMaxCount;


public:
	CUnitAllocator()
	{
		m_pArray = NULL;
		m_pHead = NULL;
		m_pTail = NULL;
		m_pfree = 0;

		m_allocated = 0;
		m_nMaxCount = 0;

	}
	inline bool IsCreated()
	{
		return m_pArray!=NULL;
	}
	/**
	  * \brief
	  * 获得单元的Id
	  * Describe:
	  * \param *pt 
	  * \return 
	  */
	inline unsigned GetId(T *pt)
	{
		st_node *pnode=(st_node *)((char *)pt-offsetof(st_node,data));
		vwassert(pnode>=m_pArray&&(st_node *)pt<m_pArray+m_nMaxCount);
		vwassert(((char *)pnode-(char *)m_pArray)%sizeof(st_node)==0);
		return(unsigned) (pnode-m_pArray);
	}
	/**
	  * \brief
	  *	初始化内存池。
	  * Describe:
	  * \param iMaxCount 有iMaxCount个分配单元可供分配。
	  * \return 
	  */
	bool	Create(int iMaxCount )
	{
		vwassert(iMaxCount);
		vwassert(!m_pArray);

		m_nMaxCount = iMaxCount;
		m_pArray = new st_node[m_nMaxCount];
		//InitializeCriticalSection( &m_CriticalSection );
		
		if( m_pArray == NULL )
			return true;
		unsigned int i = 0;
		for( i = 0;i <m_nMaxCount-1;i ++ )
		{
			m_pArray[i].pnext = m_pArray+i+1;
			m_pArray[i].pprev = NULL;
		}
		m_pArray[m_nMaxCount-1].pnext=NULL;
		m_pArray[m_nMaxCount-1].pprev=NULL;
		m_pfree=m_pArray;
		return false;
	}

	/*
	VOID	Destroy( )
	{
		Clean();
		if( m_pArray != NULL )
		{
			delete []m_pArray;
			m_pArray = NULL;
			//DeleteCriticalSection( &m_CriticalSection );
		}
		m_nMaxCount = 0;
	}
	*/
	~CUnitAllocator()
	{
		if(m_pArray)
		{
			T *pcur;
			for(pcur=NextData();pcur;pcur=NextData(pcur))
			{
				pcur->T::~T();
			}
			delete []m_pArray;
		}
	}

	/**
	  * \brief
	  *	返回已分配单元数。	
	  * Describe:
	  * \return 
	  */
	unsigned int GetCount()
	{
		return m_allocated;
	}

	/**
	  * \brief
	  *	取得已分配和虽未分配但可用来分配的单元的总数。
	  * Describe:
	  * \return 
	  */
	unsigned int GetCapacity()
	{
		return m_nMaxCount;
	}

	/*
	int Reset()
	{
		//cLock lock(&m_CriticalSection);
		if( IsCreated() )
		{
			//m_pThrough = m_pHead;
			return 1;
		}
		return 0;
	}
	inline VOID		Lock()
	{
		EnterCriticalSection( &m_CriticalSection );
	}
	inline VOID		Unlock()
	{
		LeaveCriticalSection( &m_CriticalSection );	
	}
	*/
	/**
	  * \brief
	  * 按照分配的顺序返回下一个单元的地址
	  * Describe:
	  * \param *pcur 当前单元的地址,NULL将返回第一个单元的地址
	  * \return pcur 的下一个元素,如果未初始化直接返回NULL
	  */
	inline T * NextData(T *pcur=NULL)
	{
		//vwassert(IsCreated());
		st_node *pnext=pcur?((st_node *)((char *)pcur-offsetof(st_node,data)))->pnext:m_pHead;
		return pnext?&pnext->data:(reinterpret_cast<T *>(NULL));
	}

	/**
	  * \brief
	  *	在内存池中新分配一个的单元，返回所分配的内存单元的地址。
	  * Describe:
	  * \return 
	  */
	T *New()
	{
		//cLock lock(&m_CriticalSection);
		vwassert(IsCreated());


		if(!m_pfree)
			return NULL;
		st_node *pallocate;
		pallocate=m_pfree;

		if(m_pfree)
			m_pfree=m_pfree->pnext;

		pallocate->pprev = m_pTail;
		pallocate->pnext = NULL;

		if(m_pTail)
			m_pTail->pnext=pallocate;


		m_pTail=pallocate;


		if(!m_pHead)
			m_pHead=pallocate;

		m_allocated ++;


		new (&pallocate->data) T;
		return &pallocate->data;
	}


	/**
	  * \brief
	  *	删除已分配的单元
	  * Describe:
	  * \param *p 
	  * \return 
	  */
	inline void	Del(T *p)
	{
		vwassert(IsCreated());

		unsigned id=GetId(p);
		//cLock lock(&m_CriticalSection);
		int nRet = 0;
		//vwassert(IsAllocated(p);

		st_node *pprev,*pnext;

		st_node *pnode=(st_node *)((char *)p-offsetof(st_node,data));

		p->T::~T();

		pprev=pnode->pprev;
		pnext=pnode->pnext;
		if(pprev)
		{
			pprev->pnext=pnext;
		}else
			m_pHead=pnext;

		if(pnext)
		{
			pnext->pprev=pprev;
		}else
			m_pTail=pprev;

		pnode->pnext=m_pfree;
		m_pfree=pnode;

		m_allocated --;
	}

	/*
	BOOL IsAllocated(T *p)
	{
		return ((st_node *)p)->pprev!=NULL;
	}
	*/

	T *	Get(unsigned int id)
	{
		//cLock lock(&m_CriticalSection);
		vwassert(IsCreated());
		vwassert(id<m_nMaxCount);
		if(id<m_nMaxCount)
		{
			return &m_pArray[id].data;
		}
		return NULL;
	}

	/**
	  * \brief
	  * 移到已分配单元列表的尾部,这样在NextData枚举中该单元将最后被枚举到，单元的实际地址不被变更，变更的仅仅是链接顺序
	  * Describe:
	  * \param *pcell 指向已分配的单元
	  */
	/*
	void MoveToTail(T *pcell)
	{
		//cLock lock(&m_CriticalSection);
		unsigned id=GetId(pcell);

		if( IsCreated() && (id != 0) && ( id <= m_nMaxCount ))
		{
			if( m_pArray[id].pprev != NULL )
			{
				// 删除
				//if( m_pThrough == &m_pArray[id] )
				//	m_pThrough = m_pArray[id].pprev;
				m_pArray[id].pprev->pnext = m_pArray[id].pnext;

				if( m_pArray[id].pnext != NULL )
					m_pArray[id].pnext->pprev = m_pArray[id].pprev;
				else
					m_pTail = m_pTail->pprev;

				m_pArray[id].pprev = NULL;
				m_pArray[id].pnext = NULL;

				// 添加到末尾
				m_pTail->pnext = &m_pArray[id];
				m_pArray[id].pprev = m_pTail;
				m_pArray[id].pnext = NULL;
				m_pTail = &m_pArray[id];
			}
		}
	}
	*/

};
