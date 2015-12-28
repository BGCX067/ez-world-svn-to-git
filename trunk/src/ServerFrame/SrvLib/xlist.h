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
			EnterCriticalSection( m_pCriticalSection );//����
	}
	~cLock()
	{
		if(m_pCriticalSection)
			LeaveCriticalSection( m_pCriticalSection );//����
		m_pCriticalSection = NULL;
	}
private:
	CRITICAL_SECTION * m_pCriticalSection;//�߳�ͬ�������ٽ����
};

template <class T>
class cXList
{
#ifdef _DEBUG
	unsigned m_renterflag;
	bool		bcheckflag;
#endif
	/* �����Ϣ */
	typedef struct	_node_
	{
		T				data;				//�����d
		_node_	*		pnext;				//��һ���
		_node_	*		pprev;				//ǰһ���
		unsigned int	nextfree;			//��һ�����н��
#if XLIST_LEAK_CHECK		
		char			src_file[32];
		unsigned int	src_line;
#endif
	}st_node;
	/* ������Ϣ */
	typedef struct _list_
	{
		unsigned int	m_free;				//���ý����
		unsigned int	m_totel;			//��ǰ����
		unsigned int	m_nMaxCount;		//��󳤎�
		st_node	*		m_pArray;			//ָ���ѷ���ڞ�
		st_node *		m_pHead;			//ָ���һ���ѷ���ڵ��ǰһد
		st_node *		m_pThrough;			//���ڵ��������ö��������ͬʱ���������
		st_node *		m_pTail;			//ָ�����һ���ѷ���Ľڞ�
		/// �Ƿ��Ѿ�����ռ�
		inline BOOL	IsCreated()
		{
			return (m_pArray != NULL);
		}
		/// ����Ƿ��ڵ�ǰ����د
		inline bool InLink(T *pt)
		{
			return (st_node *)pt>m_pArray && (st_node *)pt<=m_pArray+m_nMaxCount;
		}
		/// ȡ�õ�ǰ�������������е�λ�Z
		inline unsigned GetId(T *pt)
		{
			assert((st_node *)pt>m_pArray&&(st_node *)pt<=m_pArray+m_nMaxCount);
			assert(((size_t)(st_node *)pt-(size_t)m_pArray)%sizeof(st_node)==0);
			return (unsigned)((st_node *)pt-m_pArray);
		}
		/// ����ָ����С������
		bool Create(int iMaxCount)
		{
			m_nMaxCount = iMaxCount;
			m_pArray = new st_node[m_nMaxCount+1];
			return Clean();
		}
		/// ��������
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
		/// ����������
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
		/// ȡ�õ�ǰ�������ô�С
		inline unsigned int GetCount()
		{
			return m_totel;
		}
		/// ȡ�õ�ǰ������ܴ���
		inline unsigned int GetMaxCount()
		{
			return m_nMaxCount;
		}
		/// ��ԭ
		inline int Reset()
		{
			if(IsCreated())
			{
				m_pThrough = m_pHead;
				return 1;
			}
			return 0;
		}
		/// ȡ��ָ�����ݵ���һ�����ݣ�Ĭ��ȡͷ
		inline T * NextData(T *pcur=NULL)
		{
			assert(IsCreated());
			st_node *pnext=pcur?((st_node *)pcur)->pnext:m_pHead->pnext;
			return pnext?&pnext->data:NULL;
		}
		/// ͷ���ݽ��鲻��ʹ��,ʹ��NextData
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
		/// ��ǰ����,���鲻��ʹ��,ʹ��NextData
		inline T * Cur()
		{
			if(!IsCreated())
				return 0;
			if(m_pThrough != NULL && m_pThrough != m_pHead)
				return &m_pThrough->data;
			return NULL;
		}
		/// ��һ����,���鲻��ʹ��,ʹ��NextData
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
		/// ͷ����
		inline T * Beg()
		{
			if(!IsCreated())
				return 0;
			if(m_pHead != NULL)
				return &m_pHead->data;
			return NULL;
		}
		/// β���ݽ��鲻��ʹ��
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
		/// �������ݿռ�
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
		/// �������ݿռ�
		inline int Del(T *p)
		{
			return Del((unsigned )((st_node *)p-m_pArray));
		}
		/// ָ���������Ƿ����ڸ�����ռ�
		inline BOOL IsAllocated(T *p)
		{
			return ((st_node *)p)->pprev!=NULL;
		}
		/// �������ݿռ�
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
					// �ڵ��Ѿ����ͷŹ���
					//_asm int 3;
				}
			}
			return nRet;
		}
		/// ȡ��ָ��λ�õ�����
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
		/// �ƶ�������β�d
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
		/// ����ռ�λ��
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
		/// �ͷſռ�λ��
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
		if (m_bThreadSafe) EnterCriticalSection(&m_CriticalSection);//����
		unsigned nId = 0;
		for(unsigned int i=0;i<m_nCnt;i++)
		{
			if(m_plst[i].InLink(pt))
			{
				nId = m_plst[i].GetId(pt);
				nId = i*m_nBlockSize+nId;
			}
		}
		if (m_bThreadSafe) LeaveCriticalSection(&m_CriticalSection);//����
		return nId;
	}
	BOOL Create(int nBlockSize,bool bThreadSafe=true)
	{
		m_nBlockSize = nBlockSize;
		m_bThreadSafe = bThreadSafe;
		if (m_bThreadSafe)InitializeCriticalSection(&m_CriticalSection);//��ʼ���ٽ���x
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

		// ????��?
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
		if (m_bThreadSafe)DeleteCriticalSection(&m_CriticalSection);//�Ƴ��ٽ����
		m_bCreated = false;
		m_bThreadSafe = false;
	}
	BOOL Clean()
	{
		if(!m_bCreated) return TRUE;
		if (m_bThreadSafe) EnterCriticalSection(&m_CriticalSection);//����
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
		if (m_bThreadSafe) LeaveCriticalSection(&m_CriticalSection);//����
		return TRUE;
	}
	unsigned int GetCount() const 
	{
		if (m_bThreadSafe) EnterCriticalSection(&m_CriticalSection);//����
		unsigned int nSum = 0;
		for(unsigned int i=0;i<m_nCnt;i++)
		{
			nSum += m_plst[i].GetCount();
		}
		if (m_bThreadSafe) LeaveCriticalSection(&m_CriticalSection);//����
		return nSum;
	}

	unsigned int GetMaxCount() const 
	{
		if (m_bThreadSafe) EnterCriticalSection(&m_CriticalSection);//����
		unsigned int nMaxCount = 0;
		for(unsigned int i=0;i<m_nCnt;i++)
		{
			nMaxCount += m_plst[i].GetMaxCount();
		}
		if (m_bThreadSafe) LeaveCriticalSection(&m_CriticalSection);//����
		return nMaxCount;
	}

	int Reset()
	{
		if (m_bThreadSafe) EnterCriticalSection(&m_CriticalSection);//����
		for(unsigned int i=0;i<m_nCnt;i++)
		{
			m_plst[i].Reset();
		}
		if (m_bThreadSafe) LeaveCriticalSection(&m_CriticalSection);//����
		return 1;
	}
	inline T * NextData(T *pcur=NULL)
	{
		unsigned i;
		if (m_bThreadSafe) EnterCriticalSection(&m_CriticalSection);//����
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
		if (m_bThreadSafe) LeaveCriticalSection(&m_CriticalSection);//����
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

	//??������???????
	T * First()
	{
#ifdef _DEBUG
		if(bcheckflag)
		{
			_assert(!m_renterflag);
			m_renterflag=-1;
		}
#endif
		if (m_bThreadSafe) EnterCriticalSection(&m_CriticalSection);//����
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
		if (m_bThreadSafe) LeaveCriticalSection(&m_CriticalSection);//����
		return pRet;
	}

	//??������???????
	T * Cur()
	{
		if (m_bThreadSafe) EnterCriticalSection(&m_CriticalSection);//����
		T * pRet = m_plst[m_nThough].Cur();
		if (m_bThreadSafe) LeaveCriticalSection(&m_CriticalSection);//����
		return pRet;
	}

	//??������???????
	T * Next()
	{
		if (m_bThreadSafe) EnterCriticalSection(&m_CriticalSection);//����
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
		if (m_bThreadSafe) LeaveCriticalSection(&m_CriticalSection);//����
		return pRet;
	}

	//??������???????
	T * End()
	{
		if (m_bThreadSafe) EnterCriticalSection(&m_CriticalSection);//����
		T* pRet = NULL;
		if(m_nCnt)
		{
			pRet = m_plst[m_nCnt-1].End();
		}
		if (m_bThreadSafe) LeaveCriticalSection(&m_CriticalSection);//����
		return pRet;
	}

	unsigned int NewOne(T ** t,const char * src_file,unsigned int src_line)
	{
		if (m_bThreadSafe) EnterCriticalSection(&m_CriticalSection);//����
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
		if (m_bThreadSafe) LeaveCriticalSection(&m_CriticalSection);//����
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
		if (m_bThreadSafe) EnterCriticalSection(&m_CriticalSection);//����
		int nRet = 0;
		for(unsigned int i=0;i<m_nCnt;i++)
		{
			if(m_plst[i].InLink(p))
			{
				nRet = m_plst[i].Del(p);
				break;
			}
		}
		if (m_bThreadSafe) LeaveCriticalSection(&m_CriticalSection);//����
		return nRet;
	}

	BOOL IsAllocated(T *p)
	{
		if (m_bThreadSafe) EnterCriticalSection(&m_CriticalSection);//����
		BOOL bRet = FALSE;
		for(unsigned int i=0;i<m_nCnt;i++)
		{
			if(m_plst[i].InLink(p))
			{
				bRet = TRUE;
				break;
			}
		}
		if (m_bThreadSafe) LeaveCriticalSection(&m_CriticalSection);//����
		return bRet;
	}

	int	Del(unsigned int id)
	{
		if (m_bThreadSafe) EnterCriticalSection(&m_CriticalSection);//����
		int nRet = 0;
		unsigned int n1d = (id-1)/m_nBlockSize;
		unsigned int n2d = id%m_nBlockSize;
		n2d = n2d?n2d:m_nBlockSize;
		if(n1d >= 0 && n1d < m_nCnt)
		{
			nRet = m_plst[n1d].Del(n2d);
		}
		if (m_bThreadSafe) LeaveCriticalSection(&m_CriticalSection);//����
		return nRet;
	}

	T *	Get(unsigned int id)
	{
		if (m_bThreadSafe) EnterCriticalSection(&m_CriticalSection);//����
		T* pRet = NULL;
		unsigned int n1d = (id-1)/m_nBlockSize;
		unsigned int n2d = id%m_nBlockSize;
		n2d = n2d?n2d:m_nBlockSize;
		if(n1d >= 0 && n1d < m_nCnt)
		{
			pRet = m_plst[n1d].Get(n2d);
		}
		if (m_bThreadSafe) LeaveCriticalSection(&m_CriticalSection);//����
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
					m_CriticalSection;	//�߳�ͬ��
	bool			m_bCreated;			//������־
	bool			m_bThreadSafe;		//�Ƿ������
	st_list		*	m_plst;				//��ά��ͷ
	unsigned int	m_nCnt;				//��ά��С
	unsigned int	m_nBlockSize;		//�����
	unsigned int	m_nThough;			//��ǰ���ڵĈ}
};

#define XLIST_NEW(LST,ELEMENT)	(LST.NewOne(ELEMENT,__FILE__, __LINE__))

