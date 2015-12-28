#ifndef __MEMORYPOOL_H__
#define __MEMORYPOOL_H__
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <map>
#include <iostream>
#include <fstream>
#include <assert.h>
#include "MemoryChunk.h"

namespace MemPool
{
typedef CMemoryChunk<BYTE> COrigChunk;
/*!\class CMemoryPool
 * \brief 
 *
 * It is responsible for all MemoryRequests (GetMemory() / FreeMemory()) and
 * manages the allocation/deallocation of Memory from the Operating-System.
 */
class CMemoryPool
{
	
public:
	/*!
	Contructor
	\param [IN] pnBlockSize The Initial Block Size (in Bytes) of the Per Block
	\param [IN] pnBlockCnt The Count each MemoryBlock can Manage.
	\param [IN] nSize The Count element of array.
	*/
	CMemoryPool(int pnBlockSize[],int pnBlockCnt[],int nSize);
	virtual ~CMemoryPool(); //!< Destructor

	/*!
	Get "sMemorySize" Bytes from the Memory Pool.
	\param [IN] sMemorySize       Sizes (in Bytes) of Memory.
	\param [OUT] nId			  The Index Of MemoryBlock in MemoryPool.
	\return                       Pointer to a Memory-Block of "sMemorySize" Bytes, or NULL if an error occured. 
	*/
	virtual void *GetMemory(const std::size_t &sMemorySize,int &nId);
	/*!
	Free the allocated memory again....
	\param [IN] sMemorySize       Sizes (in Bytes) of Memory.
	\param [IN] nId				  The Index Of MemoryBlock in MemoryPool.
	*/
	virtual void FreeMemory(const std::size_t &sMemorySize,int nId);
private:
	std::map<int,COrigChunk *>	m_mapChunks;
};

}
#endif