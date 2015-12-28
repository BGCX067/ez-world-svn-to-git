#ifndef _READER_WRITER_
#define _READER_WRITER_
#include <assert.h>
#include <string.h>
//#include "vwutil.h"

class CBEGIN
{
};
class cWriter
{
	char m_szBuffer[65536];
	size_t m_nLength;

public:
	cWriter(){m_nLength = 0;}
	~cWriter(){m_nLength = 0;}
	char * Buffer(){return m_szBuffer;}
	size_t Length(){return m_nLength;}
	
	cWriter & operator <<(const char *pc)
	{
		String(pc,strlen(pc));
		return *this;
	}

	cWriter & operator <<(char *pc)
	{
		String(pc,strlen(pc));
		return *this;
	}


	cWriter & operator <<(const CBEGIN &)
	{
		m_nLength=0;
		return *this;
	}

	template<typename T> cWriter & operator <<(const T &t)
	{
		if(m_nLength+sizeof(t)>sizeof(m_szBuffer))
		{
			assert(0);
			return *this;
		}
		*(T *)(m_szBuffer + m_nLength) = t;
		m_nLength+=sizeof(t);
		return *this;
	}
	void Begin(int cmd)
	{
		*(int*)m_szBuffer = cmd;
		m_nLength = sizeof(int);
	}
	void Clear()
	{
		m_nLength = 0;
	}
	void UCHAR(unsigned char uc)
	{
		if(m_nLength+sizeof(unsigned char)>sizeof(m_szBuffer))
		{
			assert(0);
			return;
		}
		*(unsigned char *)(m_szBuffer + m_nLength) = uc;
		m_nLength += sizeof(unsigned char);
	}
	void CHAR(char c)
	{
		if(m_nLength+sizeof(char)>sizeof(m_szBuffer))
		{
			assert(0);
			return;
		}
		*(char*)(m_szBuffer + m_nLength) = c;
		m_nLength+=sizeof(char);
	}
	void USHORT(unsigned short us)
	{
		if(m_nLength+sizeof(unsigned short)>sizeof(m_szBuffer))
		{
			assert(0);
			return;
		}
		*(unsigned short *)(m_szBuffer + m_nLength) = us;
		m_nLength+=sizeof(unsigned short);
	}
	void SHORT(short s)
	{
		if(m_nLength+sizeof(short)>sizeof(m_szBuffer))
		{
			assert(0);
			return;
		}
		*(short*)(m_szBuffer + m_nLength) = s;
		m_nLength+=sizeof(short);
	}
	void ULONG(unsigned long ul)
	{
		if(m_nLength+sizeof(unsigned long)>sizeof(m_szBuffer))
		{
			assert(0);
			return;
		}
		*(unsigned long*)(m_szBuffer + m_nLength) = ul;
		m_nLength+=sizeof(unsigned long);
	}
	void LONG(long l)
	{
		if(m_nLength+sizeof(long)>sizeof(m_szBuffer))
		{
			assert(0);
			return;
		}
		*(long*)(m_szBuffer + m_nLength) = l;
		m_nLength+=sizeof(long);
	}
	void UINT(unsigned int un)
	{
		if(m_nLength+sizeof(unsigned int)>sizeof(m_szBuffer))
		{
			assert(0);
			return;
		}
		*(unsigned int *)(m_szBuffer + m_nLength) = un;
		m_nLength+=sizeof(unsigned int);
	}
	void INT(int n)
	{
		if(m_nLength+sizeof(int)>sizeof(m_szBuffer))
		{
			assert(0);
			return;
		}
		*(int*)(m_szBuffer + m_nLength) = n;
		m_nLength+=sizeof(int);
	}
	void Struct(const void * s,size_t size)
	{
		if(m_nLength+size>sizeof(m_szBuffer))
		{
			assert(0);
			return;
		}
		memcpy(m_szBuffer + m_nLength,s,size);
		m_nLength+=size;
	}

	void *Reserve(size_t size)
	{
		if(m_nLength+size>sizeof(m_szBuffer))
		{
			assert(0);
			return m_szBuffer+m_nLength;
		}
		size_t curoff=m_nLength;
		m_nLength+=size;
		return m_szBuffer+curoff;
	}

	void String(const char * s,size_t len)
	{
		if(m_nLength+len+1>sizeof(m_szBuffer))
		{
			assert(0);
			return;
		}
		memcpy(m_szBuffer + m_nLength,s,len+1);
		m_nLength+=len+1;
	}
};

#define _READBUFFERLENGTH 65536

class cReader
{
	char m_szBuffer[_READBUFFERLENGTH];
	size_t m_offset;
	size_t m_nLength;
	//unsigned long m_bPerformance;
public:
	cReader()
	{
		m_offset=0;
		m_nLength = 0;
		//m_bPerformance=0;
	}
	~cReader()
	{
	}
	void clear()
	{
		m_offset=0;
		m_nLength=0;
	}
	void SetLength(size_t length)
	{
		assert(length<=sizeof(m_szBuffer));
		m_nLength=length;
	}
	size_t GetOffset(){return m_offset;}
	void Rebegin(size_t offset){ m_offset = offset;}

	char * Buffer()
	{
		return m_szBuffer;
	}
	size_t GetOffset()const
	{
		return m_offset;
	}
	size_t GetLeft()const
	{
		return m_nLength-m_offset;
	}
	size_t GetLength()
	{
		return m_nLength;
	}
	size_t GetCapacity()
	{
		return _READBUFFERLENGTH;
	}
	cReader & operator >>(const char * &pchar)
	{
		size_t len;
		pchar=String(len);
		return *this;
	}

	cReader & operator >>(char * &pchar)
	{
		size_t len;
		pchar=String(len);
		return *this;
	}

	template<typename T> cReader & operator >>(T &t)
	{
		if(m_offset+sizeof(T)<=m_nLength)
		{
			t=*(T *)(m_szBuffer + m_offset);
			m_offset+=sizeof(T);
		}else
		{
			assert(0);
		}
		return *this;
	}

	int Begin(/*unsigned long bPerformance = 0*/)
	{
		//为什么m_Reader.begin()后，内部的长度指示没有变哪？
		//这里竟然少了括号导致一直m_offset=0，郁闷~~~~~
		m_offset = sizeof(int);
		//m_bPerformance = bPerformance;
		return *(int*)m_szBuffer;
	}
	unsigned char UCHAR()
	{
		unsigned char uc;
		operator >>(uc);
		return uc;
	}
	char CHAR()
	{
		char c;
		operator >>(c);
		return c;
	}
	unsigned short USHORT()
	{
		unsigned short us;
		operator >>(us);
		return us;
	}
	short SHORT()
	{
		short s;
		operator >>(s);
		return s;
	}
	unsigned long ULONG()
	{
		unsigned long ul;
		operator >>(ul);
		return ul;
	}
	long LONG()
	{
		long l;
		operator >>(l);
		return l;
	}
	unsigned int UINT()
	{
		unsigned int un;
		operator >>(un);
		return un;
	}
	int INT()
	{
		int n;
		operator >>(n);
		return n;
	}
	void * Struct(size_t size)
	{
		void * p = m_szBuffer + m_offset;
		assert(m_offset+size<=m_nLength);
		m_offset+=size;
		return p;
	}
	char * String(size_t& len)//返回时字符串长度放在len里面，
	{
		char * p = m_szBuffer + m_offset;
		len = strnlen(m_szBuffer + m_offset,m_nLength-(unsigned)m_offset);
		if(len>=m_nLength-(unsigned)m_offset)
		{
			assert(0);
			return NULL;
		}
		m_offset+=len+1;
		return p;
	}

	const char  *GetCurBuf()
	{
		return m_szBuffer + m_offset;
	}

};
#endif
