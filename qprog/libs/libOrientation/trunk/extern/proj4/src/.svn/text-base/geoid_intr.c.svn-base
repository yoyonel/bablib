/* Determine geoid table correction value */
/*! \file geoid_intr.c
\ingroup v_datum
\brief bilinear interpolation based on gridshift data for vertical datum transform
*/
#ifndef lint
static const char SCCSID[]="@(#)geoid_intr.c";
static int debug_count =0;

#endif
#define PJ_LIB__
#include <projects.h>
	double
geoid_intr(LP t, struct QG_TABLE *qg) {
	int debug_flag = getenv( "PROJ_DEBUG" ) != NULL;

	LP frct;
	ILP indx;
	double m00, m10, m01, m11;
	double z00, z10, z01, z11;
	long index;
	int in;

	double val= HUGE_VAL;

	indx.lam = floor(t.lam /= qg->del.lam);
	indx.phi = floor(t.phi /= qg->del.phi);
	frct.lam = t.lam - indx.lam;
	frct.phi = t.phi - indx.phi;

	 if( debug_flag > 1 )
      {
 	fprintf(stderr, " \t indx.lam = %d \n", indx.lam); 
 	fprintf(stderr, " \t indx.phi = %d \n", indx.phi ); 
 	fprintf(stderr, " \t frct.lam = %6f \n", frct.lam); 
 	fprintf(stderr, " \t frct.phi  = %6f \n", frct.phi ); 
      }	


	if (indx.lam < 0) {
		if (indx.lam == -1 && frct.lam > 0.99999999999) {
			++indx.lam;
			frct.lam = 0.;
		} else
			return val;
	} else if ((in = indx.lam + 1) >= qg->lim.lam) {
		if (in == qg->lim.lam && frct.lam < 1e-11) {
			--indx.lam;
			frct.lam = 1.;
		} else
			return val;
	}
	if (indx.phi < 0) {
		if (indx.phi == -1 && frct.phi > 0.99999999999) {
			++indx.phi;
			frct.phi = 0.;
		} else
			return val;
	} else if ((in = indx.phi + 1) >= qg->lim.phi) {
		if (in == qg->lim.phi && frct.phi < 1e-11) {
			--indx.phi;
			frct.phi = 1.;
		} else
			return val;
	}
	
	//get height value
	index = indx.phi * qg->lim.lam + indx.lam;
	 if( debug_flag > 1)
      {
 	fprintf(stderr, " \t index = %d \n", index); 
 	fprintf(stderr, " \t qg->cvz = %d \n", qg->cvz); 
 	fprintf(stderr, " \t *(qg->cvz)  = %12f \n", *(qg->cvz) ); 
 	
      }	
	z00 = *(qg->cvz + index++);
	z10 = *(qg->cvz + index);
	index += qg->lim.lam;
	z11 = *(qg->cvz + index--);
	z01 = *(qg->cvz + index);
	//interpolation
	m11 = m10 = frct.lam;
	m00 = m01 = 1. - frct.lam;
	m11 *= frct.phi;
	m01 *= frct.phi;
	frct.phi = 1. - frct.phi;
	m00 *= frct.phi;
	m10 *= frct.phi;
	val = m00 * z00 + m10 * z10 + m01 * z01 + m11 * z11;
	return val;
}
