
/* pngread.c - read a PNG file
 *
 * Last changed in libpng 1.2.37 [June 4, 2009]
 * Copyright (c) 1998-2009 Glenn Randers-Pehrson
 * (Version 0.96 Copyright (c) 1996, 1997 Andreas Dilger)
 * (Version 0.88 Copyright (c) 1995, 1996 Guy Eric Schalnat, Group 42, Inc.)
 *
 * This code is released under the libpng license.
 * For conditions of distribution and use, see the disclaimer
 * and license in png.h
 *
 * This file contains routines that an application calls directly to
 * read a PNG file or stream.
 */

#define PNG_INTERNAL
#include "png.h"
#if defined(PNG_READ_SUPPORTED)

/* Create a PNG structure for reading, and allocate any memory needed. */
png_structp PNGAPI
png_create_read_struct(png_const_charp user_png_ver, png_voidp error_ptr,
   png_error_ptr error_fn, png_error_ptr warn_fn)
{

#ifdef PNG_USER_MEM_SUPPORTED
   return (png_create_read_struct_2(user_png_ver, error_ptr, error_fn,
      warn_fn, png_voidp_NULL, png_malloc_ptr_NULL, png_free_ptr_NULL));
}

/* Alternate create PNG structure for reading, and allocate any memory needed. */
png_structp PNGAPI
png_create_read_struct_2(png_const_charp user_png_ver, png_voidp error_ptr,
   png_error_ptr error_fn, png_error_ptr warn_fn, png_voidp mem_ptr,
   png_malloc_ptr malloc_fn, png_free_ptr free_fn)
{
#endif /* PNG_USER_MEM_SUPPORTED */

#ifdef PNG_SETJMP_SUPPORTED
   volatile
#endif
   png_structp png_ptr;

#ifdef PNG_SETJMP_SUPPORTED
#ifdef USE_FAR_KEYWORD
   jmp_buf jmpbuf;
#endif
#endif

   int i;

   png_debug(1, "in png_create_read_struct");
#ifdef PNG_USER_MEM_SUPPORTED
   png_ptr = (png_structp)png_create_struct_2(PNG_STRUCT_PNG,
      (png_malloc_ptr)malloc_fn, (png_voidp)mem_ptr);
#else
   png_ptr = (png_structp)png_create_struct(PNG_STRUCT_PNG);
#endif
   if (png_ptr == NULL)
      return (NULL);

   /* Added at libpng-1.2.6 */
#ifdef PNG_SET_USER_LIMITS_SUPPORTED
   png_ptr->user_width_max=PNG_USER_WIDTH_MAX;
   png_ptr->user_height_max=PNG_USER_HEIGHT_MAX;
#endif

#ifdef PNG_SETJMP_SUPPORTED
#ifdef USE_FAR_KEYWORD
   if (setjmp(jmpbuf))
#else
   if (setjmp(png_ptr->jmpbuf))
#endif
   {
      png_free(png_ptr, png_ptr->zbuf);
      png_ptr->zbuf = NULL;
#ifdef PNG_USER_MEM_SUPPORTED
      png_destroy_struct_2((png_voidp)png_ptr,
         (png_free_ptr)free_fn, (png_voidp)mem_ptr);
#else
      png_destroy_struct((png_voidp)png_ptr);
#endif
      return (NULL);
   }
#ifdef USE_FAR_KEYWORD
   png_memcpy(png_ptr->jmpbuf, jmpbuf, png_sizeof(jmp_buf));
#endif
#endif

#ifdef PNG_USER_MEM_SUPPORTED
   png_set_mem_fn(png_ptr, mem_ptr, malloc_fn, free_fn);
#endif

   png_set_error_fn(png_ptr, error_ptr, error_fn, warn_fn);

   if (user_png_ver)
   {
     i = 0;
     do
     {
       if (user_png_ver[i] != png_libpng_ver[i])
          png_ptr->flags |= PNG_FLAG_LIBRARY_MISMATCH;
     } while (png_libpng_ver[i++]);
   }
   else
        png_ptr->flags |= PNG_FLAG_LIBRARY_MISMATCH;


   if (png_ptr->flags & PNG_FLAG_LIBRARY_MISMATCH)
   {
     /* Libpng 0.90 and later are binary incompatible with libpng 0.89, so
      * we must recompile any applications that use any older library version.
      * For versions after libpng 1.0, we will be compatible, so we need
      * only check the first digit.
      */
     if (user_png_ver == NULL || user_png_ver[0] != png_libpng_ver[0] ||
         (user_png_ver[0] == '1' && user_png_ver[2] != png_libpng_ver[2]) ||
         (user_png_ver[0] == '0' && user_png_ver[2] < '9'))
     {
#if !defined(PNG_NO_STDIO) && !defined(_WIN32_WCE)
        char msg[80];
        if (user_png_ver)
        {
          png_snprintf(msg, 80,
             "Application was compiled with png.h from libpng-%.20s",
             user_png_ver);
          png_warning(png_ptr, msg);
        }
        png_snprintf(msg, 80,
             "Application  is  running with png.c from libpng-%.20s",
           png_libpng_ver);
        png_warning(png_ptr, msg);
#endif
#ifdef PNG_ERROR_NUMBERS_SUPPORTED
        png_ptr->flags = 0;
#endif
        png_error(png_ptr,
           "Incompatible libpng version in application and library");
     }
   }

   /* Initialize zbuf - compression buffer */
   png_ptr->zbuf_size = PNG_ZBUF_SIZE;
   png_ptr->zbuf = (png_bytep)png_malloc(png_ptr,
     (png_uint_32)png_ptr->zbuf_size);
   png_ptr->zstream.zalloc = png_zalloc;
   png_ptr->zstream.zfree = png_zfree;
   png_ptr->zstream.opaque = (voidpf)png_ptr;

   switch (inflateInit(&png_ptr->zstream))
   {
     case Z_OK: /* Do nothing */ break;
     case Z_MEM_ERROR:
     case Z_STREAM_ERROR: png_error(png_ptr, "zlib memory error"); break;
     case Z_VERSION_ERROR: png_error(png_ptr, "zlib version error"); break;
     default: png_error(png_ptr, "Unknown zlib error");
   }

   png_ptr->zstream.next_out = png_ptr->zbuf;
   png_ptr->zstream.avail_out = (uInt)png_ptr->zbuf_size;

   png_set_read_fn(png_ptr, png_voidp_NULL, png_rw_ptr_NULL);

#ifdef PNG_SETJMP_SUPPORTED
/* Applications that neglect to set up their own setjmp() and then encounter
   a png_error() will longjmp here.  Since the jmpbuf is then meaningless we
   abort instead of returning. */
#ifdef USE_FAR_KEYWORD
   if (setjmp(jmpbuf))
      PNG_ABORT();
   png_memcpy(png_ptr->jmpbuf, jmpbuf, png_sizeof(jmp_buf));
#else
   if (setjmp(png_ptr->jmpbuf))
      PNG_ABORT();
#endif
#endif
   return (png_ptr);
}

#if defined(PNG_1_0_X) || defined(PNG_1_2_X)
/* Initialize PNG structure for reading, and allocate any memory needed.
   This interface is deprecated in favour of the png_create_read_struct(),
   and it will disappear as of libpng-1.3.0. */
#undef png_read_init
void PNGAPI
png_read_init(png_structp png_ptr)
{
   /* We only come here via pre-1.0.7-compiled applications */
   png_read_init_2(png_ptr, "1.0.6 or earlier", 0, 0);
}

void PNGAPI
png_read_init_2(png_structp png_ptr, png_const_charp user_png_ver,
   png_size_t png_struct_size, png_size_t png_info_size)
{
   /* We only come here via pre-1.0.12-compiled applications */
   if (png_ptr == NULL)
      return;
#if !defined(PNG_NO_STDIO) && !defined(_WIN32_WCE)
   if (png_sizeof(png_struct) > png_struct_size ||
      png_sizeof(png_info) > png_info_size)
   {
      char msg[80];
      png_ptr->warning_fn = NULL;
      if (user_png_ver)
      {
        png_snprintf(msg, 80,
           "Application was compiled with png.h from libpng-%.20s",
           user_png_ver);
        png_warning(png_ptr, msg);
      }
      png_snprintf(msg, 80,
         "Application  is  running with png.c from libpng-%.20s",
         png_libpng_ver);
      png_warning(png_ptr, msg);
   }
#endif
   if (png_sizeof(png_struct) > png_struct_size)
     {
       png_ptr->error_fn = NULL;
#ifdef PNG_ERROR_NUMBERS_SUPPORTED
       png_ptr->flags = 0;
#endif
       png_error(png_ptr,
       "The png struct allocated by the application for reading is too small.");
     }
   if (png_sizeof(png_info) > png_info_size)
     {
       png_ptr->error_fn = NULL;
#ifdef PNG_ERROR_NUMBERS_SUPPORTED
       png_ptr->flags = 0;
#endif
       png_error(png_ptr,
         "The info struct allocated by application for reading is too small.");
     }
   png_read_init_3(&png_ptr, user_png_ver, png_struct_size);
}
#endif /* PNG_1_0_X || PNG_1_2_X */

void PNGAPI
png_read_init_3(png_structpp ptr_ptr, png_const_charp user_png_ver,
   png_size_t png_struct_size)
{
#ifdef PNG_SETJMP_SUPPORTED
   jmp_buf tmp_jmp;  /* to save current jump buffer */
#endif

   int i = 0;

   png_structp png_ptr=*ptr_ptr;

   if (png_ptr == NULL)
      return;

   do
   {
     if (user_png_ver[i] != png_libpng_ver[i])
     {
#ifdef PNG_LEGACY_SUPPORTED
       png_ptr->flags |= PNG_FLAG_LIBRARY_MISMATCH;
#else
       png_ptr->warning_fn = NULL;
       png_warning(png_ptr,
        "Application uses deprecated png_read_init() and should be recompiled.");
       break;
#endif
     }
   } while (png_libpng_ver[i++]);

   png_debug(1, "in png_read_init_3");

#ifdef PNG_SETJMP_SUPPORTED
   /* Save jump buffer and error functions */
   png_memcpy(tmp_jmp, png_ptr->jmpbuf, png_sizeof(jmp_buf));
#endif

   if (png_sizeof(png_struct) > png_struct_size)
   {
      png_destroy_struct(png_ptr);
      *ptr_ptr = (png_structp)png_create_struct(PNG_STRUCT_PNG);
      png_ptr = *ptr_ptr;
   }

   /* Reset all variables to 0 */
   png_memset(png_ptr, 0, png_sizeof(png_struct));

#ifdef PNG_SETJMP_SUPPORTED
   /* Restore jump buffer */
   png_memcpy(png_ptr->jmpbuf, tmp_jmp, png_sizeof(jmp_buf));
#endif

   /* Added at libpng-1.2.6 */
#ifdef PNG_SET_USER_LIMITS_SUPPORTED
   png_ptr->user_width_max=PNG_USER_WIDTH_MAX;
   png_ptr->user_height_max=PNG_USER_HEIGHT_MAX;
#endif

   /* Initialize zbuf - compression buffer */
   png_ptr->zbuf_size = PNG_ZBUF_SIZE;
   png_ptr->zbuf = (png_bytep)png_malloc(png_ptr,
     (png_uint_32)png_ptr->zbuf_size);
   png_ptr->zstream.zalloc = png_zalloc;
   png_ptr->zstream.zfree = png_zfree;
   png_ptr->zstream.opaque = (voidpf)png_ptr;

   switch (inflateInit(&png_ptr->zstream))
   {
     case Z_OK: /* Do nothing */ break;
     case Z_MEM_ERROR:
     case Z_STREAM_ERROR: png_error(png_ptr, "zlib memory"); break;
     case Z_VERSION_ERROR: png_error(png_ptr, "zlib version"); break;
     default: png_error(png_ptr, "Unknown zlib error");
   }

   png_ptr->zstream.next_out = png_ptr->zbuf;
   png_ptr->zstream.avail_out = (uInt)png_ptr->zbuf_size;

   png_set_read_fn(png_ptr, png_voidp_NULL, png_rw_ptr_NULL);
}

#ifndef PNG_NO_SEQUENTIAL_READ_SUPPORTED
/* Read the information before the actual image data.  This has been
 * changed in v0.90 to allow reading a file that already has the magic
 * bytes read from the stream.  You can tell libpng how many bytes have
 * been read from the beginning of the stream (up to the maximum of 8)
 * via png_set_sig_bytes(), and we will only check the remaining bytes
 * here.  The application can then have access to the signature bytes we
 * read if it is determined that this isn't a valid PNG file.
 */
void PNGAPI
png_read_info(png_structp png_ptr, png_infop info_ptr)
{
   if (png_ptr == NULL || info_ptr == NULL)
      return;
   png_debug(1, "in png_read_info");
   /* If we haven't checked all of the PNG signature bytes, do so now. */
   if (png_ptr->sig_bytes < 8)
   {
      png_size_t num_checked = png_ptr->sig_bytes,
                 num_to_check = 8 - num_checked;

      png_read_data(png_ptr, &(info_ptr->signature[num_checked]), num_to_check);
      png_ptr->sig_bytes = 8;

      if (png_sig_cmp(info_ptr->signature, num_checked, num_to_check))
      {
         if (num_checked < 4 &&
             png_sig_cmp(info_ptr->signature, num_checked, num_to_check - 4))
            png_error(png_ptr, "Not a PNG file");
         else
            png_error(png_ptr, "PNG file corrupted by ASCII conversion");
      }
      if (num_checked < 3)
         png_ptr->mode |= PNG_HAVE_PNG_SIGNATURE;
   }

   for (;;)
   {
#ifdef PNG_USE_LOCAL_ARRAYS
      PNG_CONST PNG_IHDR;
      PNG_CONST PNG_IDAT;
      PNG_CONST PNG_IEND;
      PNG_CONST PNG_PLTE;
#if defined(PNG_READ_bKGD_SUPPORTED)
      PNG_CONST PNG_bKGD;
#endif
#if defined(PNG_READ_cHRM_SUPPORTED)
      PNG_CONST PNG_cHRM;
#endif
#if defined(PNG_READ_gAMA_SUPPORTED)
      PNG_CONST PNG_gAMA;
#endif
#if defined(PNG_READ_hIST_SUPPORTED)
      PNG_CONST PNG_hIST;
#endif
#if defined(PNG_READ_iCCP_SUPPORTED)
      PNG_CONST PNG_iCCP;
#endif
#if defined(PNG_READ_iTXt_SUPPORTED)
      PNG_CONST PNG_iTXt;
#endif
#if defined(PNG_READ_oFFs_SUPPORTED)
      PNG_CONST PNG_oFFs;
#endif
#if defined(PNG_READ_pCAL_SUPPORTED)
      PNG_CONST PNG_pCAL;
#endif
#if defined(PNG_READ_pHYs_SUPPORTED)
      PNG_CONST PNG_pHYs;
#endif
#if defined(PNG_READ_sBIT_SUPPORTED)
      PNG_CONST PNG_sBIT;
#endif
#if defined(PNG_READ_sCAL_SUPPORTED)
      PNG_CONST PNG_sCAL;
#endif
#if defined(PNG_READ_sPLT_SUPPORTED)
      PNG_CONST PNG_sPLT;
#endif
#if defined(PNG_READ_sRGB_SUPPORTED)
      PNG_CONST PNG_sRGB;
#endif
#if defined(PNG_READ_tEXt_SUPPORTED)
      PNG_CONST PNG_tEXt;
#endif
#if defined(PNG_READ_tIME_SUPPORTED)
      PNG_CONST PNG_tIME;
#endif
#if defined(PNG_READ_tRNS_SUPPORTED)
      PNG_CONST PNG_tRNS;
#endif
#if defined(PNG_READ_zTXt_SUPPORTED)
      PNG_CONST PNG_zTXt;
#endif
#endif /* PNG_USE_LOCAL_ARRAYS */
      png_uint_32 length = png_read_chunk_header(png_ptr);
      PNG_CONST png_bytep chunk_name = png_ptr->chunk_name;

      /* This should be a binary subdivision search or a hash for
       * matching the chunk name rather than a linear search.
       */
      if (!png_memcmp(chunk_name, png_IDAT, 4))
        if (png_ptr->mode & PNG_AFTER_IDAT)
          png_ptr->mode |= PNG_HAVE_CHUNK_AFTER_IDAT;

      if (!png_memcmp(chunk_name, png_IHDR, 4))
         png_handle_IHDR(png_ptr, info_ptr, length);
      else if (!png_memcmp(chunk_name, png_IEND, 4))
         png_handle_IEND(png_ptr, info_ptr, length);
#ifdef PNG_HANDLE_AS_UNKNOWN_SUPPORTED
      else if (png_handle_as_unknown(png_ptr, chunk_name))
      {
         if (!png_memcmp(chunk_name, png_IDAT, 4))
            png_ptr->mode |= PNG_HAVE_IDAT;
         png_handle_unknown(png_ptr, info_ptr, length);
         if (!png_memcmp(chunk_name, png_PLTE, 4))
            png_ptr->mode |= PNG_HAVE_PLTE;
         else if (!png_memcmp(chunk_name, png_IDAT, 4))
         {
            if (!(png_ptr->mode & PNG_HAVE_IHDR))
               png_error(png_ptr, "Missing IHDR before IDAT");
            else if (png_ptr->color_type == PNG_COLOR_TYPE_PALETTE &&
                     !(png_ptr->mode & PNG_HAVE_PLTE))
               png_error(png_ptr, "Missing PLTE before IDAT");
            break;
         }
      }
#endif
      else if (!png_memcmp(chunk_name, png_PLTE, 4))
         png_handle_PLTE(png_ptr, info_ptr, length);
      else if (!png_memcmp(chunk_name, png_IDAT, 4))
      {
         if (!(png_ptr->mode & PNG_HAVE_IHDR))
            png_error(png_ptr, "Missing IHDR before IDAT");
         else if (png_ptr->color_type == PNG_COLOR_TYPE_PALETTE &&
                  !(png_ptr->mode & PNG_HAVE_PLTE))
            png_error(png_ptr, "Missing PLTE before IDAT");

         png_ptr->idat_size = length;
         png_ptr->mode |= PNG_HAVE_IDAT;
         break;
      }
#if defined(PNG_READ_bKGD_SUPPORTED)
      else if (!png_memcmp(chunk_name, png_bKGD, 4))
         png_handle_bKGD(png_ptr, info_ptr, length);
#endif
#if defined(PNG_READ_cHRM_SUPPORTED)
      else if (!png_memcmp(chunk_name, png_cHRM, 4))
         png_handle_cHRM(png_ptr, info_ptr, length);
#endif
#if defined(PNG_READ_gAMA_SUPPORTED)
      else if (!png_memcmp(chunk_name, png_gAMA, 4))
         png_handle_gAMA(png_ptr, info_ptr, length);
#endif
#if defined(PNG_READ_hIST_SUPPORTED)
      else if (!png_memcmp(chunk_name, png_hIST, 4))
         png_handle_hIST(png_ptr, info_ptr, length);
#endif
#if defined(PNG_READ_oFFs_SUPPORTED)
      else if (!png_memcmp(chunk_name, png_oFFs, 4))
         png_handle_oFFs(png_ptr, info_ptr, length);
#endif
#if defined(PNG_READ_pCAL_SUPPORTED)
      else if (!png_memcmp(chunk_name, png_pCAL, 4))
         png_handle_pCAL(png_ptr, info_ptr, length);
#endif
#if defined(PNG_READ_sCAL_SUPPORTED)
      else if (!png_memcmp(chunk_name, png_sCAL, 4))
         png_handle_sCAL(png_ptr, info_ptr, length);
#endif
#if defined(PNG_READ_pHYs_SUPPORTED)
      else if (!png_memcmp(chunk_name, png_pHYs, 4))
         png_handle_pHYs(png_ptr, info_ptr, length);
#endif
#if defined(PNG_READ_sBIT_SUPPORTED)
      else if (!png_memcmp(chunk_name, png_sBIT, 4))
         png_handle_sBIT(png_ptr, info_ptr, length);
#endif
#if defined(PNG_READ_sRGB_SUPPORTED)
      else if (!png_memcmp(chunk_name, png_sRGB, 4))
         png_handle_sRGB(png_ptr, info_ptr, length);
#endif
#if defined(PNG_READ_iCCP_SUPPORTED)
      else if (!png_memcmp(chunk_name, png_iCCP, 4))
         png_handle_iCCP(png_ptr, info_ptr, length);
#endif
#if defined(PNG_READ_sPLT_SUPPORTED)
      else if (!png_memcmp(chunk_name, png_sPLT, 4))
         png_handle_sPLT(png_ptr, info_ptr, length);
#endif
#if defined(PNG_READ_tEXt_SUPPORTED)
      else if (!png_memcmp(chunk_name, png_tEXt, 4))
         png_handle_tEXt(png_ptr, info_ptr, length);
#endif
#if defined(PNG_READ_tIME_SUPPORTED)
      else if (!png_memcmp(chunk_name, png_tIME, 4))
         png_handle_tIME(png_ptr, info_ptr, length);
#endif
#if defined(PNG_READ_tRNS_SUPPORTED)
      else if (!png_memcmp(chunk_name, png_tRNS, 4))
         png_handle_tRNS(png_ptr, info_ptr, length);
#endif
#if defined(PNG_READ_zTXt_SUPPORTED)
      else if (!png_memcmp(chunk_name, png_zTXt, 4))
         png_handle_zTXt(png_ptr, info_ptr, length);
#endif
#if defined(PNG_READ_iTXt_SUPPORTED)
      else if (!png_memcmp(chunk_name, png_iTXt, 4))
         png_handle_iTXt(png_ptr, info_ptr, length);
#endif
      else
         png_handle_unknown(png_ptr, info_ptr, length);
   }
}
#endif /* PNG_NO_SEQUENTIAL_READ_SUPPORTED */

/* Optional call to update the users info_ptr structure */
void PNGAPI
png_read_update_info(png_structp png_ptr, png_infop info_ptr)
{
   png_debug(1, "in png_read_update_info");
   if (png_ptr == NULL)
      return;
   if (!(png_ptr->flags & PNG_FLAG_ROW_INIT))
      png_read_start_row(png_ptr);
   else
      png_warning(png_ptr,
      "Ignoring extra png_read_update_info() call; row buffer not reallocated");
   png_read_transform_info(png_ptr, info_ptr);
}

#ifndef PNG_NO_SEQUENTIAL_READ_SUPPORTED
/* Initialize palette, background, etc, after transformations
 * are set, but before any reading takes place.  This allows
 * the user to obtain a gamma-corrected palette, for example.
 * If the user doesn't call this, we will do it ourselves.
 */
void PNGAPI
png_start_read_image(png_structp png_ptr)
{
   png_debug(1, "in png_start_read_image");
   if (png_ptr == NULL)
      return;
   if (!(png_ptr->flags & PNG_FLAG_ROW_INIT))
      png_read_start_row(png_ptr);
}
#endif /* PNG_NO_SEQUENTIAL_READ_SUPPORTED */

#ifndef PNG_NO_SEQUENTIAL_READ_SUPPORTED
void PNGAPI
png_read_row(png_structp png_ptr, png_bytep row, png_bytep dsp_row)
{
#ifdef PNG_USE_LOCAL_ARRAYS
   PNG_CONST PNG_IDAT;
   PNG_CONST int png_pass_dsp_mask[7] = {0xff, 0x0f, 0xff, 0x33, 0xff, 0x55,
      0xff};
   PNG_CONST int png_pass_mask[7] = {0x80, 0x08, 0x88, 0x22, 0xaa, 0x55, 0xff};
#endif
   int ret;
   if (png_ptr == NULL)
      return;
   png_debug2(1, "in png_read_row (row %lu, pass %d)",
      png_ptr->row_number, png_ptr->pass);
   if (!(png_ptr->flags & PNG_FLAG_ROW_INIT))
      png_read_start_row(png_ptr);
   if (png_ptr->row_number == 0 && png_ptr->pass == 0)
   {
   /* Check for transforms that have been set but were defined out */
#if defined(PNG_WRITE_INVERT_SUPPORTED) && !defined(PNG_READ_INVERT_SUPPORTED)
   if (png_ptr->transformations & PNG_INVERT_MONO)
      png_warning(png_ptr, "PNG_READ_INVERT_SUPPORTED is not defined.");
#endif
#if defined(PNG_WRITE_FILLER_SUPPORTED) && !defined(PNG_READ_FILLER_SUPPORTED)
   if (png_ptr->transformations & PNG_FILLER)
      png_warning(png_ptr, "PNG_READ_FILLER_SUPPORTED is not defined.");
#endif
#if defined(PNG_WRITE_PACKSWAP_SUPPORTED) && !defined(PNG_READ_PACKSWAP_SUPPORTED)
   if (png_ptr->transformations & PNG_PACKSWAP)
      png_warning(png_ptr, "PNG_READ_PACKSWAP_SUPPORTED is not defined.");
#endif
#if defined(PNG_WRITE_PACK_SUPPORTED) && !defined(PNG_READ_PACK_SUPPORTED)
   if (png_ptr->transformations & PNG_PACK)
      png_warning(png_ptr, "PNG_READ_PACK_SUPPORTED is not defined.");
#endif
#if defined(PNG_WRITE_SHIFT_SUPPORTED) && !defined(PNG_READ_SHIFT_SUPPORTED)
   if (png_ptr->transformations & PNG_SHIFT)
      png_warning(png_ptr, "PNG_READ_SHIFT_SUPPORTED is not defined.");
#endif
#if defined(PNG_WRITE_BGR_SUPPORTED) && !defined(PNG_READ_BGR_SUPPORTED)
   if (png_ptr->transformations & PNG_BGR)
      png_warning(png_ptr, "PNG_READ_BGR_SUPPORTED is not defined.");
#endif
#if defined(PNG_WRITE_SWAP_SUPPORTED) && !defined(PNG_READ_SWAP_SUPPORTED)
   if (png_ptr->transformations & PNG_SWAP_BYTES)
      png_warning(png_ptr, "PNG_READ_SWAP_SUPPORTED is not defined.");
#endif
   }

#if defined(PNG_READ_INTERLACING_SUPPORTED)
   /* If interlaced and we do not need a new row, combine row and return */
   if (png_ptr->interlaced && (png_ptr->transformations & PNG_INTERLACE))
   {
      switch (png_ptr->pass)
      {
         case 0:
            if (png_ptr->row_number & 0x07)
            {
               if (dsp_row != NULL)
                  png_combine_row(png_ptr, dsp_row,
                     png_pass_dsp_mask[png_ptr->pass]);
               png_read_finish_row(png_ptr);
               return;
            }
            break;
         case 1:
            if ((png_ptr->row_number & 0x07) || png_ptr->width < 5)
            {
               if (dsp_row != NULL)
                  png_combine_row(png_ptr, dsp_row,
                     png_pass_dsp_mask[png_ptr->pass]);
               png_read_finish_row(png_ptr);
               return;
            }
            break;
         case 2:
            if ((png_ptr->row_number & 0x07) != 4)
            {
               if (dsp_row != NULL && (png_ptr->row_number & 4))
                  png_combine_row(png_ptr, dsp_row,
                     png_pass_dsp_mask[png_ptr->pass]);
               png_read_finish_row(png_ptr);
               return;
            }
            break;
         case 3:
            if ((png_ptr->row_number & 3) || png_ptr->width < 3)
            {
               if (dsp_row != NULL)
                  png_combine_row(png_ptr, dsp_row,
                     png_pass_dsp_mask[png_ptr->pass]);
               png_read_finish_row(png_ptr);
               return;
            }
            break;
         case 4:
            if ((png_ptr->row_number & 3) != 2)
            {
               if (dsp_row != NULL && (png_ptr->row_number & 2))
                  png_combine_row(png_ptr, dsp_row,
                     png_pass_dsp_mask[png_ptr->pass]);
               png_read_finish_row(png_ptr);
               return;
            }
            break;
         case 5:
            if ((png_ptr->row_number & 1) || png_ptr->width < 2)
            {
               if (dsp_row != NULL)
                  png_combine_row(png_ptr, dsp_row,
                     png_pass_dsp_mask[png_ptr->pass]);
               png_read_finish_row(png_ptr);
               return;
            }
            break;
         case 6:
            if (!(png_ptr->row_number & 1))
            {
               png_read_finish_row(png_ptr);
               return;
            }
            break;
      }
   }
#endif

   if (!(png_ptr->mode & PNG_HAVE_IDAT))
      png_error(png_ptr, "Invalid attempt to read row data");

   png_ptr->zstream.next_out = png_ptr->row_buf;
   png_ptr->zstream.avail_out = (uInt)png_ptr->irowbytes;
   do
   {
      if (!(png_ptr->zstream.avail_in))
      {
         while (!png_ptr->idat_size)
         {
            png_crc_finish(png_ptr, 0);

            png_ptr->idat_size = png_read_chunk_header(png_ptr);
            if (png_memcmp(png_ptr->chunk_name, png_IDAT, 4))
               png_error(png_ptr, "Not enough image data");
         }
         png_ptr->zstream.avail_in = (uInt)png_ptr->zbuf_size;
         png_ptr->zstream.next_in = png_ptr->zbuf;
         if (png_ptr->zbuf_size > png_ptr->idat_size)
            png_ptr->zstream.avail_in = (uInt)png_ptr->idat_size;
         png_crc_read(png_ptr, png_ptr->zbuf,
            (png_size_t)png_ptr->zstream.avail_in);
         png_ptr->idat_size -= png_ptr->zstream.avail_in;
      }
      ret = inflate(&png_ptr->zstream, Z_PARTIAL_FLUSH);
      if (ret == Z_STREAM_END)
      {
         if (png_ptr->zstream.avail_out || png_ptr->zstream.avail_in ||
            png_ptr->idat_size)
            png_error(png_ptr, "Extra compressed data");
         png_ptr->mode |= PNG_AFTER_IDAT;
         png_ptr->flags |= PNG_FLAG_ZLIB_FINISHED;
         break;
      }
      if (ret != Z_OK)
         png_error(png_ptr, png_ptr->zstream.msg ? png_ptr->zstream.msg :
                   "Decompression error");

   } while (png_ptr->zstream.avail_out);

   png_ptr->row_info.color_type = png_ptr->color_type;
   png_ptr->row_info.width = png_ptr->iwidth;
   png_ptr->row_info.channels = png_ptr->channels;
   png_ptr->row_info.bit_depth = png_ptr->bit_depth;
   png_ptr->row_info.pixel_depth = png_ptr->pixel_depth;
   png_ptr->row_info.rowbytes = PNG_ROWBYTES(png_ptr->row_info.pixel_depth,
       png_ptr->row_info.width);

   if (png_ptr->row_buf[0])
   png_read_filter_row(png_ptr, &(png_ptr->row_info),
      png_ptr->row_buf + 1, png_ptr->prev_row + 1,
      (int)(png_ptr->row_buf[0]));

   png_memcpy_check(png_ptr, png_ptr->prev_row, png_ptr->row_buf,
      png_ptr->rowbytes + 1);

#if defined(PNG_MNG_FEATURES_SUPPORTED)
   if ((png_ptr->mng_features_permitted & PNG_FLAG_MNG_FILTER_64) &&
      (png_ptr->filter_type == PNG_INTRAPIXEL_DIFFERENCING))
   {
      /* Intrapixel differencing */
      png_do_read_intrapixel(&(png_ptr->row_info), png_ptr->row_buf + 1);
   }
#endif


   if (png_ptr->transformations || (png_ptr->flags&PNG_FLAG_STRIP_ALPHA))
      png_do_read_transformations(png_ptr);

#if defined(PNG_READ_INTERLACING_SUPPORTED)
   /* Blow up interlaced rows to full size */
   if (png_ptr->interlaced &&
      (png_ptr->transformations & PNG_INTERLACE))
   {
      if (png_ptr->pass < 6)
         /* Old interface (pre-1.0.9):
          * png_do_read_interlace(&(png_ptr->row_info),
          *    png_ptr->row_buf + 1, png_ptr->pass, png_ptr->transformations);
          */
         png_do_read_interlace(png_ptr);

      if (dsp_row != NULL)
         png_combine_row(png_ptr, dsp_row,
            png_pass_dsp_mask[png_ptr->pass]);
      if (row != NULL)
         png_combine_row(png_ptr, row,
            png_pass_mask[png_ptr->pass]);
   }
   else
#endif
   {
      if (row != NULL)
         png_combine_row(png_ptr, row, 0xff);
      if (dsp_row != NULL)
         png_combine_row(png_ptr, dsp_row, 0xff);
   }
   png_read_finish_row(png_ptr);

   if (png_ptr->read_row_fn != NULL)
      (*(png_ptr->read_row_fn))(png_ptr, png_ptr->row_number, png_ptr->pass);
}
#endif /* PNG_NO_SEQUENTIAL_READ_SUPPORTED */

#ifndef PNG_NO_SEQUENTIAL_READ_SUPPORTED
/* Read one or more rows of image data.  If the image is interlaced,
 * and png_set_interlace_handling() has been called, the rows need to
 * contain the contents of the rows from the previous pass.  If the
 * image has alpha or transparency, and png_handle_alpha()[*] has been
 * called, the rows contents must be initialized to the contents of the
 * screen.
 *
 * "row" holds the actual image, and pixels are placed in it
 * as they arrive.  If the image is displayed after each pass, it will
 * appear to "sparkle" in.  "display_row" can be used to display a
 * "chunky" progressive image, with finer detail added as it becomes
 * available.  If you do not want this "chunky" display, you may pass
 * NULL for display_row.  If you do not want the sparkle display, and
 * you have not called png_handle_alpha(), you may pass NULL for rows.
 * If you have called png_handle_alpha(), and the image has either an
 * alpha channel or a transparency chunk, you must provide a buffer for
 * rows.  In this case, you do not have to provide a display_row buffer
 * also, but you may.  If the image is not interlaced, or if you have
 * not called png_set_interlace_handling(), the display_row buffer will
 * be ignored, so pass NULL to it.
 *
 * [*] png_handle_alpha() does not exist yet, as of this version of libpng
 */

void PNGAPI
png_read_rows(png_structp png_ptr, png_bytepp row,
   png_bytepp display_row, png_uint_32 num_rows)
{
   png_uint_32 i;
   png_bytepp rp;
   png_bytepp dp;

   png_debug(1, "in png_read_rows");
   if (png_ptr == NULL)
      return;
   rp = row;
   dp = display_row;
   if (rp != NULL && dp != NULL)
      for (i = 0; i < num_rows; i++)
      {
         png_bytep rptr = *rp++;
         png_bytep dptr = *dp++;

         png_read_row(png_ptr, rptr, dptr);
      }
   else if (rp != NULL)
      for (i = 0; i < num_rows; i++)
      {
         png_bytep rptr = *rp;
         png_read_row(png_ptr, rptr, png_bytep_NULL);
         rp++;
      }
   else if (dp != NULL)
      for (i = 0; i < num_rows; i++)
      {
         png_bytep dptr = *dp;
         png_read_row(png_ptr, png_bytep_NULL, dptr);
         dp++;
      }
}
#endif /* PNG_NO_SEQUENTIAL_READ_SUPPORTED */

#ifndef PNG_NO_SEQUENTIAL_READ_SUPPORTED
/* Read the entire image.  If the image has an alpha channel or a tRNS
 * chunk, and you have called png_handle_alpha()[*], you will need to
 * initialize the image to the current image that PNG will be overlaying.
 * We set the num_rows again here, in case it was incorrectly set in
 * png_read_start_row() by a call to png_read_update_info() or
 * png_start_read_image() if png_set_interlace_handling() wasn't called
 * prior to either of these functions like it should have been.  You can
 * only call this function once.  If you desire to have an image for
 * each pass of a interlaced image, use png_read_rows() instead.
 *
 * [*] png_handle_alpha() does not exist yet, as of this version of libpng
 */
void PNGAPI
png_read_image(png_structp png_ptr, png_bytepp image)
{
   png_uint_32 i, image_height;
   int pass, j;
   png_bytepp rp;

   png_debug(1, "in png_read_image");
   if (png_ptr == NULL)
      return;

#ifdef PNG_READ_INTERLACING_SUPPORTED
   pass = png_set_interlace_handling(png_ptr);
#else
   if (png_ptr->interlaced)
      png_error(png_ptr,
        "Cannot read interlaced image -- interlace handler disabled.");
   pass = 1;
#endif


   image_height=png_ptr->height;
   png_ptr->num_rows = image_height; /* Make sure this is set correctly */

   for (j = 0; j < pass; j++)
   {
      rp = image;
      for (i = 0; i < image_height; i++)
      {
         png_read_row(png_ptr, *rp, png_bytep_NULL);
         rp++;
      }
   }
}
#endif /* PNG_NO_SEQUENTIAL_READ_SUPPORTED */

#ifndef PNG_NO_SEQUENTIAL_READ_SUPPORTED
/* Read the end of the PNG file.  Will not read past the end of the
 * file, will verify the end is accurate, and will read any comments
 * or time information at the end of the file, if info is not NULL.
 */
void PNGAPI
png_read_end(png_structp png_ptr, png_infop info_ptr)
{
   png_debug(1, "in png_read_end");
   if (png_ptr == NULL)
      return;
   png_crc_finish(png_ptr, 0); /* Finish off CRC from last IDAT chunk */

   do
   {
#ifdef PNG_USE_LOCAL_ARRAYS
      PNG_CONST PNG_IHDR;
      PNG_CONST PNG_IDAT;
      PNG_CONST PNG_IEND;
      PNG_CONST PNG_PLTE;
#if defined(PNG_READ_bKGD_SUPPORTED)
      PNG_CONST PNG_bKGD;
#endif
#if defined(PNG_READ_cHRM_SUPPORTED)
      PNG_CONST PNG_cHRM;
#endif
#if defined(PNG_READ_gAMA_SUPPORTED)
      PNG_CONST PNG_gAMA;
#endif
#if defined(PNG_READ_hIST_SUPPORTED)
      PNG_CONST PNG_hIST;
#endif
#if defined(PNG_READ_iCCP_SUPPORTED)
      PNG_CONST PNG_iCCP;
#endif
#if defined(PNG_READ_iTXt_SUPPORTED)
      PNG_CONST PNG_iTXt;
#endif
#if defined(PNG_READ_oFFs_SUPPORTED)
      PNG_CONST PNG_oFFs;
#endif
#if defined(PNG_READ_pCAL_SUPPORTED)
      PNG_CONST PNG_pCAL;
#endif
#if defined(PNG_READ_pHYs_SUPPORTED)
      PNG_CONST PNG_pHYs;
#endif
#if defined(PNG_READ_sBIT_SUPPORTED)
      PNG_CONST PNG_sBIT;
#endif
#if defined(PNG_READ_sCAL_SUPPORTED)
      PNG_CONST PNG_sCAL;
#endif
#if defined(PNG_READ_sPLT_SUPPORTED)
      PNG_CONST PNG_sPLT;
#endif
#if defined(PNG_READ_sRGB_SUPPORTED)
      PNG_CONST PNG_sRGB;
#endif
#if defined(PNG_READ_tEXt_SUPPORTED)
      PNG_CONST PNG_tEXt;
#endif
#if defined(PNG_READ_tIME_SUPPORTED)
      PNG_CONST PNG_tIME;
#endif
#if defined(PNG_READ_tRNS_SUPPORTED)
      PNG_CONST PNG_tRNS;
#endif
#if defined(PNG_READ_zTXt_SUPPORTED)
      PNG_CONST PNG_zTXt;
#endif
#endif /* PNG_USE_LOCAL_ARRAYS */
      png_uint_32 length = png_read_chunk_header(png_ptr);
      PNG_CONST png_bytep chunk_name = png_ptr->chunk_name;

      if (!png_memcmp(chunk_name, png_IHDR, 4))
         png_handle_IHDR(png_ptr, info_ptr, length);
      else if (!png_memcmp(chunk_name, png_IEND, 4))
         png_handle_IEND(png_ptr, info_ptr, length);
#ifdef PNG_HANDLE_AS_UNKNOWN_SUPPORTED
      else if (png_handle_as_unknown(png_ptr, chunk_name))
      {
         if (!png_memcmp(chunk_name, png_IDAT, 4))
         {
            if ((length > 0) || (png_ptr->mode & PNG_HAVE_CHUNK_AFTER_IDAT))
               png_error(png_ptr, "Too many IDAT's found");
         }
         png_handle_unknown(png_ptr, info_ptr, length);
         if (!png_memcmp(chunk_name, png_PLTE, 4))
            png_ptr->mode |= PNG_HAVE_PLTE;
      }
#endif
      else if (!png_memcmp(chunk_name, png_IDAT, 4))
      {
         /* Zero length IDATs are legal after the last IDAT has been
          * read, but not after other chunks have been read.
          */
         if ((length > 0) || (png_ptr->mode & PNG_HAVE_CHUNK_AFTER_IDAT))
            png_error(png_ptr, "Too many