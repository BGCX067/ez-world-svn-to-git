// File name: "EnumToString.h"
#undef ENUM_ITEM
#undef BEGIN_ENUM
#undef END_ENUM

#undef ENUM_ITEM_NUM_EX
#undef ENUM_ITEM_EX
#undef BEGIN_ENUM_EX
#undef END_ENUM_EX

#ifdef GENERATE_ENUM_STRINGS

struct stEnumElement
{
	int nCmd;
	const char * lpszCommand;
};

#include <map>
#include <vector>
class EnumClass
{
	std::map<int,const char *> m_mapElements;
	std::vector<int> m_vecElements;
public:
	EnumClass(stEnumElement elements[],int nCount)
	{
		int nLastIndex = 0;
		for (int n=0;n<nCount;n++)
		{
			if(elements[n].nCmd == -1)
				elements[n].nCmd = nLastIndex;
			else
				nLastIndex = elements[n].nCmd;
			nLastIndex ++;
			m_mapElements[elements[n].nCmd] = elements[n].lpszCommand;
			m_vecElements.push_back(elements[n].nCmd);
		}	
	}
	const char * GetString(int nElement)
	{
		std::map<int,const char *>::iterator i = m_mapElements.find(nElement);
		if(i != m_mapElements.end())
			return i->second;
		return NULL;
	}
	unsigned int GetCount(bool bLastMax)
	{
		return (unsigned int)m_mapElements.size() - (bLastMax?1:0);
	}
	int GetElement(int nIndex)
	{
		return m_vecElements[nIndex];
	}
};
#endif

#ifndef GENERATE_ENUM_STRINGS
    #define ENUM_ITEM( element ) element
    #define BEGIN_ENUM( ENUM_NAME ) typedef enum tag##ENUM_NAME
    #define END_ENUM( ENUM_NAME ) ENUM_NAME; char* GetString##ENUM_NAME(int nElement);\
											unsigned int GetCount##ENUM_NAME(bool bLastMax);\
											int GetElement##ENUM_NAME(int nIndex);	

	#define ENUM_ITEM_NUM_EX( element, num ) element = num
	#define ENUM_ITEM_EX( element ) element
	#define BEGIN_ENUM_EX( ENUM_NAME ) typedef enum tag##ENUM_NAME
	#define END_ENUM_EX( ENUM_NAME ) ENUM_NAME;const char* GetString##ENUM_NAME(int element);\
											unsigned int GetCount##ENUM_NAME(bool bLastMax);\
											int GetElement##ENUM_NAME(int nIndex);	
#else
    #define ENUM_ITEM( element ) #element
    #define BEGIN_ENUM( ENUM_NAME ) char* gs_##ENUM_NAME [] =
    #define END_ENUM( ENUM_NAME ) ; char* GetString##ENUM_NAME(int index){ return gs_##ENUM_NAME [index]; }\
									unsigned int GetCount##ENUM_NAME(bool bLastMax){return sizeof(gs_##ENUM_NAME)/sizeof(char*) - (bLastMax?1:0);}\
									int GetElement##ENUM_NAME(int nIndex){return nIndex;}

	#define ENUM_ITEM_NUM_EX( element, num ) {num,#element}
	#define ENUM_ITEM_EX( element ) {-1,#element}
	#define BEGIN_ENUM_EX( ENUM_NAME ) stEnumElement gs_##ENUM_NAME [] =
	#define END_ENUM_EX( ENUM_NAME ) ; EnumClass gc_##ENUM_NAME(gs_##ENUM_NAME,sizeof(gs_##ENUM_NAME)/sizeof(stEnumElement));\
									const char* GetString##ENUM_NAME(int index){ return gc_##ENUM_NAME.GetString(index);}\
									unsigned int GetCount##ENUM_NAME(bool bLastMax){return gc_##ENUM_NAME.GetCount(bLastMax);}\
									int GetElement##ENUM_NAME(int nIndex){return gc_##ENUM_NAME.GetElement(nIndex);}
									
#endif

