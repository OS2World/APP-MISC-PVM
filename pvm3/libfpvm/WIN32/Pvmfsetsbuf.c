
/* $Id: Pvmfsetsbuf.c,v 1.2 1998/09/21 20:27:40 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\include\pvm3.h"
#include "..\..\src\pvmwin.h"
#else 
#include "pvm3.h"
#endif

#include "pvm_consts.h"

void 
#ifdef IMA_WIN32_WATCOM
pvmfsetsbuf (mbuf, oldbuf)
#else
__fortran PVMFSETSBUF (mbuf, oldbuf)
#endif

int *mbuf, *oldbuf;
{
   *oldbuf = pvm_setsbuf(*mbuf);
}

