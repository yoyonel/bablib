/*
*  C Implementation: geoid_init
*
* Description: 
*
*
* Author: Frank Warmerdam <warmerdam@pobox.com>, (C) 2008
*
* Copyright: See COPYING file that comes with this distribution
*
*/
/*! \file geoid_init.c
\ingroup v_datum
\brief manage geoid table file and structure
*/
#define PJ_LIB__

#include <projects.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#ifdef _WIN32_WCE
/* assert.h includes all Windows API headers and causes 'LP' name clash.
 * Here assert we disable assert() for Windows CE.
 * TODO - mloskot: re-implement porting friendly assert
 */
# define assert(exp)	((void)0)
#else
# include <assert.h>
#endif /* _WIN32_WCE */


/************************************************************************/
/*                              geoid_free()                              */
/*                                                                      */
/*      Free a QG_TABLE grid shift structure produced by nad_init().      */
/************************************************************************/

void geoid_free(struct QG_TABLE *qg) 
{
    if (qg) {
        if( qg->cvz != NULL )
            pj_dalloc(qg->cvz);

        pj_dalloc(qg);
    }
}
