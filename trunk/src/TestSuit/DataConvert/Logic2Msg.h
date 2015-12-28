#pragma once

#include "LogicEngine.h"
#include "LogicDefine.h"

class LogicEngin : public CDllLoader
{
	cWorld	*				m_pTheWorld;
	cBaseNode *				m_pWorldRoot;
public:
	LogicEngin();
	~LogicEngin();
	BOOL Init(const TCHAR * lpszDLL,const char * lpszFile);
	cWorld	* GetWorld(){return m_pTheWorld;}
	cBaseNode * GetRoot(){return m_pWorldRoot;}
};

struct UnitField
{
	int nUnitID;
	const char ** lpszFields;
	int nMaxFields;
};
BOOL ImportTemplate(cWorld	* pWorld,google::protobuf::Message * pTemplateRoot,const std::string & field,Logic::Unit_IDs eUnit,const char * lpszFields[],int nLevelCol);
BOOL ImportContact(cWorld	* pWorld,google::protobuf::Message * pTemplateRoot,Logic::Contact_IDs eContact,const char * lpszIndices[],const char * lpszFields[],UINT nIdxLevel);
