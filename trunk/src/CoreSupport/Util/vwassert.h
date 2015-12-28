#ifndef _VWASSERT_H
#define _VWASSERT_H
#ifdef _DEBUG
#define vwassert(exp) {if(!(exp))_asm{_asm int 3}; }
#define vwverify(exp) {if(!(exp))_asm{_asm int 3}; }
#else
#define vwassert(exp) (void(0))
#define vwverify(exp) (void(exp))
#endif

#endif