#pragma once

#include "tchar.h"
#include "string"

#ifdef UNICODE
typedef std::wstring t_string;
#else
typedef std::string	t_string;
#endif // !UNICODE

#include <sstream>
#ifdef UNICODE
typedef std::wstringstream t_stringstream;

#else
typedef std::stringstream t_stringstream;
#endif // !UNICODE
