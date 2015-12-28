#ifndef _MEMMGR_H
#define _MEMMGR_H

#include "CSLock.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////
// 关于内存管理器
// 1、不同的Dll,主调程序，可能会使用不同的堆去分配内存。所以不是自己分配的，自己不能释放。
// 2、重载全局的 new 一定要在所有的头文件包含之后进行。避免与 STL 这些也重载了 new 的系统相冲突。
// 3、管理器中为了查找哪些内存是自己分配的，需要 hash 表，这个表不能使用 STL（会造成递归释放！）只能自己写
////////////////////////////////////////////////////////////////////////////////////////////////////////

const int HASH_SIZE		= 11369;
class HashTbl 
{
private:
	struct Entry	//	HASH 表的一个条目. 
	{
		Entry	*	next;				// 指向哈西表中中有同样索引的下个条目
		void	*	ptr;				// 被包含的指针
		void	*	key;
	};
	Entry * Hash[HASH_SIZE];
	size_t GetHashIndex( void *address );

public:
	HashTbl();
	~HashTbl();
	void put(void * key, void * ptr);
	void * get(void * key);
	void * erase(void * key);
	void clear();
};


// 内存管理器所分配的内存格式
// 只要是避免零碎内存。需要分配的内存大小超过最大可能的时候，大块只包括一个小块。
// 每个 memory Chunk 被初始化成 16K 大小, 
//
// memory Chunk[0]
// {
//		ChunkHeader
//		ChunkMemory
//		{
//			memory Block[0]
//			{
//				BlockHeader
//				BlockMemory
//				BlockPostfix
//			}
//				....
//				....
//			memory Block[n]
//		}
//	}
//		....
//		....
// memory Chunk[n]

const int SRCNAME_LEN	= 31;
const int L_MINBLOCK	= 4;     // 最小块的大小  4 ^2 = 0x 10	Byte
const int L_MAXBLOCK	= 10;    // 最大块的大小 10 ^2 = 0x400	Byte
const int L_NUMBLOCKSIZES = (1+(L_MAXBLOCK - L_MINBLOCK));
const int L_PREFIX_MAGIC_COOKIE  = 0xDEADBEEF;
const int L_POSTFIX_MAGIC_COOKIE = 0xFEEBDAED;

class CSysMemory: public IMemManager,public CriticalSection 
{
	// 内存大块
	struct nChunkHeader 
	{
		nChunkHeader * next;			// 指向下个块
		nChunkHeader * prev;			// 指向上个块
		int	bnum;						// 包含小块的数量
		size_t bsize;					// 本身的长度

		inline nChunkHeader()
		{
			next = NULL;
			prev = NULL;
			bnum = 0;
			bsize = 0;
		}
	};

	// 内存小块 这个头一定要保持16字节的整数！
	struct nBlockHeader 
	{
		struct nBlockHeader *next;		// 指向下个小块					4
		size_t size;					// 本身的长度					8
		ALLOC_TYPE allocationType;		// 分配的类型					1
		char src_name[SRCNAME_LEN];	// 分配这个小块的源文件名		27
		int  src_line;					// 分配这个小块的源文件行数		4
		int magic;						//								4
	};

	struct nBlockPostfix 
	{
		int magic;
		int pad[3];
	};

	// 预定长度大块中小块信息
	size_t	b_size[L_NUMBLOCKSIZES];			// 每个大块中小块的大小
    int		b_num[L_NUMBLOCKSIZES];				// 每个大块中小块的数量
	nBlockHeader *blocks[L_NUMBLOCKSIZES];	// 每个大块中第一个小块的头
    
	// 内存大块管理
	nChunkHeader * chunksHead;
	nChunkHeader * chunksTail;
	nChunkHeader * chunksTailPrev;

	// 每个经过内存管理器分配的内存指针，都被这个表管理
	// 释放时来检测是否应该自己释放
	HashTbl myHashTable;

	// 内存信息统计
	size_t n_poolused;		// 内存管理器使用的内存大小
	size_t n_appused;		// 程序申请的内存大小

	nBlockHeader *NewChunk(size_t, int);
    void FreeChunk(nChunkHeader *);
	void CriticalError(const TCHAR * msg);

	size_t GetHashIndex( void *address )
	{
		return ((size_t)address >> 4) & (HASH_SIZE -1);
	}

public:
    
	CSysMemory();
    ~CSysMemory();
    
	void * Malloc( const char *file, int line, size_t size, ALLOC_TYPE type, void *address = NULL );
	bool Free( void *address, ALLOC_TYPE type );
	bool CheckPtr(void * mem);
	void MemoryInfo(size_t &nPoosUsed, size_t &nAppUsed);
	void DumpAllocs(const TCHAR * file);
};

#endif