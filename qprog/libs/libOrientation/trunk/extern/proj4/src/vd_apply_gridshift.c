/*
*  C Implementation: vertical_datum_set
*
* Description: 
*
*
* Author: Nicolas DAVID <nicolas.david@ign.fr>, (C) 2008
*
* Copyright: See COPYING file that comes with this distribution
*
*/
#define PJ_LIB__

#include <projects.h>
#include <string.h>
#include <math.h>


/************************************************************************/
/*                         vd_apply_gridshift()                         */
/************************************************************************/

int vd_apply_gridshift( const char *geoidgrids, int inverse, 
                        long point_count, int point_offset,
                        double *x, double *y, double *z )
{
    int debug_flag = getenv( "PROJ_DEBUG" ) != NULL;
   

    int geoid_count = 0;
    QG_GRIDINFO   **geoids;
    int  i;
    static int debug_count = 0;

    pj_errno = 0;

    geoids = qg_gridlist_from_geoidgrids( geoidgrids, &geoid_count);
    if( geoids == NULL || geoid_count == 0 )
        return pj_errno=-45;

    for( i = 0; i < point_count; i++ )
    {
        long io = i * point_offset;
        LP   input;
		double output;
        int  igeoid;

        input.phi = y[io];
        input.lam = x[io];
        output = HUGE_VAL;
	
        /* keep trying till we find a table that works */
        for( igeoid = 0; igeoid < geoid_count; igeoid++ )
        {
            QG_GRIDINFO *qgi = geoids[igeoid];
            struct QG_TABLE *qg_t = qgi->qg;

            /* skip tables that don't match our point at all.  */
            if( qg_t->ll.phi > input.phi || qg_t->ll.lam > input.lam
                || qg_t->ll.phi + (qg_t->lim.phi-1) * qg_t->del.phi < input.phi
                || qg_t->ll.lam + (qg_t->lim.lam-1) * qg_t->del.lam < input.lam )
                continue;


            /* load the grid shift info if we don't have it. */
            if( qg_t->cvz == NULL && !qg_gridinfo_load( qgi ) )
            {
                pj_errno = -46;
                return pj_errno;
            }
            
	
            output = geoid_cvt( input, inverse, qg_t );
            if( output != HUGE_VAL )
            {
                if( debug_flag && debug_count++ < 20 )
                    fprintf( stderr,
                             "vd_apply_gridshift(): used %s %lf\n",
                             qg_t->id , output);
                break;
            }
        }

        if( output == HUGE_VAL )
        {
            if( debug_flag )
            {
                fprintf( stderr, 
                         "vd_apply_gridshift(): failed to find a geoid shift table for\n"
                         "                      location (%.7fdW,%.7fdN)\n",
                         x[io]*RAD_TO_DEG , 
                         y[io]*RAD_TO_DEG );
                fprintf( stderr, 
                         "   tried: %s\n", geoidgrids );
            }
        
            pj_errno = -38;
            return pj_errno;
        }
        else
        {
            z[io] += output;
        }
    }

return 0;
}

