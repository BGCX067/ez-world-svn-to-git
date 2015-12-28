#pragma once
#include <Windows.h>
#include <assert.h>
#include "support.h"
#include "_assert.h"
#include "ModuleSupport.h"

#define XLIST_LEAK_CHECK	0

class cLock
{
public:
	cLock(CRITICAL_SECTION * pCriticalSection)
	{
		m_pCriticalSection = pCriticalSection;
		if(m_pCriticalSection)
			EnterCriticalSection( m_pCriticalSection );//锁定
	}
	~cLock()
	{
		if(m_pCriticalSection)
			LeaveCriticalSection( m_pCriticalSection );//解锁
		m_pCriticalSection = NULL;
	}
private:
	CRITICAL_SECTION * m_pCriticalSection;//线程同步创建临界对象
};

template <class T>
class cXList
{
#ifdef _DEBUG
	unsigned m_renterflag;
	bool		bcheckflag;
#endif
	/* 结点信息 */
	typedef struct	_node_
	{
		T				data;				//数据郿
		_node_	*		pnext;				//后一结点
		_node_	*		pprev;				//前一结点
		unsigned int	nextfree;			//下一个空闲结炿
#if XLIST_LEAK_CHECK		
		char			src_file[32];
		unsigned int	src_line;
#endif
	}st_node;
	/* 链表信息 */
	typedef struct _list_
	{
		unsigned int	m_free;				//可用结点编号
		unsigned int	m_totel;			//当前长度
		unsigned int	m_nMaxCount;		//最大长庿
		st_node	*		m_pArray;			//指向已分配节炿
		st_node *		m_pHead;			//指向第一个已分配节点的前一丿
		st_node *		m_pThrough;			//用于叠代不适用多个叠代器同时叠代的情况
		st_node *		m_pTail;			//指向最后一个已分配的节炿
		/// 是否已经申请空间
		inline BOOL	IsCreated()
		{
			return (m_pArray != NULL);
		}
		/// 检测是否在当前的链丿
		inline bool InLink(T *pt)
		{
			return (st_node *)pt>m_pArray && (st_node *)pt<=m_pArray+m_nMaxCount;
		}
		/// 取得当前数据所在链表中的位罿
		inline unsigned GetId(T *pt)
		{
			assert((st_node *)pt>m_pArray&&(st_node *)pt<=m_pArray+m_nMaxCount);
			assert(((size_t)(st_node *)pt-(size_t)m_pArray)%sizeof(st_node)==0);
			return (unsigned)((st_node *)pt-m_pArray);
		}
		/// 创建指定大小的链衿
		bool Create(int iMaxCount)
		{
			m_nMaxCount = iMaxCount;
			m_pArray = new st_node[m_nMaxCount+1];
			return Clean();
		}
		/// 销毁链衿
		void Destroy()
		{
			Clean();
			if(m_pArray != NULL)
			{
				delete []m_pArray;
				m_pArray = NULL;
			}
			m_nMaxCount = 0;
		}
		/// 创建后的清除
		bool Clean()
		{
			if(m_pArray == NULL)
				return false;
			unsigned int i = 0;
			for(i = 0;i <= m_nMaxCount;i ++)
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
		/// 取得当前链表的总大尿
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
		/// 取得指定数据的下一条数据，默认取头
		inline T * NextData(T *pcur=NULL)
		{
			assert(IsCreated());
			st_node *pnext=pcur?((st_node *)pcur)->pnext:m_pHead->pnext;
			return pnext?&pnext->data:NULL;
		}
		/// 头数捿建议不再使用,使用NextData
		inline T * First()
		{
			if(!IsCreated())
				return 0;
			if(m_pHead == NULL)
				return NULL;
			m_pThrough = m_pHead->pnext;
			if(m_pThrough != NULL)
				return &m_pThrough->data;
			return NULL;
		}
		/// 当前数据,建议不再使用,使用NextData
		inline T * Cur()
		{
			if(!IsCreated())
				return 0;
			if(m_pThrough != NULL && m_pThrough != m_pHead)
				return &m_pThrough->data;
			return NULL;
		}
		/// 下一数据,建议不再使用,使用NextData
		inline T * Next()
		{
			if(!IsCreated())
				return 0;
			if(m_pThrough != NULL)
				m_pThrough = m_pThrough->pnext;
			if(m_pThrough != NULL)
				return &m_pThrough->data;
			return NULL;
		}
		/// 头数捿
		inline T * Beg()
		{
			if(!IsCreated())
				return 0;
			if(m_pHead != NULL)
				return &m_pHead->data;
			return NULL;
		}
		/// 尾数捿建议不再使用
		inline T * End()
		{
			if(!IsCreated())
				return 0;
			if(m_pTail != NULL)
				return &m_pTail->data;
			return NULL;
		}
		
		inline const char * f_Name(const char *path){
			size_t length, l;
			l = length = _tcslen(path)-1;
			while (length > 0 && path[length] != '\\' && path[length] != '/'){
				length--;
			}
			return &path[length+1];
		}		
		/// 申请数据空间
		inline unsigned int New(T ** t,const char * src_file,unsigned int src_line)
		{
			if(IsCreated())
			{
				unsigned int id = AllocId();
				if(id > 0 && id <= m_nMaxCount)
				{
					*t = &m_pArray[id].data;
					m_pTail->pnext = &m_pArray[id];
					m_pArray[id].pprev = m_pTail;
					m_pArray[id].pnext = NULL;
#if XLIST_LEAK_CHECK		
					m_pArray[id].src_line = src_line;
					strcpy_s(m_pArray[id].src_file,32,f_Name(src_file));
#endif
					m_pTail = &m_pArray[id];
					m_totel ++;
					return id;
				}
			}
			return 0;
		}
		/// 回收数据空间
		inline int Del(T *p)
		{
			return Del((unsigned )((st_node *)p-m_pArray));
		}
		/// 指定的数据是否属于该链表空间
		inline BOOL IsAllocated(T *p)
		{
			return ((st_node *)p)->pprev!=NULL;
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
					ResaveId(id);
					m_totel --;

					nRet = 1;
				}
				else
				{
					// 节点已经被释放过亿
					//_asm int 3;
				}
			}
			return nRet;
		}
		/// 取得指定位置的数捿
		inline T *Get(unsigned int id)
		{
			if(IsCreated() && (id != 0) && (id <= m_nMaxCount))
			{
				if(m_pArray[id].pnext == NULL && m_pArray[id].pprev == NULL)
					return NULL;
				else
					return &m_pArray[id].data;
			}
			return NULL;
		}
		/// 移动到链表尾郿
		inline void MoveToTail(unsigned int id)
		{
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

					m_pTail->pnext = &m_pArray[id];
					m_pArray[id].pprev = m_pTail;
					m_pArray[id].pnext = NULL;
					m_pTail = &m_pArray[id];
				}
			}
		}
		/// 申请空间位置
		unsigned int AllocId()
		{
			unsigned int ret = 0;
			if(IsCreated())
			{
				ret = m_free;
				if(ret != 0)
					m_free = m_pArray[ret].nextfree;
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
	cXList()
	:m_bCreated(false)
	,m_plst(NULL)
	,m_nCnt(0)
	,m_nBlockSize(0)
	,m_nThough(0)
	,m_bThreadSafe(false)
#ifdef _DEBUG
	,m_renterflag(0)
	,bcheckflag(false)
#endif
	{}

	virtual ~cXList()
	{
		Destroy();
	}
	inline unsigned GetId(T *pt)
	{
		if (m_bThreadSafe) EnterCriticalSection(&m_CriticalSection);//锁定
		unsigned nId = 0;
		for(unsigned int i=0;i<m_nCnt;i++)
		{
			if(m_plst[i].InLink(pt))
			{
				nId = m_plst[i].GetId(pt);
				nId = i*m_nBlockSize+nId;
			}
		}
		if (m_bThreadSafe) LeaveCriticalSection(&m_CriticalSection);//解锁
		return nId;
	}
	BOOL Create(int nBlockSize,bool bThreadSafe=true)
	{
		m_nBlockSize = nBlockSize;
		m_bThreadSafe = bThreadSafe;
		if (m_bThreadSafe)InitializeCriticalSection(&m_CriticalSection);//初始化临界对豿
		m_bCreated = true;
		Enlarge();
		return TRUE;
	}
	bool IsCreated()
	{
		return m_bCreated;
	}
	VOID Destroy()
	{
		if(!m_bCreated) 
			return;

		// ????й?
		unsigned int nLeaked = GetCount();
		if(nLeaked>0)
		{
			LOGF_ERROR(DLL_LOGGER,_T("cXList:still %d nodes not be free\n"),nLeaked);
			
			T * t = NextData(NULL);
			while(t)
			{
				st_node * node = (st_node *)t;

#if XLIST_LEAK_CHECK		
				LOGF_ERROR(DLL_LOGGER,_T("\tfile = %s,line = %d\n"),node->src_file,node->src_line);
#endif
				t = NextData(t);
			}
		}
		
		
		for(unsigned int i=0;i<m_nCnt;i++)
		{
			m_plst[i].Destroy();
		}
		if(m_plst)
		{
			delete []m_plst;
			m_plst = NULL;
		}
		if (m_bThreadSafe)DeleteCriticalSection(&m_CriticalSection);//移除临界对象
		m_bCreated = false;
		m_bThreadSafe = false;
	}
	BOOL Clean()
	{
		if(!m_bCreated) return TRUE;
		if (m_bThreadSafe) EnterCriticalSection(&m_CriticalSection);//锁定
		if(m_nCnt)
		{
			for(unsigned int i=0;i<m_nCnt;i++)
			{
				m_plst[i].Clean();
			}
		}
		m_nCnt = 0;
		m_nBlockSize = 0;
		m_nThough = 0;
		if (m_bThreadSafe) LeaveCriticalSection(&m_CriticalSection);//解锁
		return TRUE;
	}
	unsigned int GetCount() const 
	{
		if (m_bThreadSafe) EnterCriticalSection(&m_CriticalSection);//锁定
		unsigned int nSum = 0;
		for(unsigned int i=0;i<m_nCnt;i++)
		{
			nSum += m_plst[i].GetCount();
		}
		if (m_bThreadSafe) LeaveCriticalSection(&m_CriticalSection);//解锁
		return nSum;
	}

	unsigned int GetMaxCount() const 
	{
		if (m_bThreadSafe) EnterCriticalSection(&m_CriticalSection);//锁定
		unsigned int nMaxCount = 0;
		for(unsigned int i=0;i<m_nCnt;i++)
		{
			nMaxCount += m_plst[i].GetMaxCount();
		}
		if (m_bThreadSafe) LeaveCriticalSection(&m_CriticalSection);//解锁
		return nMaxCount;
	}

	int Reset()
	{
		if (m_bThreadSafe) EnterCriticalSection(&m_CriticalSection);//锁定
		for(unsigned int i=0;i<m_nCnt;i++)
		{
			m_plst[i].Reset();
		}
		if (m_bThreadSafe) LeaveCriticalSection(&m_CriticalSection);//解锁
		return 1;
	}
	inline T * NextData(T *pcur=NULL)
	{
		unsigned i;
		if (m_bThreadSafe) EnterCriticalSection(&m_CriticalSection);//锁定
		T* pRet = NULL;
		if(!pcur && m_nCnt)
		{
			for(i=0;i<m_nCnt;i++)
			{
				pRet = m_plst[i].NextData();
				if(pRet)
					break;
			}
		}
		else
		{
			for(i=0;i<m_nCnt;i++)
			{
				if(m_plst[i].InLink(pcur))
				{
					break;
				}
			}
			if(i!=m_nCnt)
			{
				pRet=pcur;
				for(unsigned j=i;j<m_nCnt;j++)
				{
					pRet = m_plst[j].NextData(pRet);
					if(pRet)
						break;
				}
			}else
				_assert(0);

		}
		if (m_bThreadSafe) LeaveCriticalSection(&m_CriticalSection);//解锁
		return pRet;
	}
#ifdef _DEBUG
	void OpenCheck()
	{
		bcheckflag=true;
	}

	void CloseCheck()
	{
		bcheckflag=false;
	}
#endif

	//??ááá???????
	T * First()
	{
#ifdef _DEBUG
		if(bcheckflag)
		{
			_assert(!m_renterflag);
			m_renterflag=-1;
		}
#endif
		if (m_bThreadSafe) EnterCriticalSection(&m_CriticalSection);//锁定
		T* pRet = NULL;
		for(m_nThough = 0;m_nThough<m_nCnt;m_nThough++)
		{
			pRet = m_plst[m_nThough].First();
			if(pRet)
				break;
		}
#ifdef _DEBUG
		if(!pRet)
			m_renterflag=0;
#endif
		if (m_bThreadSafe) LeaveCriticalSection(&m_CriticalSection);//解锁
		return pRet;
	}

	//??ááá???????
	T * Cur()
	{
		if (m_bThreadSafe) EnterCriticalSection(&m_CriticalSection);//锁定
		T * pRet = m_plst[m_nThough].Cur();
		if (m_bThreadSafe) LeaveCriticalSection(&m_CriticalSection);//解锁
		return pRet;
	}

	//??ááá???????
	T * Next()
	{
		if (m_bThreadSafe) EnterCriticalSection(&m_CriticalSection);//锁定
		T* pRet = NULL;
		pRet = m_plst[m_nThough].Next();
		if(pRet == NULL)
		{
			for(;m_nThough < m_nCnt-1;)
			{
				++m_nThough;
				pRet = m_plst[m_nThough].First();
				if(pRet)
					break;
			}
		}
#ifdef _DEBUG
		if(bcheckflag)
		{
			if(pRet == NULL)
			{
				m_renterflag=0;
			}
		}
#endif
		if (m_bThreadSafe) LeaveCriticalSection(&m_CriticalSection);//解锁
		return pRet;
	}

	//??ááá???????
	T * End()
	{
		if (m_bThreadSafe) EnterCriticalSection(&m_CriticalSection);//锁定
		T* pRet = NULL;
		if(m_nCnt)
		{
			pRet = m_plst[m_nCnt-1].End();
		}
		if (m_bThreadSafe) LeaveCriticalSection(&m_CriticalSection);//解锁
		return pRet;
	}

	unsigned int NewOne(T ** t,const char * src_file,unsigned int src_line)
	{
		if (m_bThreadSafe) EnterCriticalSection(&m_CriticalSection);//锁定
		unsigned int nRet = 0;
		for(unsigned int i=0;i<m_nCnt;i++)
		{
			if( (nRet=m_plst[i].New(t,src_file,src_line)) )
			{
				nRet = i*m_nBlockSize+nRet;
				break;
			}
		}
		if(!nRet && Enlarge())
		{
			nRet = m_plst[m_nCnt-1].New(t,src_file,src_line);
			if (nRet)
			{
				nRet = (m_nCnt-1)*m_nBlockSize+nRet;
			}
		}
		if (m_bThreadSafe) LeaveCriticalSection(&m_CriticalSection);//解锁
		if(nRet==0)
		{
			LOGF_ERROR(DLL_LOGGER,_T("ASSERT:m_nCnt={%d},m_plst.m_free={%d},m_plst.m_totel={%d},m_plst.m_nMaxCount={%d},file=%s,line=%d\n"), 
				m_nCnt,
				m_plst->m_free,
				m_plst->m_totel,
				m_plst->m_nMaxCount,
				__FILE__,
				__LINE__);
			MEMORYSTATUS MemStat;
			MemStat.dwLength = sizeof(MEMORYSTATUS);
			GlobalMemoryStatus(&MemStat);
			unsigned short FreeMem = (unsigned short)(MemStat.dwAvailPhys/1000/1000);
			LOGF_ERROR(DLL_LOGGER,_T("Enlarge can not allocate memory,phys mem left{%d M}\n"),FreeMem);
		}
		//__assert(nRet>0);
		return nRet;
	}

	inline int Del(T *p)
	{
		if (m_bThreadSafe) EnterCriticalSection(&m_CriticalSection);//锁定
		int nRet = 0;
		for(unsigned int i=0;i<m_nCnt;i++)
		{
			if(m_plst[i].InLink(p))
			{
				nRet = m_plst[i].Del(p);
				break;
			}
		}
		if (m_bThreadSafe) LeaveCriticalSection(&m_CriticalSection);//解锁
		return nRet;
	}

	BOOL IsAllocated(T *p)
	{
		if (m_bThreadSafe) EnterCriticalSection(&m_CriticalSection);//锁定
		BOOL bRet = FALSE;
		for(unsigned int i=0;i<m_nCnt;i++)
		{
			if(m_plst[i].InLink(p))
			{
				bRet = TRUE;
				break;
			}
		}
		if (m_bThreadSafe) LeaveCriticalSection(&m_CriticalSection);//解锁
		return bRet;
	}

	int	Del(unsigned int id)
	{
		if (m_bThreadSafe) EnterCriticalSection(&m_CriticalSection);//锁定
		int nRet = 0;
		unsigned int n1d = (id-1)/m_nBlockSize;
		unsigned int n2d = id%m_nBlockSize;
		n2d = n2d?n2d:m_nBlockSize;
		if(n1d >= 0 && n1d < m_nCnt)
		{
			nRet = m_plst[n1d].Del(n2d);
		}
		if (m_bThreadSafe) LeaveCriticalSection(&m_CriticalSection);//解锁
		return nRet;
	}

	T *	Get(unsigned int id)
	{
		if (m_bThreadSafe) EnterCriticalSection(&m_CriticalSection);//锁定
		T* pRet = NULL;
		unsigned int n1d = (id-1)/m_nBlockSize;
		unsigned int n2d = id%m_nBlockSize;
		n2d = n2d?n2d:m_nBlockSize;
		if(n1d >= 0 && n1d < m_nCnt)
		{
			pRet = m_plst[n1d].Get(n2d);
		}
		if (m_bThreadSafe) LeaveCriticalSection(&m_CriticalSection);//解锁
		return pRet;
	}

private:
	bool Enlarge()
	{
		++m_nCnt;
		st_list *pList = new st_list[m_nCnt];
		ZeroMemory(pList,sizeof(st_list)*m_nCnt);
		if(m_plst)
		{
			memcpy(pList,m_plst,sizeof(st_list)*(m_nCnt-1));
			delete []m_plst;
		}
		m_plst = pList;
		m_plst[m_nCnt-1].Create(m_nBlockSize);
		return true;
	}
private:
	mutable CRITICAL_SECTION 
					m_CriticalSection;	//线程同步
	bool			m_bCreated;			//创建标志
	bool			m_bThreadSafe;		//是否可扩弿
	st_list		*	m_plst;				//二维链头
	unsigned int	m_nCnt;				//二维大小
	unsigned int	m_nBlockSize;		//块大尿
	unsigned int	m_nThough;			//当前所在的坿
};

#define XLIST_NEW(LST,ELEMENT)	(LST.NewOne(ELEMENT,__FILE__, __LINE__))

