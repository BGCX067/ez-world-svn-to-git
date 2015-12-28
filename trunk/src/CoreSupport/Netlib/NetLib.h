
#pragma once

#include "NetInterface.h"
#include "EzModule.h"

NETLIB_EXPORTS_API iModule * CreateModule(const iInitParam * param, ISystem * pSystem);
NETLIB_EXPORTS_API void DestroyModule(iModule *& pModule);
