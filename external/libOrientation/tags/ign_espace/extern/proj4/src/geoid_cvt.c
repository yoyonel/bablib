/*! \file geoid_cvt.c
\ingroup v_datum
\brief compute altitude to height shift for a latitude longitude coordinates
*/
#ifndef lint
static const char SCCSID[]="@(#)geoid_cvt.c";
static int debug_count =0;
#endif
#define PJ_LIB__
#include <projects.h>
#define MAX_TRY 9
#define TOL 1e-12
	double
geoid_cvt(LP in, int inverse, struct QG_TABLE *qg) {
	int debug_flag = getenv( "PROJ_DEBUG" ) != NULL;
	LP tb;
	double z;
	if (in.lam == HUGE_VAL)
		return HUGE_VAL;
	/* normalize input to ll origin */
	tb = in;
	tb.lam -= qg->ll.lam;
	tb.phi -= qg->ll.phi;
	tb.lam = adjlon(tb.lam - PI) + PI;
		
	z = geoid_intr(tb, qg);
	if (inverse) {
		if (z == HUGE_VAL)
			return HUGE_VAL;
		else {
			return -z;
		}	
	} 
	else {
		if (z == HUGE_VAL)
			return HUGE_VAL;
		else {
			return z;
		}
	}
	
}

