//-------------------------------------------------------------------
//  memmgr.c
//-------------------------------------------------------------------
#include <malloc.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "support.h"
#include "vfile.h"
#include "memmgr.h"


size_t HashTbl::GetHashIndex( void *address )
{
	size_t a = (size_t)address;
	a = (a+0x7ed55d16) + (a<<12);
	a = (a^0xc761c23c) ^ (a>>19);
	a = (a+0x165667b1) + (a<<5);
	a = (a+0xd3a2646c) ^ (a<<9);
	a = (a+0xfd7046c5) + (a<<3);
	a = (a^0xb55a4f09) ^ (a>>16);
	return a % HASH_SIZE;
	
	//return ((size_t)address >> 4) & (HASH_SIZE -1);
}

HashTbl::HashTbl()
{
	memset(Hash, 0, HASH_SIZE*sizeof(Entry *));
}
HashTbl::~HashTbl(void)
{
	clear();
}
void HashTbl::put(void * key, void * ptr)
{
	size_t index = GetHashIndex(key);
	Entry *e;

	for(e=Hash[index]; e; e=e->next)
	{
		if(e->key == key) 
			break;
	}
	if(!e) 
	{
		e=(Entry *)_aligned_malloc(sizeof(Entry), 16);
		e->next=Hash[index];
		e->key = key;
		e->ptr = ptr;
		Hash[index]=e;
	}
}

void * HashTbl::get(void * key)
{
	size_t index = GetHashIndex(key);
	for(Entry *e=Hash[index]; e; e=e->next)
	{
		if(e->key==key) 
			return e->ptr;
	}
	return NULL;
}

void * HashTbl::erase(void * key)
{
	size_t index = GetHashIndex(key);
	Entry * prv = NULL, * next = NULL, *e=Hash[index];
	while(e)
	{
		if(e->key == key)
		{
			if(prv)
				prv->next = e->next;	// 连接本项目的链
			else
				Hash[index] = e->next;	// 清除链表

			void * ret = e->ptr;
			_aligned_free(e);			
			e = NULL;					// 删除元素

			return ret;
		}
		prv = e;
		e = e->next; 
	}
	return NULL;
}

void HashTbl::clear()
{
	Entry *enext, *e;

	for(int i=0; i<HASH_SIZE; i++)
	{
		for(e=Hash[i]; e; e=enext)
		{
			enext=e->next;	// 
			_aligned_free(e);		// 删除元素
			e = NULL;
			Hash[i] = enext;// 清除链表
		}
	}
}



//-------------------------------------------------------------------
//  CSysMemory()
//  先分配一定数量的大块
//-------------------------------------------------------------------
CSysMemory::CSysMemory()
{
	int i;
    
	chunksTail = NULL;
	chunksHead = (nChunkHeader *) &(chunksTail);		// 头指针指向尾指针
	chunksTailPrev = (nChunkHeader *) &(chunksHead);	// 头指针指向尾指针;

	n_poolused = 0;					
	n_appused = 0;

	for (i=0; i<(int)L_NUMBLOCKSIZES; i++)
	{
        b_size[i] = (1<<(i+L_MINBLOCK));		// 0x1000(1<<4) 0x10000(1<<5) 1<<7
        b_num[i]  = (int)((16*1024) / b_size[i]);
        blocks[i] = NewChunk(b_size[i],b_num[i]);
        if (NULL == blocks[i]) 
			CriticalError(_T("CSysMemory::CSysMemory():Out Of Mem when alloc blocks!")); 
    }
}

//-------------------------------------------------------------------
//  ~CSysMemory()
//  释放所有的大块
//-------------------------------------------------------------------
CSysMemory::~CSysMemory()
{
	DumpAllocs(_T("Memory.Log")); 
	nChunkHeader *ch;
	assert(_heapchk() == _HEAPOK);
	while (ch = chunksHead, ch->next)	// 从大块的头指针开始，到尾指针为止，标记是尾指针指向结尾标记 ChunksTail(NULL)
        this->FreeChunk(ch);
}

//-------------------------------------------------------------------
//  NewChunk()
// 分配内存大块,参数为 小块的大小，小块的数量
//-------------------------------------------------------------------
CSysMemory::nBlockHeader *CSysMemory::NewChunk(size_t bsize, int bnum)
{
    // 小快的大小为 实际大小加头，加后缀
	size_t block_size = bsize + sizeof(nBlockHeader) + sizeof(nBlockPostfix);
	// 大块的大小为 块头 + 小块*小块数量
	size_t chunk_size = sizeof(nChunkHeader) + (block_size*bnum);
    
	// 分配大块(16位边界对齐)
	BYTE *c = (BYTE *) _aligned_malloc(chunk_size, 16);
	assert(!((ULONG_PTR)c & 15));

    if (c) 
	{
        n_poolused += chunk_size;

        // ChunkHeader 初始化
        nChunkHeader *ch = (nChunkHeader *)c;
        ch->next = NULL;					// 当前的下个指针为空
		ch->prev = NULL;					// 当前的下个指针为空
        ch->bnum  = bnum;					// 小块的数量
        ch->bsize = block_size;				// 每个小块的大小
        
		// 增加到链表尾部
        nChunkHeader * tail = (nChunkHeader *)&(chunksTail);
		ch->prev = tail->prev;				// 接管原来尾指针的prev指针
        ch->next = tail;					// next 指针指向原来的尾指针
        tail->prev->next = ch;				// 原来尾指针的prev 指针的next 指向自己
        tail->prev = ch;					// 原来尾指针的prev 指向自己

        // 初始化大块中的每个小块
        BYTE *f = c + sizeof(nChunkHeader); // 第一个小块
        BYTE *p = c + chunk_size;           // 最后一个小块 + 1
        BYTE *next = NULL;
        while (p > f) 
		{
            p -= block_size;						// 从后向前遍历
            nBlockHeader *bh = (nBlockHeader *)p;	// 取得小块头标记
            bh->next = (nBlockHeader *) next;		// 
            bh->size = 0;
            strcpy(bh->src_name,"<new>");
            bh->src_line = 0;
            bh->magic = L_PREFIX_MAGIC_COOKIE;
			bh->allocationType = MM_UNKNOWN;
            next = p;
        }
        return (nBlockHeader *) f;					// 返回第一个小块的头
    }
    return NULL;
}

//-------------------------------------------------------------------
//  FreeChunk()
// 释放内存大块
//-------------------------------------------------------------------
void CSysMemory::FreeChunk(CSysMemory::nChunkHeader *ch)
{
    n_poolused -= sizeof(nChunkHeader) + (ch->bsize*ch->bnum); 

	// 清除本身在链表中的位置
    ch->next->prev = ch->prev;
    ch->prev->next = ch->next;
    ch->next = NULL;
    ch->prev = NULL;

	// 清除大块内存
	assert(!((ULONG_PTR)ch & 15));
    _aligned_free((void *)ch);
}

//-------------------------------------------------------------------
//  Malloc()
//  分配内存
//-------------------------------------------------------------------
inline const char * find_Name(const char *path){
	size_t length, l;
	l = length = strlen(path)-1;
	while (length > 0 && path[length] != '\\' && path[length] != '/'){
		length--;
	}
	return &path[length+1];
}

void * CSysMemory::Malloc( const char *file, int line, size_t size, ALLOC_TYPE type, void *address )
{
	CSGUARD;

	BYTE *p = NULL;
	file = find_Name(file);

	// 分配源文件名字的长度超长被截断,计算实际偏移量
    size_t len = strlen(file)+1;					
    int off = (int)len - SRCNAME_LEN;
    if (off < 0) 
		off = 0;
    // 如果是 relloc,需要先释放前面的内存。
	if(type == MM_REALLOC)
	{
		// relloc 只能重新分配内存管理器分配的内存，如果不是经过内存管理器分配的，不能 relloc 所以需要检测一下
		
		// 1. 这个内存是不是经过内存管理器分配的
		nBlockHeader * bh = (nBlockHeader * )myHashTable.get(address); 
		if(!bh)
			CriticalError(_T("CSysMemory::Malloc(): Request to reallocate RAM that was never allocated!"));
		
		// 2. 以前的类型是否可以进行 relloc
		else if( ( bh->allocationType != MM_MALLOC ) &&
				 ( bh->allocationType == MM_CALLOC ) &&
				 ( bh->allocationType == MM_REALLOC ) )
		{
			CriticalError(_T("CSysMemory::Malloc(): Request to reallocate RAM 's type not match!"));
		}
		// 3. 释放内存
		else
			Free(address,MM_FREE);
	}
	
	// 如果需要的大小超过预定的大块长度，就直接分配一个需要长度的大块。
	// 只有在需要的内存大小小于一定长度时，采取一次分配一个大块的策略。
	// 这样做是为了避免分配小内存。
	if (size > (1<<L_MAXBLOCK)) 
	{
        // 重新分配一个大块
        p = (BYTE *) this->NewChunk(size,1);
        if (p) 
		{
            nBlockHeader *bh  = (nBlockHeader *) p;
            nBlockPostfix *bp = (nBlockPostfix *) (p + size + sizeof(nBlockHeader));
            bh->next = (nBlockHeader *) -1L;    // 只有一个小块的大块，后面没有其他小块了。
            bh->size = size;
            strcpy(bh->src_name,&(file[off]));
            bh->src_line = line;
			bh->allocationType = type;
            bh->magic = L_PREFIX_MAGIC_COOKIE;
            bp->magic = L_POSTFIX_MAGIC_COOKIE;
            p += sizeof(nBlockHeader);

            n_appused += size;
			myHashTable.put(p,bh); 
        }
    } 
	else 
	{
        // 寻找最佳的大块预定长度
        int i;
        for (i=0; i<(int)(L_NUMBLOCKSIZES-1); i++) 
		{
            size_t mask = ~(b_size[i]-1);
            if ((size & mask) == 0) {
                break;
            }
        }
        p = (BYTE *) blocks[i];
		if (p) 
		{
            nBlockHeader  *bh = (nBlockHeader *)  p;
            nBlockPostfix *bp = (nBlockPostfix *) (p + size + sizeof(nBlockHeader));
            
			// 为了能够每次都找到一个可用的内存小块，在一个大块被用完后，要再次分配大块
			blocks[i] = bh->next;
            if (NULL == blocks[i]) 
			{
                // 大块已经被用完
                blocks[i] = this->NewChunk(b_size[i],b_num[i]);
                if (NULL == blocks[i]) 
					CriticalError(_T("CSysMemory::Malloc(): Can't new memory"));
            }
            
			// 分配后的小块,next变量记录了小块属于哪个尺寸的Block
			bh->next  = (nBlockHeader *) ((size_t)i);			
            bh->size  = size;
            strcpy(bh->src_name,&(file[off]));
            bh->src_line = line;
			bh->allocationType = type;
            bh->magic = L_PREFIX_MAGIC_COOKIE;
            bp->magic = L_POSTFIX_MAGIC_COOKIE;
            p += sizeof(nBlockHeader);
            n_appused += size;

			myHashTable.put(p,bh); 
        }
    }
    
	if(!p)
	{
		// throw 错误到异常处理
		TCHAR szBuffer[256];
		_stprintf(szBuffer,_T("CSysMemory::Malloc(): Can't new memory!_File:%s;Line:%d;Size:%d"),
			file, line, size);
		CriticalError(szBuffer);
	}

	return p;
}


//-------------------------------------------------------------------
//  Free()
//-------------------------------------------------------------------
bool CSysMemory::Free(void *address, ALLOC_TYPE type)
{
	// 空指针
	if (!address)
		return true;

	CSGUARD;
	
	// 如果不是自己分配的，因为不同进程不用同在一个堆，不能直接释放，只能返回false代表不能释放。
	nBlockHeader * add = (nBlockHeader *)myHashTable.get(address);
	if(!add)
		return false;

	// 是自己分配的
	BYTE *pc = (BYTE *) address;
    nBlockHeader  *bh = (nBlockHeader *) (pc - sizeof(nBlockHeader));
	assert(add == bh);
    nBlockPostfix *bp;

    // 检查块头标记当前的块是否被破坏?
    if (bh->magic != (int)L_PREFIX_MAGIC_COOKIE) 
	{
		TCHAR szBuffer[1024];
		_stprintf(szBuffer,_T("Free(): START OF MEM BLOCK CORRUPTED\nFile:%s\nLines:%d"),bh->src_name, bh->src_line);  
		CriticalError(szBuffer);
    }
    
	// 检查块尾标记当前的块是否被破坏?
	bp = (nBlockPostfix *) (pc + bh->size);
    if (bp->magic != (int)L_POSTFIX_MAGIC_COOKIE) 
	{
		TCHAR szBuffer[1024];
		_stprintf(szBuffer,_T("Free(): END OF MEM BLOCK CORRUPTED\nFile:%s\nLines:%d"),bh->src_name, bh->src_line);  
		CriticalError(szBuffer);
    }

	// 检查释放方法是否匹配
	bool bMatched = true;
	switch ( type )
	{
	case MM_DELETE:
		if( bh->allocationType != MM_NEW )
			bMatched = false;
		break;
	case MM_DELETE_ARRAY:
		if( bh->allocationType != MM_NEW_ARRAY )
			bMatched = false;
		break;
	case MM_FREE:
		if( (bh->allocationType != MM_MALLOC) &&
			(bh->allocationType != MM_CALLOC) &&
			(bh->allocationType != MM_REALLOC))
			bMatched = false;
		break;
	}
	if(!bMatched)
	{
		TCHAR szBuffer[1024];
		_stprintf(szBuffer,_T("Free(): BLOCK TYPE NOT MATCH\nFile:%s\nLines:%d"),bh->src_name, bh->src_line);  
		CriticalError(szBuffer);
	}

    // 内存状态变量
    n_appused -= bh->size;

    // 改变标签内容
    strcpy(bh->src_name,"<freed>");
    bh->src_line = 0;
	bh->allocationType = MM_UNKNOWN;

	memset(pc,0xef,bh->size);

    if (((size_t)bh->next) == -1L) 
	{
        // Block wurde als einzelner Chunk allokiert
        nChunkHeader *ch = (nChunkHeader *) (((BYTE *)bh) - sizeof(nChunkHeader));
        bh->size = 0;
        this->FreeChunk(ch);
    } 
	else 
	{
        // Block wurde abgehandelt
        size_t i = (size_t) bh->next;
        nBlockHeader *next = blocks[i];
        blocks[i] = bh;
        bh->next = next;
        bh->size = 0;
    }

	// 从 hash 表中删除
	myHashTable.erase(address); 
	return true;
}



bool CSysMemory::CheckPtr(void * mem)
{
	BYTE *pc = (BYTE *)mem;
	if(!pc)
		return TRUE;
	
    CSGUARD;
	
	nBlockHeader  *bh = (nBlockHeader *) (pc - sizeof(nBlockHeader));
    nBlockPostfix *bp;
    // 检查块头标记当前的块是否被破坏?
    if (bh->magic != (int)L_PREFIX_MAGIC_COOKIE) 
	{
		CriticalError(_T("CheckPtr(): START OF MEM BLOCK CORRUPTED"));
		return FALSE;
    }
    
	// 检查块尾标记当前的块是否被破坏?
	bp = (nBlockPostfix *) (pc + bh->size);
    if (bp->magic != (int)L_POSTFIX_MAGIC_COOKIE) 
	{
		CriticalError(_T("Free(): END OF MEM BLOCK CORRUPTED"));
		return FALSE;
    }
	return TRUE;
}

void CSysMemory::DumpAllocs(const TCHAR * file)
{
    CSGUARD;
	
	nChunkHeader *ch = chunksHead;

	FILE * fp = _tfopen(file, _T("wt"));
    _ftprintf(fp, _T("=============Alloced Memory Dump=================\n"));
	int n_memnumalloc = 0;

	while (ch->next)	// 从大块的头指针开始，到尾指针为止。尾指针的Next指向chunksTail(NULL)
	{
        BYTE *b = (BYTE *)ch + sizeof(nChunkHeader);// 大块的实际指针	
        int i;
        
		// 报告大块中所有的小块信息
		for (i=0; i<ch->bnum; i++,b+=ch->bsize) 
		{
            nBlockHeader *bh = (nBlockHeader *)b;
            if (bh->size != 0) 
			{
				_ftprintf(fp, _T("src : %s, line : %d, size %d\n"),bh->src_name, bh->src_line, bh->size);
				n_memnumalloc ++;
            }

        }
		ch = ch->next; 
    }

	_ftprintf(fp, _T("still alloced memory blocks %d not be free\n"), n_memnumalloc);


	fclose(fp);
	fp = NULL;
}

void CSysMemory::MemoryInfo(size_t &nPoosUsed, size_t &nAppUsed)
{
	CSGUARD;
	nPoosUsed = this->n_poolused;
	nAppUsed = this->n_appused;
}

void CSysMemory::CriticalError(const TCHAR * msg)
{
	int ret = MessageBox(NULL, msg, _T("CSysMemory Critical Error") , MB_ABORTRETRYIGNORE | MB_ICONSTOP | MB_APPLMODAL);
    switch (ret)
    {
    case IDABORT:
        exit(-1);
    case IDRETRY:
        _asm int 3;
    case IDIGNORE:
        break;
    }
}
