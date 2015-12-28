#ifndef EZ_MODULE_H
#define EZ_MODULE_H

class ISystem;
class iModule
{
public:
	virtual ~iModule(){};
};
class iInitParam
{
};

typedef iModule * (FnCreateModule)(const iInitParam * param, ISystem * pSystem);
typedef void (FnDestroyModule)(iModule *& pModule);
#define CreateModule_FN "CreateModule"
#define DestroyModule_FN "DestroyModule"

#endif