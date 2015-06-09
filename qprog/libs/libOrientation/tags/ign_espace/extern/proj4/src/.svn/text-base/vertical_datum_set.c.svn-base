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
#include <projects.h>
#include <pj_init.h>
#include <string.h>


/************************************************************************/
/*                           vertical_datum_set()                       */
/************************************************************************/

int vertical_datum_set(paralist *pl, PJ *projdef)
{
    char *s;
    const char *name , *geoidgrids, *z_shift;
    int i;

  /* set units */
    s = 0;
    if (name = pj_param(pl, "sh_units").s) { 
	for (i = 0; (s = pj_units[i].id) && strcmp(name, s) ; ++i) ;
	if (!s) { pj_errno = -7; return 1; }
	s = pj_units[i].to_meter;
    }
    if (s || (s = pj_param(pl, "sto_h_meter").s)) {
	projdef->to_h_meter = strtod(s, &s);
	if (*s == '/') /* ratio number */
		projdef->to_h_meter /= strtod(++s, 0);
	projdef->fr_h_meter = 1. / projdef->to_h_meter;
     } else
	projdef->to_h_meter = projdef->fr_h_meter = 1.;

/* -------------------------------------------------------------------- */
/*      Is there a vertical datum definition in the parameters list?  If so,     */
/*      add the defining values to the parameter list.  Note that       */
/*      this will append the vd_type definition as well as the          */
/*      zshift= and related parameters.  It should also be pointed     */
/*      out that the addition is permanent rather than temporary        */
/*      like most other keyword expansion so that the ellipse           */
/*      definition will last into the pj_ell_set() function called      */
/*      after this one.                                                 */
/* -------------------------------------------------------------------- */
 if( (name = pj_param(pl,"svd_datum").s) != NULL )
    {
        paralist *curr;
        const char *s;
        int i;

        /* find the end of the list, so we can add to it */
        for (curr = pl; curr && curr->next ; curr = curr->next) {}
        
        /* find the datum definition */
        for (i = 0; (s = vd_datums[i].id) && strcmp(name, s) ; ++i) {}

        if (!s) { pj_errno = -9; return 1; }

	if( vd_datums[i].id && strlen(vd_datums[i].id) > 0 ){
	    char	entry[100];
            strcpy( entry, "vd_id=" );
            strncat( entry, vd_datums[i].id, 80 );
            curr = curr->next = pj_mkparam(entry);
		}

        if( vd_datums[i].type && strlen(vd_datums[i].type) > 0 )
            curr = curr->next = pj_mkparam(vd_datums[i].type);

	if( vd_datums[i].defn && strlen(vd_datums[i].defn) > 0 )
            curr = curr->next = pj_mkparam(vd_datums[i].defn);
        
        if( vd_datums[i].geoidgrid && strlen(vd_datums[i].geoidgrid) > 0 )
            curr = curr->next = pj_mkparam(vd_datums[i].geoidgrid);

    }

/* -------------------------------------------------------------------- */
/*      Is there a external file definition with +h_init                 */
/*      add the defining values to the parameter list.  		*/
/* -------------------------------------------------------------------- */
	if (pj_param(pl, "tvd_init").i) {
	    paralist *curr = pl;
		paralist *last;   
        /* find the end of the list, so we can add to it */
            for (curr = pl; curr && curr->next ; curr = curr->next);

        last= curr;
		
	    if (!(curr = get_init(&pl, curr, pj_param(pl, "svd_init").s)))
			return 1;
	    if (curr == last) { pj_errno = -2; return 1; }
	}


/* -------------------------------------------------------------------- */
/* Test if vertical datum has identifiers			        */
/* -------------------------------------------------------------------- */
    projdef->vertical_datum_type = VD_NO;
    projdef->vd_transform_type = VD_UNKNOWN;
    if( (name = pj_param(pl,"svd_type").s) != NULL )
    {
	if ( strcmp(name, "ALT")==0){
	projdef->vertical_datum_type = VD_ALT;}
	if ( strcmp(name, "HEIGHT")==0){
	projdef->vertical_datum_type = VD_HEIGHT;}
    }
   
/* -------------------------------------------------------------------- */
/* Test if datum is geocent eg 3D datum and not 2D planimetric+1D vertival */
/* -------------------------------------------------------------------- */
    if( projdef->is_geocent )
   {
	projdef->vertical_datum_type = VD_GEOCENT;
   }

/* -------------------------------------------------------------------- */
/*      Check for transform parameter.                                   */
/* -------------------------------------------------------------------- */
   projdef->z_shift[0]=0;
   projdef->z_shift[1]=1;
     if( (z_shift = pj_param(pl,"sz_shift").s) != NULL )
    {
	int    parm_count = 0;
        const char *s;
	 memset( projdef->z_shift, 0, sizeof(double) * 2);
        /* parse out the parameters */
        s = z_shift;
        for( s = z_shift; *s != '\0' && parm_count < 2; ) 
        {
            projdef->z_shift[parm_count++] = atof(s);
            while( *s != '\0' && *s != ',' )
                s++;
            if( *s == ',' )
                s++;
        }
      
    }

    if( (geoidgrids = pj_param(pl,"sgeoidgrids").s) != NULL )
    {
        /* We don't actually save the value separately.  It will continue
           to exist in the param list for use in vd_apply_geoidshift.c */
        projdef->vd_transform_type = VD_GEOIDGRID;
    }
	

    return 0;
}
