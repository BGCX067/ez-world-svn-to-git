#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include "DictFilter.h"

int ItemCompare( const void *p1, const void *p2 )
{
	stItem * r1 = (stItem*)p1;
	stItem * r2 = (stItem*)p2;
	return strcmp((const char *)r1->lpszWord,(const char *)r2->lpszWord);
}

cFilter::cFilter()
{
	m_pBuffer = NULL;
	m_pItems = NULL;
	m_nItems = 0;
}

cFilter::~cFilter()
{
	if(m_pBuffer)
	{
		delete []m_pBuffer;
		delete []m_pItems;
	}
	m_nItems = 0;
}

BOOL cFilter::Init(const char *lpszDict,unsigned nLen)
{
	// 不能重复初始化
	if(m_pBuffer)
		return FALSE;

	nLen ++;
	m_pBuffer = new char[nLen];
	memcpy(m_pBuffer,lpszDict,nLen);
	m_pBuffer[nLen-1] = 0;

	// 把回车换行符号去掉
	int nCount = 0;
	for(unsigned long i=0;i<nLen;i++)
	{
		if((i<(nLen-1))&&(m_pBuffer[i] == 0x0d)&&(m_pBuffer[i+1] == 0x0a))
		{
			// 有个特殊情况，回车换行，本身也是词典中的一个。
			nCount ++;
			m_pBuffer[i] = 0;
			m_pBuffer[i+1] = 0;
		}
	}

	// 分配字典条目
	m_pItems = new stItem[nCount];

	int nLast = 0;
	char cLast = 0;
	for(unsigned i=0;i<nLen;i++)
	{
		if((m_pBuffer[i] == 0) && (cLast != 0))
		{
			m_pItems[m_nItems].lpszWord = m_pBuffer+nLast;
			m_pItems[m_nItems].len = strlen(m_pItems[m_nItems].lpszWord);
			m_nItems ++;
		}
		if((m_pBuffer[i] != 0) && (cLast == 0))
			nLast = i;
		cLast = m_pBuffer[i];
	}	

	qsort(m_pItems,m_nItems,sizeof(stItem),ItemCompare);
	return TRUE;
}

BOOL cFilter::Initialize(const char * lpszDict)
{
	// 不能重复初始化
	if(m_pBuffer)
		return FALSE;

	// 打开文件
	FILE * fp = fopen(lpszDict,"rb");
	if(!fp)
		return FALSE;

	// 确定长度
	fseek(fp,0,SEEK_END);
	long nLen = ftell(fp);
	if(nLen == 0)
	{
		fclose(fp);
		return FALSE;
	}
	nLen ++;
	
	// 读取文件
	fseek(fp,0,SEEK_SET);
	m_pBuffer = new char[nLen];
	fread(m_pBuffer,nLen,1,fp);
	fclose(fp);
	m_pBuffer[nLen-1] = 0;

	// 把回车换行符号去掉
	int nCount = 0;
	for(long i=0;i<nLen;i++)
	{
		if((i<(nLen-1))&&(m_pBuffer[i] == 0x0d)&&(m_pBuffer[i+1] == 0x0a))
		{
			// 有个特殊情况，回车换行，本身也是词典中的一个。
			nCount ++;
			m_pBuffer[i] = 0;
			m_pBuffer[i+1] = 0;
		}
	}

	// 分配字典条目
	if ( nCount>0 ) {			//nCount>0, 再做处理. [zhouqq]
		m_pItems = new stItem[nCount];
		int nLast = 0;
		char cLast = 0;
		for(long i=0;i<nLen;i++)
		{
			if((m_pBuffer[i] == 0) && (cLast != 0))
			{
				m_pItems[m_nItems].lpszWord = m_pBuffer+nLast;
				m_pItems[m_nItems].len = strlen(m_pItems[m_nItems].lpszWord);
				m_nItems ++;
			}
			if((m_pBuffer[i] != 0) && (cLast == 0))
				nLast = i;
			cLast = m_pBuffer[i];
		}	
		qsort(m_pItems,m_nItems,sizeof(stItem),ItemCompare);
	}
	
	return TRUE;
}

int cFilter::FirstPos(const char * pString,BOOL bInvisbleAllow, size_t * pnWidth)
{
	// 如果是汉字，
	const char * p = pString;
	while(*p)
	{
		// 不可见字符
		if(bInvisbleAllow && (*p>0) && (*p<32))
		{
			p++;
			continue;
		}
		
		int nPos = DichotomySearch(p);
		if(nPos>=0)
		{
			if(pnWidth)
				*pnWidth = m_pItems[nPos].len; 
			return (int)(p - pString);
		}

		if(*p<0)
			p+=2;
		else
			p++;
	}
	return -1;
}
void cFilter::Filter(char * pString,char cReplace)
{
	char * p = pString;
	size_t nWidth = 0;
	while(*p)
	{
		int nPos = FirstPos(p,TRUE,&nWidth);
		if(nPos<0)
			return;
		FillMemory(p+nPos,nWidth,cReplace);
		p += nWidth + nPos;
	}
}

int cFilter::DichotomySearch(const char * pString)
{
	int nStartPos, nEndPos, nCurPos, nResult;
	nStartPos = 0;
	nEndPos = m_nItems;
	nCurPos = (nEndPos+nStartPos)/2;
	while( (nCurPos > nStartPos) && (nCurPos < nEndPos) )
	{
		nResult = strncmp(m_pItems[nCurPos].lpszWord,pString,m_pItems[nCurPos].len);				// 索引
		if (nResult<0)
		{
			nStartPos = nCurPos;
			nCurPos = (nEndPos+nStartPos)/2;
		}
		else if (nResult>0)
		{
			nEndPos = nCurPos;
			nCurPos = (nEndPos+nStartPos)/2;
		}
		else 
		{
			return nCurPos;
		}
	}
	return -1;
}

const char *cFilter::GetString(int pos)
{
	if(pos<0||pos>=(int)m_nItems)
		return NULL;
	return m_pItems[pos].lpszWord;
}
