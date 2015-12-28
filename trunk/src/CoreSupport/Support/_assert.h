#pragma once

// 在 Debug 版本和 Release 版本同时起作用的 assert

#include <crtdefs.h>
#undef  _assert

#ifdef  __cplusplus
extern "C" {
#endif

_CRTIMP void __cdecl _wassert(_In_z_ const wchar_t * _Message, _In_z_ const wchar_t *_File, _In_ unsigned _Line);

#ifdef  __cplusplus
}
#endif

#define _assert(_Expression) (void)( (!!(_Expression)) || (_wassert(_CRT_WIDE(#_Expression), _CRT_WIDE(__FILE__), __LINE__), 0) )
