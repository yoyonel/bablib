/*
*  C Implementation: qg_gridlist
*
* Description: 
*
*
* Author: Frank Warmerdam <warmerdam@pobox.com>, (C) 2008
*
* Copyright: See COPYING file that comes with this distribution
*
*/
#define PJ_LIB__

#include <projects.h>
#include <string.h>
#include <math.h>

#ifdef _WIN32_WCE
/* assert.h includes all Windows API headers and causes 'LP' name clash.
 * Here assert we disable assert() for Windows CE.
 * TODO - mloskot: re-implement porting friendly assert
 */
# define assert(exp)	((void)0)
#else
# include <assert.h>
#endif /* _WIN32_WCE */

static QG_GRIDINFO *grid_list = NULL;

/* used only by qg_load_nadgrids() and qg_deallocate_grids() */

static int           last_geoidgrids_max = 0;
static int           last_geoidgrids_count = 0;
static QG_GRIDINFO **last_geoidgrids_list = NULL;
static char         *last_geoidgrids = NULL;

/************************************************************************/
/*                        qg_deallocate_grids()                         */
/*                                                                      */
/*      Deallocate all loaded grids.                                    */
/************************************************************************/

void qg_deallocate_grids()

{
    while( grid_list != NULL )
    {
        QG_GRIDINFO *item = grid_list;
        grid_list = grid_list->next;
        item->next = NULL;

        qg_gridinfo_free( item );
    }

    if( last_geoidgrids != NULL )
    {
        pj_dalloc( last_geoidgrids );
        last_geoidgrids = NULL;

        pj_dalloc( last_geoidgrids_list );
        last_geoidgrids_list = NULL;

        last_geoidgrids_count = 0;
        last_geoidgrids_max = 0;
    }
}

/************************************************************************/
/*                       qg_gridlist_merge_grid()                       */
/*                                                                      */
/*      Find/load the named gridfile and merge it into the              */
/*      last_geoidgrids_list.                                            */
/************************************************************************/

static int qg_gridlist_merge_gridfile( const char *gridname )

{
    int i, got_match=0;
    QG_GRIDINFO *this_grid, *tail = NULL;

/* -------------------------------------------------------------------- */
/*      Try to find in the existing list of loaded grids.  Add all      */
/*      matching grids as with NTv2 we can get many grids from one      */
/*      file (one shared gridname).                                     */
/* -------------------------------------------------------------------- */
    for( this_grid = grid_list; this_grid != NULL; this_grid = this_grid->next)
    {
        if( strcmp(this_grid->gridname,gridname) == 0 )
        {
            got_match = 1;

            /* dont add to the list if it is invalid. */
            if( this_grid->qg == NULL )
                return 0;

            /* do we need to grow the list? */
            if( last_geoidgrids_count >= last_geoidgrids_max - 2 )
            {
                QG_GRIDINFO **new_list;
                int new_max = last_geoidgrids_max + 20;

                new_list = (QG_GRIDINFO **) pj_malloc(sizeof(void*) * new_max);
                if( last_geoidgrids_list != NULL )
                {
                    memcpy( new_list, last_geoidgrids_list, 
                            sizeof(void*) * last_geoidgrids_max );
                    pj_dalloc( last_geoidgrids_list );
                }

                last_geoidgrids_list = new_list;
                last_geoidgrids_max = new_max;
            }

            /* add to the list */
            last_geoidgrids_list[last_geoidgrids_count++] = this_grid;
            last_geoidgrids_list[last_geoidgrids_count] = NULL;
        }

        tail = this_grid;
    }

    if( got_match )
        return 1;

/* -------------------------------------------------------------------- */
/*      Try to load the named grid.                                     */
/* -------------------------------------------------------------------- */
    this_grid = qg_gridinfo_init( gridname );

    if( this_grid == NULL )
    {
        /* we should get at least a stub grid with a missing "ct" member */
        assert( FALSE );
        return 0;
    }
    
    if( tail != NULL )
        tail->next = this_grid;
    else
        grid_list = this_grid;

/* -------------------------------------------------------------------- */
/*      Recurse to add the grid now that it is loaded.                  */
/* -------------------------------------------------------------------- */
    return qg_gridlist_merge_gridfile( gridname );
}

/************************************************************************/
/*                     pj_gridlist_from_nadgrids()                      */
/*                                                                      */
/*      This functions loads the list of grids corresponding to a       */
/*      particular nadgrids string into a list, and returns it.  The    */
/*      list is kept around till a request is made with a different     */
/*      string in order to cut down on the string parsing cost, and     */
/*      the cost of building the list of tables each time.              */
/************************************************************************/

QG_GRIDINFO **qg_gridlist_from_geoidgrids( const char *geoidgrids, int *grid_count)

{
    const char *s;

    pj_errno = 0;
    *grid_count = 0;

    if( last_geoidgrids != NULL 
        && strcmp(geoidgrids,last_geoidgrids) == 0 )
    {
        *grid_count = last_geoidgrids_count;
        if( *grid_count == 0 )
            pj_errno = -38;

        return last_geoidgrids_list;
    }

/* -------------------------------------------------------------------- */
/*      Free old one, if any, and make space for new list.              */
/* -------------------------------------------------------------------- */
    if( last_geoidgrids != NULL )
    {
        pj_dalloc(last_geoidgrids);
    }
    
    last_geoidgrids = (char *) pj_malloc(strlen(geoidgrids)+1);
    strcpy( last_geoidgrids, geoidgrids );

    last_geoidgrids_count = 0;

/* -------------------------------------------------------------------- */
/*      Loop processing names out of nadgrids one at a time.            */
/* -------------------------------------------------------------------- */
    for( s = geoidgrids; *s != '\0'; )
    {
        int   end_char;
        int   required = 1;
        char  name[128];

        if( *s == '@' )
        {
            required = 0;
            s++;
        }

        for( end_char = 0; 
             s[end_char] != '\0' && s[end_char] != ','; 
             end_char++ ) {}

        if( end_char > sizeof(name) )
        {
            pj_errno = -38;
            return NULL;
        }
        
        strncpy( name, s, end_char );
        name[end_char] = '\0';

        s += end_char;
        if( *s == ',' )
            s++;

        if( !qg_gridlist_merge_gridfile( name ) && required )
        {
            pj_errno = -38;
            return NULL;
        }
        else
            pj_errno = 0;
    }

    if( last_geoidgrids_count > 0 )
    {
        *grid_count = last_geoidgrids_count;
        return last_geoidgrids_list;
    }
    else
        return NULL;
}
