
/* $Id: Pvmfsiblings.c,v 1.2 1998/09/21 20:27:42 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\src\pvmwin.h"
#endif
#include "pvm3.h"
#include "pvm_consts.h"

void 
#ifdef IMA_WIN32_WATCOM
pvmfsiblings (nsiblings, sibidx, stid)
#else
__fortran PVMFSIBLINGS (nsiblings, sibidx, stid)
#endif

int *nsiblings, *sibidx, *stid;
{
	static int nsib = -1;
	static int *sibs;
	if (nsib == -1)
		nsib = pvm_siblings(&sibs);

	*nsiblings = nsib;

	if (*sibidx >= 0 && *sibidx< nsib)
	{
		*stid = sibs[*sibidx];
	}
	else
		*stid = PvmNoTask;
}

