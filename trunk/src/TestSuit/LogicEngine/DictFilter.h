#pragma once
#include "logicengine.h"
struct stItem
{
	char * lpszWord;
	size_t len;
};

class LOGIC_ENGINE cFilter
{
	char	*	m_pBuffer;
	stItem	*	m_pItems;
	UINT		m_nItems;

	int DichotomySearch(const char * pString);

public:
	cFilter();
	~cFilter();

	BOOL Init(LPCTSTR lpszDict,unsigned nlen);
	BOOL Initialize(const char * lpszDict);
	int FirstPos(const char * pString, BOOL bInvisbleAllow = TRUE,size_t * pnWidth = NULL);
	void Filter(char * pString,char cReplace);
	const char *GetString(int pos);
};
