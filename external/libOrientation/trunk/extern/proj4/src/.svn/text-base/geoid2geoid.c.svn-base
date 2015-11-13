/*
*  C Implementation: geoid2geoid
*
* Description:
*
* GEOID FORMAT all geoid are converted on a geoid grid with :
* -longitude between a min of -PI  and a maximun of PI. (not between 0 and 2PI like egm96 ).
* -Internal grid begin on lower left corner (southernmost and westermost)
* and are organised by row from west to est. And row from south to north.
*/
/*! \file geoid2geoid.c
    \brief conversion tools between different geoids file formats

*/
#ifndef lint
static const char SCCSID[]="@(#)geoid2geoid.c	4.2 08/06/24 GIE REL";
#endif
#include <stdio.h>
#include <stdlib.h>
#define PJ_LIB__
#include <projects.h>
#include <string.h>
#include <math.h>
#include "emess.h"
#ifdef _WIN32_WCE
/* assert.h includes all Windows API headers and causes 'LP' name clash.
 * Here assert we disable assert() for Windows CE.
 */
# define assert(exp)	((void)0)
#else
# include <assert.h>
#endif /* _WIN32_WCE */
static char *usage = "%s\nusage: %s [ -io [args] ] [ files ]\n";
static int debug_flag=0;
/************************************************************************/
/*                             swap_words()                             */
/*                                                                      */
/*      Convert the byte order of the given word(s) in place.           */
/************************************************************************/
static int  byte_order_test = 1;
#define IS_LSB	(((unsigned char *) (&byte_order_test))[0] == 1)

static void swap_words ( unsigned char *data, int word_size, int word_count )
{
	int	word;
	for ( word = 0; word < word_count; word++ )
	{
		int	i;
		for ( i = 0; i < word_size/2; i++ )
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

void qg_free ( QG_GRIDINFO *qgi )
{
	if ( qgi == NULL )
		return;


	if ( qgi->qg != NULL )
		geoid_free ( qgi->qg );

	free ( qgi->gridname );
	if ( qgi->filename != NULL )
		free ( qgi->filename );

	pj_dalloc ( qgi );
}

/************************************************************************/
/*                          geoid_save()                      	        */
/*                                                                      */
/*      This function is intended to save geoid file on format          */
/*      supported by vd_apply_geoidshift ie only gra ascii file         */
/*      and binary bin file.
/************************************************************************/
int geoid_save ( QG_GRIDINFO *geoid , const char* format_out )
{
	FILE *fp;
	char* filename_out;
	int val_return=1;
	filename_out = ( char* ) malloc ( sizeof ( char ) * 300 );
	memset ( filename_out,0,300 );
	int filename_size=strlen ( geoid->filename );
	strncpy ( filename_out, geoid->filename,filename_size-3 );

	if ( strcmp ( format_out,"gra" ) == 0 )
	{
		strcat ( filename_out,"tmp.gra" );
		if ( debug_flag ) { fprintf ( stderr, "save geoid on gra format %s \n", filename_out );}
		if ( ( fp = fopen ( filename_out, "wt" ) ) == NULL )
		{
			fprintf ( stderr, "error file create new file %s \n", filename_out );
		}
		double e_lam, o_lam, s_phi, n_phi, delta_lam, delta_phi;
		o_lam= ( geoid->qg->ll.lam ) *RAD_TO_DEG;
		s_phi= ( geoid->qg->ll.phi ) *RAD_TO_DEG;
		delta_lam= ( geoid->qg->del.lam ) *RAD_TO_DEG;
		delta_phi= ( geoid->qg->del.phi ) *RAD_TO_DEG;
		e_lam=o_lam+ ( geoid->qg->lim.lam-1 ) *delta_lam;
		n_phi=s_phi+ ( geoid->qg->lim.phi-1 ) *delta_phi;
		fprintf ( fp, " %15.12f \t %15.12f \r\n %15.12f \t %15.12f \r\n %15.12f \t %15.12f \r\n",s_phi, n_phi, o_lam, e_lam, delta_phi, delta_lam );

		int row=0;
		for ( row = 0; row < geoid->qg->lim.phi; row++ )
		{
			int	    i;
			double     *cvz;
			for ( i = 0; i < geoid->qg->lim.lam; i++ )
			{
				double z_tmp;
				cvz = geoid->qg->cvz + ( geoid->qg->lim.phi - row - 1 ) * geoid->qg->lim.lam + ( i );
				z_tmp= *cvz;
				if ( i%8 == 7 ) {fprintf ( fp, " %6.4f\r\n", z_tmp );}
				else if ( i== ( geoid->qg->lim.lam-1 ) ) {fprintf ( fp, " %6.4f\r\n", z_tmp );}
				else{fprintf ( fp, " %6.4f\t",z_tmp ); }
			}
		}
		fclose ( fp );
		val_return=0;
	}
	else if ( strcmp ( format_out,"bin" ) == 0 )
	{
		strcat ( filename_out,"tmp.bin" );
		if ( debug_flag ) { fprintf ( stderr, "save geoid on bin format %s \n",filename_out );}
		if ( ( fp = fopen ( filename_out, "wb" ) ) == NULL )
		{
			fprintf ( stderr, "error file create new file %s \n", filename_out );
		}

		char* header;
		header = ( char* ) malloc ( sizeof ( char ) * 44 );
		memset ( header,0,44 );
		assert ( sizeof ( int ) == 4 );
		assert ( sizeof ( double ) == 8 );
		if ( sizeof ( int ) != 4 || sizeof ( double ) != 8 )
		{
			fprintf ( stderr,  "basic types of inappropriate size in geoid_save()\n" );
			return 1;
		}
		double* ll_phi= ( double * ) header;
		*ll_phi= ( geoid->qg->ll.phi*RAD_TO_DEG );
		double* ll_lam= ( double * ) ( header+8 );
		*ll_lam= ( geoid->qg->ll.lam*RAD_TO_DEG+360 );
		double* del_phi= ( double * ) ( header+16 );
		*del_phi= ( geoid->qg->del.phi*RAD_TO_DEG );
		double* del_lam= ( double * ) ( header+24 );
		*del_lam= ( geoid->qg->del.lam*RAD_TO_DEG );
		int* lim_phi= ( int* ) ( header+32 );
		*lim_phi= ( geoid->qg->lim.phi );
		int* lim_lam= ( int* ) ( header+36 );
		*lim_lam= ( geoid->qg->lim.lam );
		int* type= ( int* ) ( header+40 );
		*type=1;
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
		fwrite ( header, 44, 1, fp );

		unsigned char z_tmp[4];
		int row=0;
		for ( row = 0; row < geoid->qg->lim.phi; row++ )
		{
			int	    i;
			double     *cvz;
			for ( i = 0; i < geoid->qg->lim.lam; i++ )
			{
				double zd_tmp;
				cvz = geoid->qg->cvz + ( row ) * geoid->qg->lim.lam + i;
				zd_tmp=*cvz;
				/*			if ( debug_flag && row==0 ) { fprintf ( stderr, " %lf \t",zd_tmp );}*/
				float* zf_tmp= ( float* ) z_tmp;
				*zf_tmp= ( float ) zd_tmp;
				if ( !IS_LSB ) swap_words ( z_tmp, 4, 1 );
				fwrite ( z_tmp, 4, 1, fp );
			}
		}
		fclose ( fp );
		val_return=0;
	}
	else
	{
		if ( debug_flag ) { fprintf ( stderr, "wrong output geoid format, only gra and bin are supported as writing \n" );}
		val_return=1;
	}

	free ( filename_out );
	return val_return;
}

/************************************************************************/
/*                          geoid_load()                      	        */
/*                                                                      */
/*      This function is intended to implement delayed loading of       */
/*      the data contents of a grid file.  The header and related       */
/*      stuff are loaded by qg_gridinfo_init().                         */
/************************************************************************/
int geoid_load ( QG_GRIDINFO *geoid )
{

	if ( strcmp ( geoid->format,"gra" ) == 0 )
	{
		int	row;
		FILE *fid;
		fid = pj_open_lib ( geoid->filename, "rt" );
		if ( fid == NULL ) { return 0;}

		fseek ( fid, geoid->grid_offset, SEEK_SET );
		geoid->qg->cvz = ( double * ) pj_malloc ( geoid->qg->lim.lam*geoid->qg->lim.phi*sizeof ( double ) );

		if ( geoid->qg->cvz == NULL )
		{
			return 1;
		}

		for ( row = 0; row < geoid->qg->lim.phi; row++ )
		{
			int	    i;
			double     *cvz;
			for ( i = 0; i < geoid->qg->lim.lam; i++ )
			{
				double z_tmp;
				if ( fscanf ( fid, "%lf", &z_tmp ) ==EOF )
				{
					pj_dalloc ( geoid->qg->cvz );
					if ( debug_flag ) { fprintf ( stderr, "geoid: error while loading gra table  \n" );}
					return 1;
				}
				cvz = geoid->qg->cvz + ( geoid->qg->lim.phi - row - 1 ) * geoid->qg->lim.lam + ( i );
				*cvz = z_tmp;
			}
		}
		fclose ( fid );
		return 0;
	}

	if ( strcmp ( geoid->format,"ign_txt" ) == 0 )
	{
		int	col;
		FILE *fid;
		fid = pj_open_lib ( geoid->filename, "rt" );
		if ( fid == NULL )
		{
			return 1;
		}
		fseek ( fid, geoid->grid_offset, SEEK_SET );
		geoid->qg->cvz = ( double * ) pj_malloc ( geoid->qg->lim.lam*geoid->qg->lim.phi*sizeof ( double ) );
		if ( geoid->qg->cvz == NULL )
		{
			return 1;
		}
		for ( col = 0; col < geoid->qg->lim.lam; col++ )
		{
			int	    i;
			double     *cvz;
			for ( i = 0; i < geoid->qg->lim.phi; i++ )
			{
				double lambda, phi, z_tmp;
				int conf, in;
				if ( fscanf ( fid, "%lf %lf %lf %d", &lambda, &phi, &z_tmp, &conf ) ==EOF )
				{
					pj_dalloc ( geoid->qg->cvz );
					if ( debug_flag ) { fprintf ( stderr, "geoid: error while loading txt table  \n" );}
					return 1;
				}
				LP tb, frct, tbi;
				tb.lam = lambda*DEG_TO_RAD - geoid->qg->ll.lam;
				tb.phi = phi*DEG_TO_RAD - geoid->qg->ll.phi;
				ILP indx;
				tbi.lam= tb.lam/ ( geoid->qg->del.lam ) +0.25;
				tbi.phi= tb.phi/ ( geoid->qg->del.phi ) +0.25;
				indx.lam = floor ( tbi.lam );
				indx.phi = floor ( tbi.phi );
				frct.lam = tb.lam - indx.lam;
				frct.phi = tb.phi - indx.phi;
				if ( indx.lam < 0 )
				{
					if ( indx.lam == -1 && frct.lam > 0.99999999999 )
					{
						++indx.lam;
						frct.lam = 0.;
					}
					else
					{
						if ( debug_flag ) { fprintf ( stderr, "geoid: index error while loading txt table  \n" );};
						return 1;
					}
				}
				if ( indx.phi < 0 )
				{
					if ( indx.phi == -1 && frct.phi > 0.99999999999 )
					{
						++indx.phi;
						frct.phi = 0.;
					}
					else
					{
						if ( debug_flag ) { fprintf ( stderr, "geoid: index error while loading txt table  \n" );};
						return 1;
					}
				}
				cvz = geoid->qg->cvz + ( indx.phi ) * geoid->qg->lim.lam+ indx.lam;
				*cvz = z_tmp;
			}
		}
		fclose ( fid );
		return 0;
	}

	if ( strcmp ( geoid->format,"bin" ) == 0 )
	{
		int row=0;
		FILE *fid;
		fid = pj_open_lib ( geoid->filename, "rb" );
		if ( fid == NULL )
		{
			return 1;
		}
		fseek ( fid, geoid->grid_offset, SEEK_SET );
		/* read all the actual shift values */
		geoid->qg->cvz = ( double * ) pj_malloc ( geoid->qg->lim.lam*geoid->qg->lim.phi*sizeof ( double ) );
		if ( geoid->qg->cvz == NULL )
		{
			return 1;
		}

		for ( row = 0; row < geoid->qg->lim.phi; row++ )
		{
			int	    i=0;
			double     *cvz;
			for ( i = 0; i < geoid->qg->lim.lam; i++ )
			{
				double zd_tmp;
				float zf_tmp;
				if ( fread ( &zf_tmp, 4, 1, fid ) ==EOF )
				{
					pj_dalloc ( geoid->qg->cvz );
					fclose ( fid );
					if ( debug_flag ) { fprintf ( stderr, "geoid: error while loading bin table  \n" );}
					return 1;
				}
				if ( !IS_LSB ) swap_words ( ( unsigned char* ) ( &zf_tmp ), 4, 1 );
				zd_tmp= ( double ) ( zf_tmp );
				cvz = geoid->qg->cvz + ( row ) * geoid->qg->lim.lam + i;
				*cvz = zd_tmp;
			}
		}
		if ( fclose ( fid ) ==EOF ) {if ( debug_flag ) { fprintf ( stderr, "  error while closing file" );}}
		return 0;
	}

	if ( strcmp ( geoid->format,"egm" ) == 0 )
	{
		int row=0;
		FILE *fid;
		fid = pj_open_lib ( geoid->filename, "rb" );
		if ( fid == NULL )
		{
			return 1;
		}
		fseek ( fid, geoid->grid_offset, SEEK_SET );
		/* read all the actual shift values */
		geoid->qg->cvz = ( double * ) pj_malloc ( geoid->qg->lim.lam*geoid->qg->lim.phi*sizeof ( double ) );
		if ( geoid->qg->cvz == NULL )
		{
			return 1;
		}

		for ( row = 0; row < geoid->qg->lim.phi; row++ )
		{
			int	    i=0;
			double     *cvz;
			for ( i = 0; i < geoid->qg->lim.lam; i++ )
			{
				double zd_tmp;
				short z_cm;
				double lam;
				int index_lam;
				if ( fread ( &z_cm, 2, 1, fid ) ==EOF )
				{
					pj_dalloc ( geoid->qg->cvz );
					fclose ( fid );
					if ( debug_flag ) { fprintf ( stderr, "geoid: error while loading bin table  \n" );}
					return 1;
				}
				if ( IS_LSB ) swap_words ( ( unsigned char* ) ( &z_cm ), 2, 1 );
				zd_tmp= ( double ) ( z_cm/100.0 );
				lam=adjlon ( i*geoid->qg->del.lam )-geoid->qg->ll.lam;
				index_lam=floor ( lam/ ( geoid->qg->del.lam ) +0.5 );
				cvz = geoid->qg->cvz+ ( geoid->qg->lim.phi - row - 1 ) *geoid->qg->lim.lam + index_lam;
				/*				if ( debug_flag && row==0) { fprintf ( stderr, " %d \t %d \t %lf \t", row, index_lam, zd_tmp );}*/
				*cvz = zd_tmp;
			}
		}
		if ( fclose ( fid ) ==EOF ) {if ( debug_flag ) { fprintf ( stderr, " error while closing file" );}}
		return 0;
	}
	return 1;
}
/************************************************************************/
/*                       qg_gridinfo_init_gra                          */
/*                                                                      */
/*      Load a gra geoid file (IGN format)                              */
/************************************************************************/

static int geoid_init_gra ( FILE *fid, QG_GRIDINFO *geoid )
{
	struct QG_TABLE *qg;
	LP		ur;
	double a,b;

	qg = ( struct QG_TABLE * ) pj_malloc ( sizeof ( struct QG_TABLE ) );
	strcpy ( qg->id, "gra Geiod Grid Shift File" );

	char row[200];
	if ( fgets ( row, 200 , fid ) ==NULL )
	{
		printf ( "wrong first ligne for gra file " );
		return 1;
	}
	if ( sscanf ( row,"%lf %lf",&a,&b ) ==EOF )
	{
		printf ( "wrong first ligne for gra file " );
		return 1;
	}
	qg->ll.phi = a;
	ur.phi = b;

	if ( fgets ( row, 200 , fid ) ==NULL )
	{
		printf ( "wrong second ligne for gra file " );
		return 1;
	}
	if ( sscanf ( row,"%lf %lf",&a,&b ) ==EOF )
	{
		printf ( "wrong second ligne for gra file " );
		return 1;
	}
	qg->ll.lam =a ;
	ur.lam =b;

	if ( fgets ( row, 200 , fid ) ==NULL )
	{
		printf ( "wrong third ligne for gra file " );
		return 1;
	}
	if ( sscanf ( row,"%lf %lf",&a,&b ) ==EOF )
	{
		printf ( "wrong third ligne for gra file " );
		return 1;
	}
	qg->del.phi =a;
	qg->del.lam =b;

	//compute limite
	qg->lim.lam = ( int ) ( fabs ( ur.lam-qg->ll.lam ) /qg->del.lam + 0.5 ) + 1;
	qg->lim.phi = ( int ) ( fabs ( ur.phi-qg->ll.phi ) /qg->del.phi + 0.5 ) + 1;

	qg->del.lam *= DEG_TO_RAD;
	qg->del.phi *= DEG_TO_RAD;
	qg->ll.lam *= DEG_TO_RAD;
	qg->ll.phi *= DEG_TO_RAD;

	geoid->qg = qg;
	geoid->grid_offset = ftell ( fid );

	return 0;
}

/************************************************************************/
/*                       geoid_init_mnt	                                */
/*                                                                      */
/*      Load a french (IGN) .mnt geoid file                             */
/************************************************************************/

static int geoid_init_mnt ( FILE *fid, QG_GRIDINFO *geoid )
{
	struct QG_TABLE *qg;
	LP		ur;
	double a,b,c,d,e,f;

	qg = ( struct QG_TABLE * ) pj_malloc ( sizeof ( struct QG_TABLE ) );
	strcpy ( qg->id, "gra Geiod Grid Shift File" );

	char row[500];
	if ( fgets ( row, 500 , fid ) ==NULL )
	{
		printf ( "wrong first ligne for mnt file " );
		return 1;
	}
	if ( sscanf ( row,"%lf %lf %lf %lf %lf %lf",&a,&b,&c,&d,&e,&f ) ==EOF )
	{
		printf ( "wrong first ligne for mnt file " );
		return 1;
	}
	qg->ll.phi = c;
	ur.phi = d;
	qg->ll.lam =a ;
	ur.lam =b;
	qg->del.phi =f;
	qg->del.lam =e;

	//compute limite
	qg->lim.lam = ( int ) ( fabs ( ur.lam-qg->ll.lam ) /qg->del.lam + 0.5 ) + 1;
	qg->lim.phi = ( int ) ( fabs ( ur.phi-qg->ll.phi ) /qg->del.phi + 0.5 ) + 1;

	qg->del.lam *= DEG_TO_RAD;
	qg->del.phi *= DEG_TO_RAD;
	qg->ll.lam *= DEG_TO_RAD;
	qg->ll.phi *= DEG_TO_RAD;

	geoid->qg = qg;
	geoid->grid_offset = ftell ( fid );

	return 0;
}
/************************************************************************/
/*                       geoid_init_asc                                 */
/*                                                                      */
/*      Load a american asc geoid file (ASCII for .bin)                 */
/************************************************************************/
static int geoid_init_asc ( FILE *fid, QG_GRIDINFO *geoid )
{
	if ( debug_flag )
	{
		fprintf ( stderr, "geoid2geoid: init asc file not yet implemented  \n" );
	}
	return 1;
}

/************************************************************************/
/*                       geoid_init_bin                                 */
/*                                                                      */
/*      Load a bin geoid file (american format)                              */
/************************************************************************/
static int geoid_init_bin ( FILE *fid, QG_GRIDINFO *geoid )

{
	if ( debug_flag )
	{
		fprintf ( stderr, "geoid2geoid: init bin file not yet implemented  \n" );
	}
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
	qg->lim.lam = * ( ( int * ) ( header+36 ) ) ;
	qg->lim.phi = * ( ( int * ) ( header+32 ) )  ;


	qg->ll.lam *= DEG_TO_RAD;
	qg->ll.lam = adjlon ( qg->ll.lam );
	qg->ll.phi *= DEG_TO_RAD;
	qg->del.lam *= DEG_TO_RAD;
	qg->del.phi *= DEG_TO_RAD;
	qg->cvz = NULL;

	geoid->qg = qg;
	geoid->grid_offset = ftell ( fid );
	return 0;
}

/************************************************************************/
/*                       geoid_init_egm                                 */
/*                                                                      */
/*      Load a bin geoid file (american format)                          */
/************************************************************************/
static int geoid_init_egm ( FILE *fid, QG_GRIDINFO *geoid )

{
	struct QG_TABLE *qg;
	qg = ( struct QG_TABLE * ) pj_malloc ( sizeof ( struct QG_TABLE ) );
	strcpy ( qg->id, "egm Grid Geoid File" );

	qg->ll.lam = -179.25*DEG_TO_RAD;
	qg->ll.phi = -90.0*DEG_TO_RAD;
	qg->del.lam = 0.25*DEG_TO_RAD;
	qg->del.phi = 0.25*DEG_TO_RAD;
	qg->lim.lam = 1440 ;
	qg->lim.phi =721  ;

	geoid->qg = qg;
	geoid->grid_offset = ftell ( fid );
	return 0;
}


/************************************************************************/
/*                       geoid_init                                    */
/*                                                                      */
/*      Load a gra geoid file (IGN format)                              */
/************************************************************************/
int geoid_init ( const char *gridname, QG_GRIDINFO*   geoid )
{
	char 	fname[MAX_PATH_FILENAME+1];
	char	header[160];
	FILE *fp;

	geoid->gridname = strdup ( gridname );
	geoid->filename = NULL;
	geoid->grid_offset = 0;
	geoid->qg = NULL;
	geoid->next = NULL;
	/* -------------------------------------------------------------------- */
	/*      search extension, to check format	                        */
	/* -------------------------------------------------------------------- */
	if ( debug_flag )
	{
		int gridname_size=strlen ( gridname );
		char	extension[4];
		strncpy ( extension, gridname+ ( gridname_size-3 ),3 );
		extension[3]='\0';
		if ( strcmp ( extension,"gra" ) && strcmp ( extension,"GRA" )  && !strncmp ( geoid->format,"gra",3 ) )
		{
			fprintf ( stderr, "warning format type, extension is .%s and not %s \n",extension,geoid->format );
		}
		if ( strcmp ( extension,"bin" ) && strcmp ( extension,"BIN" ) && !strncmp ( geoid->format,"bin",3 ) )
			//if (strcmp ( extension,"bin" ) && strcmp ( extension,"BIN" ) && (geoid->format == "bin" ) )
		{
			fprintf ( stderr, "warning format type, extension is .%s and not %s \n",extension,geoid->format );
		}
		if ( strcmp ( extension,"asc" ) && strcmp ( extension,"ASC" ) && !strncmp ( geoid->format,"asc",3 ) )
		{
			fprintf ( stderr, "warning format type, extension is .%s and not %s \n",extension,geoid->format );
		}
		if ( strcmp ( extension,"mnt" ) && strcmp ( extension,"txt" ) && !strncmp ( geoid->format,"ign_txt",3 ) )
		{
			fprintf ( stderr, "warning format type, extension is .%s and not %s \n",extension,geoid->format );
		}
		if ( strcmp ( extension,"mnt" ) && strcmp ( extension,"txt" ) && !strncmp ( geoid->format,"ign_txt",3 ) )
		{
			fprintf ( stderr, "warning format type, extension is .%s and not %s \n",extension,geoid->format );
		}
		if ( strcmp ( extension,"dac" ) && strcmp ( extension,"DAC" ) && !strncmp ( geoid->format,"egm",3 ) )
		{
			fprintf ( stderr, "warning format type, extension is .%s and not %s \n",extension,geoid->format );
		}
	}
	/* -------------------------------------------------------------------- */
	/*      Open the file using the usual search rules.                     */
	/* -------------------------------------------------------------------- */
	strcpy ( fname, gridname );
	if ( !strcmp ( geoid->format,"bin" ) || !strcmp ( geoid->format,"egm" ) )
	{
		if ( ! ( fp = pj_open_lib ( fname, "rb" ) ) )
		{
			return 1;
		}
	}
	else if ( !strcmp ( geoid->format,"gra" ) || !strcmp ( geoid->format,"asc" ) || !strcmp ( geoid->format,"ign_txt" ) )
	{
		if ( ! ( fp = pj_open_lib ( fname, "rt" ) ) )
		{
			return 1;
		}
	}
	else
	{
		if ( debug_flag ) { fprintf ( stderr, "geoid format not supported :  \n" );}
		return 1;
	}
	geoid->filename = strdup ( fname );
	/* -------------------------------------------------------------------- */
	/*      Load a header, to determine the file type.                      */
	/* -------------------------------------------------------------------- */
	if ( !strcmp ( geoid->format,"gra" ) )
	{
		geoid_init_gra ( fp, geoid );
	}
	if ( !strcmp ( geoid->format,"ign_txt" ) )
	{
		geoid_init_mnt ( fp, geoid );
	}
// 	if ( !strcmp ( geoid->format,"asc" ) )
// 	{
// 		geoid_init_asc ( fp, geoid );
// 	}
	if ( !strcmp ( geoid->format,"bin" ) )
	{
		geoid_init_bin ( fp, geoid );
	}
	if ( !strcmp ( geoid->format,"egm" ) )
	{
		geoid_init_egm ( fp, geoid );
	}
	if ( debug_flag && geoid->qg !=NULL )
	{
		fprintf ( stderr, "geoid_table check :  \n" );
		fprintf ( stderr, "\t \t Lower left :\t %lf \t%lf  \n",geoid->qg->ll.lam*RAD_TO_DEG,geoid->qg->ll.phi*RAD_TO_DEG );
		fprintf ( stderr, "\t \t resolution :\t %lf \t%lf \n",geoid->qg->del.lam*RAD_TO_DEG,geoid->qg->del.phi*RAD_TO_DEG );
		fprintf ( stderr, "\t \t nb col row :\t %d \t%d \n",geoid->qg->lim.lam,geoid->qg->lim.phi );
	}
	fclose ( fp );
	return 0;
}




int main ( int argc, char **argv )
{

	debug_flag = getenv ( "PROJ_DEBUG" ) != NULL;
	if ( debug_flag )
	{
		fprintf ( stderr, " ********************************************** \n" );
		fprintf ( stderr, "  	GEOID GRID CONVERT			 \n" );
		fprintf ( stderr, " ********************************************** \n" );
	}
	QG_GRIDINFO   *qg_in;
	const char * format_out;
	qg_in = ( QG_GRIDINFO * ) pj_malloc ( sizeof ( QG_GRIDINFO ) );
	memset ( qg_in, 0, sizeof ( QG_GRIDINFO ) );
	char* arg,  **eargv= argv;
	int eargc = 0;

	if ( emess_dat.Prog_name = strrchr ( *argv,DIR_CHAR ) )
		++emess_dat.Prog_name;
	else emess_dat.Prog_name = *argv;
	if ( argc <= 1 )
	{
		( void ) fprintf ( stderr, usage, pj_get_release(), emess_dat.Prog_name );
		exit ( 0 );
	}

	/* -------------------------------------------------------------------- */
	/* process run line arguments */
	/* -------------------------------------------------------------------- */
	while ( --argc > 0 ) /* collect run line arguments */
	{
		if ( **++argv == '-' )
		{
			arg = *argv;
			switch ( *++arg )
			{
				case '\0': /* position of "stdin" */
					if ( arg[-1] == '-' ) eargv[eargc++] = "-";
					break;
				case 'i': /*input format tag*/
					arg += 2;
					qg_in->format= arg;
					if ( debug_flag ) { fprintf ( stderr, "input_format=%s \t",qg_in->format );}
					continue;
				case 'o': /* output format tag */
					arg += 2;
					format_out=arg;
					if ( debug_flag ) { fprintf ( stderr, "output_format=%s \t",format_out );}
					continue;
				default:
					emess ( 1, "invalid option: -%c",*arg );
					break;
			}
		}
		else /* assumed to be input file name(s) */
		{
			eargv[eargc++] = *argv;
			if ( debug_flag ) {  fprintf ( stderr, "file=%s \t", *argv );}
		}
	}
	if ( debug_flag ) {  fprintf ( stderr, "\n ******* file conversion processing *** \n" );}

	/* -------------------------------------------------------------------- */
	/*     processing loop                                                 */
	/* -------------------------------------------------------------------- */
	if ( eargc == 0 ) /* if no file input */
	{
		( void ) fprintf ( stderr, " no input file specified \n" );
		return 0;
	}
	else
	{
		int num_file=0;
		for ( num_file=0; num_file < eargc; num_file++ )
		{
			if ( debug_flag ) {  fprintf ( stderr, "convert %s \n", eargv[num_file] );}
			//initialisation of imput geoid
			if ( geoid_init ( eargv[num_file], qg_in ) )
			{
				if ( debug_flag ) {  fprintf ( stderr, "wrong initialisation of geoid \n" );}
				continue;
			}
			//load the data
			if ( geoid_load ( qg_in ) )
			{
				if ( debug_flag ) {  fprintf ( stderr, "error while loading geoid data\n" );}
				continue;
			}
			//save the table
			if ( geoid_save ( qg_in, format_out ) )
			{
				if ( debug_flag ) {  fprintf ( stderr, "error while saving geoid data\n" );}
				continue;
			}
			//free table and memory
			qg_free ( qg_in );
		}
	}
	return 0; /* normal completion */
}

