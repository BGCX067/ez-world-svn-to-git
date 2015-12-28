#include "MemoryPool.h"

namespace MemPool
{

/*
 * Constructor
 */
CMemoryPool::CMemoryPool(int pnBlockSize[],int pnBlockCnt[],int nSize)
{
	for (int i=0;i<nSize;i++)
	{
		if (pnBlockSize[i])
		{
			COrigChunk * pChunk = new COrigChunk;
			if(pChunk)
			{
				if(pChunk->Create(pnBlockCnt[i],pnBlockSize[i]))
				{
					m_mapChunks[ pnBlockSize[i] ] = pChunk;
				}
			}
		}
	}
}

/*
 * Destructor
 */
CMemoryPool::~CMemoryPool()
{
	std::map<int,COrigChunk *>::iterator iBeg = m_mapChunks.begin();
	std::map<int,COrigChunk *>::iterator iEnd = m_mapChunks.end();
	for (; iBeg!=iEnd;iBeg++)
	{
		COrigChunk * pChunk = iBeg->second;
		if (pChunk)
		{
			delete pChunk;
		}
	}
}

/*
 * GetMemory
 */
void *CMemoryPool::GetMemory(const std::size_t &sMemorySize,int &nId)
{
	BYTE *pRet = NULL;
	std::map<int,COrigChunk *>::iterator iFind = m_mapChunks.find(sMemorySize);
	if (iFind == m_mapChunks.end())
	{
		assert(false && "Error:MemorySize Not Manage.");
		return NULL;
	}
	COrigChunk * pChunk = iFind->second;
	nId = pChunk->New(&pRet);
	return (void *)pRet;
}

/*
 * FreeMemory
 */
void CMemoryPool::FreeMemory(const std::size_t &sMemorySize,int nId)
{
	std::map<int,COrigChunk *>::iterator iFind = m_mapChunks.find(sMemorySize);
	if (iFind != m_mapChunks.end())
	{
		COrigChunk * pChunk = iFind->second;
		pChunk->Del(nId);
	}
}

}