#ifndef _MODULE_MANAGER_H
#define _MODULE_MANAGER_H

#include <vector>
#include "readerwriter.h"

class IFuncModel
{
public:
	static iSocketIO * m_pSessiones;
	static cReader * m_pReader;
	static cWriter * m_pWriter;



	virtual void OnClientReqest(UINT cmd,LPVOID param) = 0;
	virtual void OnRcvDBResponse(DbCommand eDbCommand,UINT nSession,LPVOID param) = 0;
	virtual void OnClientReqestDefault	(UINT cmd,LPVOID param){}
	void Send(unsigned nSession)
	{
		m_pSessiones->Write(nSession,m_pWriter->Buffer(),m_pWriter->Length());
	}
};

struct Module
{
	IFuncModel * pModule;
	int nClientCmdRange[2];
	int nDBCmdRange[2];
};


class CModuleManager
{
	std::vector<Module> m_vecModules;
public:
	CModuleManager(){}
	void AddModule(const Module &m);
	bool OnClientRequest(int nCmd,LPVOID param);
	bool OnRcvDBResult(DbCommand eDbCommand,UINT nSession,LPVOID param);
	~CModuleManager(){}
};

#endif
