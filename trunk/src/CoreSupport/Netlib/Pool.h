#ifndef POOL_H_2010_11_30
#define POOL_H_2010_11_30
#include "support.h"
struct CHUNK;
class CLock
{
	CRITICAL_SECTION * m_pCriticalSection;
public:
	CLock(CRITICAL_SECTION * pCriticalSection)
	{
		assert(pCriticalSection);
		m_pCriticalSection=pCriticalSection;
		EnterCriticalSection(pCriticalSection);
	}
	~CLock()
	{
		assert(m_pCriticalSection);
		LeaveCriticalSection( m_pCriticalSection );
	}
};

struct CELL
{
	CHUNK  *pchunk;		//CURRENT CHUNK
	CELL *pnext,*pprev;
	bool m_bused;
};

struct CHUNK
{
	//size_t	num_used_cells;
	CELL  *m_pfreecell;
	//CELL  *m_pused,*m_ptail,*m_pcell;

	CHUNK *pnextchunk,*pprevchunk;
	//CHUNK *pprev;
	bool bhasfreecell()const
	{
		return m_pfreecell!=0;
	}

	bool IsInValidCellPointer(CELL  *pcell,unsigned cellcount,unsigned cellsize)const
	{
		UINT celloffset=((char *)pcell-(char *)(this+1));
		if(celloffset>=cellcount*(sizeof(CELL)+cellsize))
			return true;
		return ((celloffset%(sizeof(CELL)+cellsize))!=0);
	}
	//bool IsInValidCell(CELL  *pcell,unsigned cellcount,unsigned cellsize)const
	//{
	//	return ((pcell->pnext!=NULL)&&IsInValidCellPointer(pcell->pnext,cellcount,cellsize))||(pcell->pchunk!=this);
	//}

	//检查CHUNK结构是否有错
	//bool IsInValid(unsigned cellcount,unsigned cellsize)const
	//{
	//	UINT i;

	//	if(m_pfreecell!=NULL&&IsInValidCellPointer(m_pfreecell,cellcount,cellsize))
	//	{
	//		assert(0);
	//		return true;
	//	}
	//	CELL *pcell=(CELL *)(this+1);
	//	for(i=0;i<cellcount;i++)
	//	{
	//		if(IsInValidCell(pcell,cellcount,cellsize))
	//		{
	//			assert(0);
	//			return true;
	//		}
	//		*(char **)&pcell+=sizeof(CELL)+cellsize;
	//	}
	//	pcell=m_pfreecell;
	//	for(i=0;i<cellcount&&pcell!=0;i++)
	//	{
	//		pcell=pcell->pnext;
	//	}
	//	return pcell!=NULL;
	//}

	CHUNK(unsigned ncell,unsigned cellsize,CELL *pcell)
	{

		pnextchunk=NULL;
		pprevchunk=NULL;
		m_pfreecell=pcell;

		unsigned i;
		for(i=0;i<ncell-1;i++)
		{
			pcell->pnext=(CELL *)((char *)pcell+sizeof(CELL)+cellsize);
			//m_pcell[i].pprev=NULL;
			pcell->pchunk=this;
			assert(pcell->pchunk);
			pcell=pcell->pnext;
			pcell->m_bused=false;
		}
		pcell->pnext=NULL;
		pcell->pchunk=this;
	}

	CELL *New()
	{
		CELL  *pfree=m_pfreecell;
		assert(pfree);
		m_pfreecell=pfree->pnext;
		//pfree->pnext=NULL;
		pfree->m_bused=true;
		return pfree;
	}
	void Del(void *pnode)
	{
		CELL *pcell=(CELL *)((char *)pnode-sizeof(CELL));
		pcell->pnext=m_pfreecell;
		pcell->m_bused=false;
		m_pfreecell=pcell;

	}
	~CHUNK()
	{
	}
};

class CPool
{
protected:
	unsigned m_cellcount;	//每个Chunk中的单元数
	unsigned m_cellsize;	//每个单元可用部分的大小
	UINT m_used;			//被使用的块
	UINT m_maxallocated;	//最大分配单元数
	CHUNK	*m_pfree;//第一个有空闲单元的块
	CHUNK	*m_pused;//指向第一个被完全占用的块
	CELL	*m_pusedcell;
	unsigned m_usedchunk,m_prevusedchunk;
	char m_name[32];
	void init()
	{
		m_pused=NULL;
		//m_ptail=NULL;
		m_pfree=NULL;
		m_cellcount=0;
		m_cellsize=0;
		m_maxallocated=m_used=0;
		m_pusedcell=NULL;
		m_usedchunk=0;
		m_prevusedchunk=0;
		m_name[0]='\0';
	}

public:
	CPool()
	{
		init();
	}
	CPool(const char *pname)
	{
		init();
		strcpy(m_name,pname);
	}

	~CPool()
	{
		CHUNK *pchunk,*pnextchunk;
		for(pchunk=m_pused;pchunk;pchunk=pnextchunk)
		{
			pnextchunk=pchunk->pnextchunk;
			assert(!pchunk->m_pfreecell);
			delete[] (char *)pchunk;
		}
		for(pchunk=m_pfree;pchunk;pchunk=pnextchunk)
		{
			pnextchunk=pchunk->pnextchunk;
			delete[] (char *)pchunk;
		}
	}

	void Create(UINT CellCount,UINT cellsize)
	{
		m_cellcount=CellCount;
		m_cellsize=cellsize;
	}
	UINT GetChunkSize()
	{
		return sizeof(CHUNK)+m_cellcount*(m_cellsize+sizeof(CELL));
	}

	//最多曾经分配多少个单元
	UINT GetMaxAllocated()
	{
		return m_maxallocated;
	}
	//当前分配的单元数
	UINT GetAllocated()
	{
		return m_used;
	}
	
	void *NextData(void *pdata=NULL)const
	{
		if(!pdata)
			return m_pusedcell?(m_pusedcell+1):NULL;
		CELL *pcell=(CELL *)pdata-1;
		if(pcell->pnext)
			return (pcell->pnext+1);
		return NULL;
	}
	void * New()
	{
		if(!m_pfree)
		{//没有空闲的cell
			m_pfree=(CHUNK *)new char[GetChunkSize()];
			if(!m_pfree)
				return NULL;

			new(m_pfree)CHUNK(m_cellcount,m_cellsize,(CELL *)(m_pfree+1));
			m_usedchunk++;
			if(m_usedchunk/8!=m_prevusedchunk/8)
			{
				m_prevusedchunk=m_usedchunk/8*8;
				//_error("The number of chunk in memory pool %s has grown to %d chunks,total allocated=%d,memory pool size=%d\n",m_name,m_usedchunk,GetAllocated(),GetChunkSize()*m_usedchunk);
			}
		}

		CELL *pfree=m_pfree->New();
		//将所有占用cell串接起来
		pfree->pnext=m_pusedcell;
		pfree->pprev=NULL;
		if(m_pusedcell)
			m_pusedcell->pprev=pfree;
		m_pusedcell=pfree;

		if(!m_pfree->bhasfreecell())
		{//从自由表中删除，串接到占用表中
			CHUNK *porghead=m_pused;
			m_pused=m_pfree;
			assert(!m_pfree->pprevchunk);

			m_pfree=m_pfree->pnextchunk;
			if(m_pfree)
				m_pfree->pprevchunk=NULL;

			m_pused->pnextchunk=porghead;
			if(porghead)
				porghead->pprevchunk=m_pused;
			assert(!m_pused->pprevchunk);
			assert(m_pfree!=m_pused);
			if(m_pfree)
				assert(!m_pfree->pprevchunk);
		}

		m_used++;
		if(m_maxallocated<m_used)
			m_maxallocated=m_used;
		return pfree+1;
	}
 
	void Del(void *pnode)
	{
		CELL *pcell=(CELL *)((char *)pnode-sizeof(CELL));
		CHUNK *pchunk=pcell->pchunk;
#ifdef _DEBUG
		if(pchunk->IsInValidCellPointer(pcell,m_cellcount,m_cellsize))
		{
			assert(0);
			return;
		}
#endif
		//从使用表中去除
		CELL *pnextusedcell,*pprevusedcell;
		pnextusedcell=pcell->pnext;
		pprevusedcell=pcell->pprev;
		if(pnextusedcell)
			pnextusedcell->pprev=pprevusedcell;
		if(pprevusedcell)
			pprevusedcell->pnext=pnextusedcell;
		else
			m_pusedcell=pnextusedcell;

		pchunk->Del(pnode);
		if(!pcell->pnext)
		{//如果这是当前chunk中唯一的free cell
			CHUNK *pprevchunk,*pnextchunk;
			pnextchunk=pchunk->pnextchunk;
			pprevchunk=pchunk->pprevchunk;
			//从占用表中删除
			if(m_pused==pchunk)
			{
				m_pused=m_pused->pnextchunk;
			}
			if(pprevchunk)
			{
				pprevchunk->pnextchunk=pnextchunk;
			}
			if(pnextchunk)
			{
				pnextchunk->pprevchunk=pprevchunk;
			}

			//串接到自由表中
			if(m_pfree)
			{
				assert(!m_pfree->pprevchunk);
				m_pfree->pprevchunk=pchunk;
			}
			pchunk->pnextchunk=m_pfree;
			pchunk->pprevchunk=NULL;
			m_pfree=pchunk;


			assert(!m_pfree->pprevchunk);
			assert(m_pfree!=m_pused);
		}
		assert(m_used>=0);
		m_used--;
	}
	//bool IsInValid()const
	//{
	//	//unsigned totalused=0;
	//	CHUNK *pchunk;
	//	for(pchunk=m_pused;pchunk;pchunk=pchunk->pnextchunk)
	//	{
	//		if(pchunk->IsInValid(m_cellcount,m_cellsize))
	//			return true;
	//	}
	//	for(pchunk=m_pfree;pchunk;pchunk=pchunk->pnextchunk)
	//	{
	//		if(pchunk->IsInValid(m_cellcount,m_cellsize))
	//			return true;
	//	}
	//	return false;
	//}
};

class CThreadSafePool:public CPool
{
	CRITICAL_SECTION m_CriticalSection;
public:
	CThreadSafePool()
	{
		InitializeCriticalSection(&m_CriticalSection);
	}

	~CThreadSafePool()
	{
		DeleteCriticalSection(&m_CriticalSection);
	}
	void * New()
	{
		CLock lock(&m_CriticalSection);
		return CPool::New();
	}
	void Del(void *pnode)
	{
		CLock lock(&m_CriticalSection);
		return CPool::Del(pnode);
	}
};

template<class T>
class CObjectPool : public CPool
{
public:
	void Create(UINT CellCount)
	{
		CPool::Create(CellCount,sizeof(T));
	}
	T *NextData(void *pdata=NULL)const
	{
		return (T *)CPool::NextData(pdata);
	}
	T * New()
	{
		T *pnode;
		pnode=(T *)CPool::New();
		new(pnode) T;
		return pnode;
	}
	void Del(T *pnode)
	{
		pnode->~T();
		CPool::Del(pnode);
	}
	CObjectPool(const char *pname):CPool(pname)
	{
	}
	CObjectPool():CPool()
	{
	}
	~CObjectPool()
	{
		T *pdata;
		if(m_used)
		{
			//_error("Object Pool %s memory not released %d\n",m_name,m_used);
		}

		while(pdata=NextData())
		{
			Del(pdata);
		}
		//CHUNK *pchunk;
		//CELL *pcell;
		//unsigned i;
		//for(pchunk=m_pused;pchunk;pchunk=pchunk->pnextchunk)
		//{
		//	pcell=(CELL *)(pchunk+1)+1;
		//	for(i=0;i<m_cellcount-1;i++)
		//	{
		//		if(pcell->pnext)
		//			((T *)(pcell+1))->~T();
		//	}
		//}
		//for(pchunk=m_pfree;pchunk;pchunk=pchunk->pnextchunk)
		//{
		//	pcell=(CELL *)(pchunk+1)+1;
		//	for(i=0;i<m_cellcount-1;i++)
		//	{
		//		if(pcell->pnext)
		//			((T *)(pcell+1))->~T();
		//	}
		//}
	}
};

template<class T>
class CThreadSafeObjectPool:public CObjectPool<T>
{
	CRITICAL_SECTION m_CriticalSection;
public:
	CThreadSafeObjectPool():CObjectPool()
	{
		InitializeCriticalSection(&m_CriticalSection);
	}
	CThreadSafeObjectPool(const char *pname):CObjectPool(pname)
	{
		InitializeCriticalSection(&m_CriticalSection);
	}
	~CThreadSafeObjectPool()
	{
		DeleteCriticalSection(&m_CriticalSection);
	}
	T * New()
	{
		CLock lock(&m_CriticalSection);
		return CObjectPool<T>::New();
	}
	void Del(T *pnode)
	{
		CLock lock(&m_CriticalSection);
		return CObjectPool<T>::Del(pnode);
	}
};
#endif
