#ifndef MODULE_SUPPORT_H
#define MODULE_SUPPORT_H

#include "singleton.h"

class ISystem;
class ILogInstance;
class CModuleSupport : public Singleton<CModuleSupport>
{
	ISystem * m_pSystem;
	ILogInstance * m_pLog;

public:
	CModuleSupport(	ISystem * pSystem, const TCHAR * lpszModuleName );
	ISystem * GetSystem()const{return m_pSystem;}
	ILogInstance * GetLog()const{return m_pLog;}
};

#define DLL_LOGGER	CModuleSupport::getSingletonPtr()->GetLog()
#define EFFC_BEGIN(name)	CModuleSupport::getSingletonPtr()->GetSystem()->GetSysEfficiency()->GetRecord(name)->Begin()
#define EFFC_END(name)	CModuleSupport::getSingletonPtr()->GetSystem()->GetSysEfficiency()->GetRecord(name)->End()
#define DLL_VFS		CModuleSupport::getSingletonPtr()->GetSystem()->GetSysVfs()
#define DLL_EFFC	CModuleSupport::getSingletonPtr()->GetSystem()->GetSysEfficiency()

#endif