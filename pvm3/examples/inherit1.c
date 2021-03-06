
static char rcsid[] =
	"$Id: inherit1.c,v 1.3 1997/07/09 13:24:47 pvmsrc Exp $";

/*
 *         PVM version 3.4:  Parallel Virtual Machine System
 *               University of Tennessee, Knoxville TN.
 *           Oak Ridge National Laboratory, Oak Ridge TN.
 *                   Emory University, Atlanta GA.
 *      Authors:  J. J. Dongarra, G. E. Fagg, M. Fischer
 *          G. A. Geist, J. A. Kohl, R. J. Manchek, P. Mucci,
 *         P. M. Papadopoulos, S. L. Scott, and V. S. Sunderam
 *                   (C) 1997 All Rights Reserved
 *
 *                              NOTICE
 *
 * Permission to use, copy, modify, and distribute this software and
 * its documentation for any purpose and without fee is hereby granted
 * provided that the above copyright notice appear in all copies and
 * that both the copyright notice and this permission notice appear in
 * supporting documentation.
 *
 * Neither the Institutions (Emory University, Oak Ridge National
 * Laboratory, and University of Tennessee) nor the Authors make any
 * representations about the suitability of this software for any
 * purpose.  This software is provided ``as is'' without express or
 * implied warranty.
 *
 * PVM version 3 was funded in part by the U.S. Department of Energy,
 * the National Science Foundation and the State of Tennessee.
 */

/*
 *	Filename: 	inherit1.c
 *
 *  support program for inherita.c - read docs in that file
 */

#ifndef WIN32
#include <unistd.h>		/* for gethostname */
#else
#include "pvmwin.h"
#endif
#include "pvm3.h"

main()
{
	char *me = "inherit1";
	int cc, tid, ptid;
	char buf[100], buf2[100];
	char machine[25];
	int mycontext;

	ptid = pvm_parent();

	mycontext = pvm_getcontext();
	gethostname( machine, 25 );

	sprintf( buf, "%s t%x on machine <%s> with context %d.",
			me, pvm_mytid(), machine, mycontext );

	/* Send message to parent */
	pvm_initsend( PvmDataDefault );
	pvm_pkstr( buf );
	pvm_send( ptid, 1 );

	/*
	 *  Spawn a child process to confirm that it will inherit the
	 *   parent context.
	 */
	cc = pvm_spawn( "inherit2", (char **) 0, PvmTaskDefault, "", 1,
			&tid );
	if ( cc != 1 ){
		printf( "%s: can't start inherit2\n", me );
		pvm_exit();
		exit( 0 );
	}

	sprintf( buf,
			"%s: I just spawned inherit2 as tid t%x with context %d.\n",
			me, tid, pvm_getcontext() );

	/* Send message to parent */
	pvm_initsend( PvmDataDefault );
	pvm_pkstr( buf );
	pvm_send( ptid, 1 );

	/*
	 *  wait to receive message from child
	 */
	cc = pvm_recv( -1, -1 );
	pvm_bufinfo( cc, (int *) 0, (int *) 0, &tid );
	pvm_upkstr( buf2 );
	sprintf( buf, "%s: t%x %s\n", me, tid, buf2 );

	/* Send message to parent */
	pvm_initsend( PvmDataDefault );
	pvm_pkstr( buf );
	pvm_send( ptid, 1 );

	sprintf( buf, "%s t%x on machine <%s> with context %d.",
			me, pvm_mytid(), machine, mycontext );

	/* Send message to parent */
	pvm_initsend( PvmDataDefault );
	pvm_pkstr( buf );
	pvm_send( ptid, 1 );

	sprintf( buf,
			"END - sent from %s t%x on machine <%s> with context %d.",
			me, pvm_mytid(), machine, mycontext );

	/* Send message to parent */
	pvm_initsend( PvmDataDefault );
	pvm_pkstr( buf );
	pvm_send( ptid, 1 );

	pvm_exit();
	exit( 0 );
}

