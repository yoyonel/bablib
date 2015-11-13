/*
*  C Implementation: qg_grindinfo
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
#include <errno.h>

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
/*                             swap_words()                             */
/*                                                                      */
/*      Convert the byte order of the given word(s) in place.           */
/************************************************************************/

static int  byte_order_test = 1;
#define IS_LSB	(((unsigned char *) (&byte_order_test))[0] == 1)

static void swap_words( unsigned char *data, int word_size, int word_count )

{
    int	word;

    for( word = 0; word < word_count; word++ )
    {
        int	i;
        
        for( i = 0; i < word_size/2; i++ )
        {
            int	t;
            
            t = data[i];
            data[i] = data[word_size-i-1];
            data[word_size-i-1] = t;
        }
        
        data += word_size;
    }
}

/************************************************************************/
/*                          qg_gridinfo_free()                          */
/************************************************************************/

void qg_gridinfo_free( QG_GRIDINFO *qgi )

{
    if( qgi == NULL )
        return;


    if( qgi->qg != NULL )
        geoid_free( qgi->qg );
    
    free( qgi->gridname );
    if( qgi->filename != NULL )
        free( qgi->filename );

    pj_dalloc( qgi );
}

/************************************************************************/
/*                          qg_gridinfo_load()                          */
/*                                                                      */
/*      This function is intended to implement delayed loading of       */
/*      the data contents of a grid file.  The header and related       */
/*      stuff are loaded by qg_gridinfo_init().                         */
/************************************************************************/

int qg_gridinfo_load( QG_GRIDINFO *qgi )

{

    int debug_flag = getenv( "PROJ_DEBUG" ) != NULL;
	if( debug_flag ) { fprintf( stderr, " int qg_gridinfo_load( QG_GRIDINFO *qgi )  \n");}
    
    if( strcmp(qgi->format,"gra") == 0 )
    {
        int	row;
        FILE *fid;

        fid = pj_open_lib( qgi->filename, "rb" );
        
        if( fid == NULL )
        {
            pj_errno = -38;
            return 0;
        }

	fseek( fid, qgi->grid_offset, SEEK_SET );
        qgi->qg->cvz = (double *) pj_malloc(qgi->qg->lim.lam*qgi->qg->lim.phi*sizeof(double));
	
	if(  qgi->qg->cvz == NULL )
        {
            pj_errno = -38;
            return 0;
        }

	for( row = 0; row < qgi->qg->lim.phi; row++ )
        {
            int	    i;
            double     *cvz;

            for( i = 0; i < qgi->qg->lim.lam; i++ )
            {
		double z_tmp;
		 if( fscanf( fid, "%lf", &z_tmp)==EOF )
            	{
                	pj_dalloc( qgi->qg->cvz );
                	pj_errno = -38;
			if( debug_flag ) { fprintf( stderr, "vd_apply_gridshift(): error while loading table  \n");}
                	return 0;
            	}
                cvz = qgi->qg->cvz + (qgi->qg->lim.phi - row - 1) * qgi->qg->lim.lam
                    + (i);

                *cvz = z_tmp;
            }
        }

        fclose( fid ); 
        return 1;
    }
	
	if ( strcmp ( qgi->format,"bin" ) == 0 )
	{
		int row=0;
		FILE *fid;
		fid = pj_open_lib ( qgi->filename, "rb" );
		if ( fid == NULL )
		{
			return 1;
		}
		fseek( fid, qgi->grid_offset, SEEK_SET );
		/* read all the actual shift values */
		qgi->qg->cvz = ( double * ) pj_malloc ( qgi->qg->lim.lam*qgi->qg->lim.phi*sizeof ( double ) );
		if ( qgi->qg->cvz == NULL )
		{
			return 1;
		}

		for ( row = 0; row < qgi->qg->lim.phi; row++ )
		{
			int	    i=0;
			double     *cvz;
			for ( i = 0; i < qgi->qg->lim.lam; i++ )
			{
 				double zd_tmp;
				float zf_tmp;
 				if ( fread (  &zf_tmp, 4, 1, fid  )==EOF )
 				{
 					pj_dalloc ( qgi->qg->cvz );
					fclose (fid);
 					if ( debug_flag ) { fprintf ( stderr, "geoid: error while loading bin table  \n" );}
 					return 1;
 				}
 				if( !IS_LSB )swap_words( (unsigned char*)(&zf_tmp), 4, 1);
 				zd_tmp=(double)(zf_tmp);
 				cvz = qgi->qg->cvz + ( row) * qgi->qg->lim.lam + i;
 				*cvz = zd_tmp;
			}
		}
		if(fclose (fid)==EOF){if ( debug_flag) { fprintf ( stderr, "  error while closing file" );}}
    	return 0;
	}

return 0;

}

/************************************************************************/
/*                       qg_gridinfo_init_gra                          */
/*                                                                      */
/*      Load a gra geoid file (IGN format)                              */
/************************************************************************/

static int qg_gridinfo_init_gra( FILE *fid, QG_GRIDINFO *qgilist )
{
    struct QG_TABLE *qg;
    LP		ur;
    double a,b;
    char row[200];

        int debug_flag = getenv( "PROJ_DEBUG" ) != NULL;
	if( debug_flag ) { fprintf( stderr, " static int qg_gridinfo_init_gra( FILE *fid, QG_GRIDINFO *qgilist )  \n");}
    
   qg = (struct QG_TABLE *) pj_malloc(sizeof(struct QG_TABLE));
   strcpy( qg->id, "gra Geiod Grid Shift File" );

   if( fgets (row, 200 , fid)==NULL)
    {
        pj_errno = -104;
        printf("wrong first ligne for gra file ");
        return 0;
    }
    if(sscanf (row,"%lf %lf",&a,&b)==EOF){
	 pj_errno = -104;
        printf("wrong first ligne for gra file ");
        return 0;
    }
    qg->ll.phi = a;
    ur.phi = b;

    if( fgets (row, 200 , fid)==NULL)
    {
        pj_errno = -104;
        printf("wrong second ligne for gra file ");
        return 0;
    }
    if(sscanf (row,"%lf %lf",&a,&b)==EOF){
	 pj_errno = -104;
        printf("wrong second ligne for gra file ");
        return 0;
    }
    qg->ll.lam =a ;
    ur.lam =b;

    if( fgets (row, 200 , fid)==NULL)
    {
        pj_errno = -104;
        printf("wrong third ligne for gra file ");
        return 0;
    }
    if(sscanf (row,"%lf %lf",&a,&b)==EOF){
	 pj_errno = -104;
        printf("wrong third ligne for gra file ");
        return 0;
    }
    qg->del.phi =a;
    qg->del.lam =b;

    //compute limite
    qg->lim.lam = (int) (fabs(ur.lam-qg->ll.lam)/qg->del.lam + 0.5) + 1;
    qg->lim.phi = (int) (fabs(ur.phi-qg->ll.phi)/qg->del.phi + 0.5) + 1;

    qg->del.lam *= DEG_TO_RAD;
    qg->del.phi *= DEG_TO_RAD;
    qg->ll.lam *= DEG_TO_RAD;
    qg->ll.phi *= DEG_TO_RAD;
	qg->cvz = NULL;
	
    qgilist->qg = qg;
    qgilist->grid_offset = ftell( fid );
    return 0;
}

/************************************************************************/
/*                       geoid_init_bin                                 */
/*                                                                      */
/*      Load a bin geoid file (american format)                              */
/************************************************************************/
static int qg_gridinfo_init_bin ( FILE *fid, QG_GRIDINFO *qgilist )

{
	unsigned char header[44];
	struct QG_TABLE *qg;

	assert ( sizeof ( int ) == 4 );
	assert ( sizeof ( double ) == 8 );
	if ( sizeof ( int ) != 4 || sizeof ( double ) != 8 )
	{
		fprintf ( stderr,
		          "basic types of inappropriate size in geoid_init_bin()\n" );
		return 1;
	}
	/* -------------------------------------------------------------------- */
	/*      Read the header.                                                */
	/* -------------------------------------------------------------------- */
	if ( fread ( header, sizeof ( header ), 1, fid ) != 1 )
	{
		return 1;
	}
	/* -------------------------------------------------------------------- */
	/*      Regularize fields of interest.                                  */
	/* -------------------------------------------------------------------- */
	if ( !IS_LSB )
	{
		swap_words ( header, 8, 1 );
		swap_words ( header+8, 8, 1 );
		swap_words ( header+16, 8, 1 );
		swap_words ( header+24, 8, 1 );
		swap_words ( header+32, 4, 1 );
		swap_words ( header+36, 4, 1 );
		swap_words ( header+40, 4, 1 );
	}
	/* -------------------------------------------------------------------- */
	/*      Fill in CTABLE structure.                                       */
	/* -------------------------------------------------------------------- */
	qg = ( struct QG_TABLE * ) pj_malloc ( sizeof ( struct QG_TABLE ) );
	strcpy ( qg->id, "bin Grid Geoid File" );

	qg->ll.lam = * ( ( double * ) ( header+8 ) );
	qg->ll.phi = * ( ( double * ) ( header+0 ) );
	qg->del.lam = * ( ( double * ) ( header+24 ) );
	qg->del.phi = * ( ( double * ) ( header+16 ) );
	qg->lim.lam = *( (int *)  ( header+36 ) ) ;
	qg->lim.phi = *( (int *)  ( header+32 ) )  ;


	qg->ll.lam *= DEG_TO_RAD;
 	qg->ll.lam = adjlon(qg->ll.lam);
	qg->ll.phi *= DEG_TO_RAD;
	qg->del.lam *= DEG_TO_RAD;
	qg->del.phi *= DEG_TO_RAD;
	qg->cvz = NULL;

	qgilist->qg = qg;
	qgilist->grid_offset = ftell ( fid );
	return 0;
}


/************************************************************************/
/*                          qg_gridinfo_init()                          */
/*                                                                      */
/*      Open and parse header details from a datum gridshift file       */
/*      returning a list of QG_GRIDINFOs for the grids in that          */
/*      file.  	                                                        */
/************************************************************************/

QG_GRIDINFO *qg_gridinfo_init( const char *gridname )

{
    char 	fname[MAX_PATH_FILENAME+1];
    QG_GRIDINFO *qgilist;
    FILE 	*fp;
    char	header[160];

    int debug_flag = getenv( "PROJ_DEBUG" ) != NULL;
    int gridname_size=strlen(gridname);
    char	extension[4];
    errno = pj_errno = 0;

/* -------------------------------------------------------------------- */
/*      Initialize a GRIDINFO with stub info we would use if it         */
/*      cannot be loaded.                                               */
/* -------------------------------------------------------------------- */
    qgilist = (QG_GRIDINFO *) pj_malloc(sizeof(QG_GRIDINFO));
    memset( qgilist, 0, sizeof(QG_GRIDINFO) );

    qgilist->gridname = strdup( gridname );
    qgilist->filename = NULL;
    qgilist->format = "missing";
    qgilist->grid_offset = 0;
    qgilist->qg = NULL;
    qgilist->next = NULL;
  
/* -------------------------------------------------------------------- */
/*      search extension, to determine the file type.                   */
/* -------------------------------------------------------------------- */
    strncpy(extension, gridname+(gridname_size-3),3);
    extension[3]='\0';
    if( !strcmp(extension,"gra") || !strcmp(extension,"GRA") ){
	 qgilist->format = "gra";	
    }
    if( !strcmp(extension,"bin") || !strcmp(extension,"BIN") ){
	 qgilist->format = "bin";	
    }

   if( debug_flag )
      	{
        	 fprintf( stderr, "vd_apply_gridshift(): %s geoid file  \n",qgilist->format);
     	 }

/* -------------------------------------------------------------------- */
/*      Open the file using the usual search rules.                     */
/* -------------------------------------------------------------------- */
    strcpy(fname, gridname);
    if (!strcmp(qgilist->format,"bin")){
    	if (!(fp = pj_open_lib(fname, "rb"))) {
        pj_errno = errno;
        return qgilist;
    	}
    }
    else if( !strcmp(qgilist->format,"gra") || !strcmp(qgilist->format,"asc") )
    {
	if (!(fp = pj_open_lib(fname, "rt"))) {
        pj_errno = errno;
        return qgilist;
    	}
    }
    else{
	if (!(fp = pj_open_lib(fname, "rb"))) {
        pj_errno = errno;
        return qgilist;
    	}
   }
    qgilist->filename = strdup(fname);
/* -------------------------------------------------------------------- */
/*      Load a header, to determine the file type.                      */
/* -------------------------------------------------------------------- */
    if(  !strcmp(qgilist->format,"gra"))
    {
      qg_gridinfo_init_gra( fp, qgilist );
    }
    if(  !strcmp(qgilist->format,"bin") )
    {
      qg_gridinfo_init_bin( fp, qgilist );
    }
    if( debug_flag && qgilist->qg !=NULL)
      {
        fprintf( stderr, "vd_apply_gridshift(): qg_table check :  \n");
	fprintf( stderr, "\t \t Lower left :\t %lf \t%lf  \n",qgilist->qg->ll.lam,qgilist->qg->ll.phi );
	fprintf( stderr, "\t \t resolution :\t %lf \t%lf \n",qgilist->qg->del.lam,qgilist->qg->del.phi);
	fprintf( stderr, "\t \t nb col row :\t %d \t%d \n",qgilist->qg->lim.lam,qgilist->qg->lim.phi);
	fprintf(stderr, " \t qg->cvz = %d \n", qgilist->qg->cvz); 
 	//fprintf(stderr, " \t *(qg->cvz)  = %12f \n", *(qgilist->qg->cvz) ); 	
     }


    fclose(fp);
    return qgilist;
}
