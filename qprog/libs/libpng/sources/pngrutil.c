
/* pngrutil.c - utilities to read a PNG file
 *
 * Last changed in libpng 1.2.38 [July 16, 2009]
 * Copyright (c) 1998-2009 Glenn Randers-Pehrson
 * (Version 0.96 Copyright (c) 1996, 1997 Andreas Dilger)
 * (Version 0.88 Copyright (c) 1995, 1996 Guy Eric Schalnat, Group 42, Inc.)
 *
 * This code is released under the libpng license.
 * For conditions of distribution and use, see the disclaimer
 * and license in png.h
 *
 * This file contains routines that are only called from within
 * libpng itself during the course of reading an image.
 */

#define PNG_INTERNAL
#include "png.h"
#if defined(PNG_READ_SUPPORTED)

#if defined(_WIN32_WCE) && (_WIN32_WCE<0x500)
#  define WIN32_WCE_OLD
#endif

#ifdef PNG_FLOATING_POINT_SUPPORTED
#  if defined(WIN32_WCE_OLD)
/* The strtod() function is not supported on WindowsCE */
__inline double png_strtod(png_structp png_ptr, PNG_CONST char *nptr, char **endptr)
{
   double result = 0;
   int len;
   wchar_t *str, *end;

   len = MultiByteToWideChar(CP_ACP, 0, nptr, -1, NULL, 0);
   str = (wchar_t *)png_malloc(png_ptr, len * png_sizeof(wchar_t));
   if ( NULL != str )
   {
      MultiByteToWideChar(CP_ACP, 0, nptr, -1, str, len);
      result = wcstod(str, &end);
      len = WideCharToMultiByte(CP_ACP, 0, end, -1, NULL, 0, NULL, NULL);
      *endptr = (char *)nptr + (png_strlen(nptr) - len + 1);
      png_free(png_ptr, str);
   }
   return result;
}
#  else
#    define png_strtod(p,a,b) strtod(a,b)
#  endif
#endif

png_uint_32 PNGAPI
png_get_uint_31(png_structp png_ptr, png_bytep buf)
{
#ifdef PNG_READ_BIG_ENDIAN_SUPPORTED
   png_uint_32 i = png_get_uint_32(buf);
#else
   /* Avoid an extra function call by inlining the result. */
   png_uint_32 i = ((png_uint_32)(*buf) << 24) +
      ((png_uint_32)(*(buf + 1)) << 16) +
      ((png_uint_32)(*(buf + 2)) << 8) +
      (png_uint_32)(*(buf + 3));
#endif
   if (i > PNG_UINT_31_MAX)
     png_error(png_ptr, "PNG unsigned integer out of range.");
   return (i);
}
#ifndef PNG_READ_BIG_ENDIAN_SUPPORTED
/* Grab an unsigned 32-bit integer from a buffer in big-endian format. */
png_uint_32 PNGAPI
png_get_uint_32(png_bytep buf)
{
   png_uint_32 i = ((png_uint_32)(*buf) << 24) +
      ((png_uint_32)(*(buf + 1)) << 16) +
      ((png_uint_32)(*(buf + 2)) << 8) +
      (png_uint_32)(*(buf + 3));

   return (i);
}

/* Grab a signed 32-bit integer from a buffer in big-endian format.  The
 * data is stored in the PNG file in two's complement format, and it is
 * assumed that the machine format for signed integers is the same.
 */
png_int_32 PNGAPI
png_get_int_32(png_bytep buf)
{
   png_int_32 i = ((png_int_32)(*buf) << 24) +
      ((png_int_32)(*(buf + 1)) << 16) +
      ((png_int_32)(*(buf + 2)) << 8) +
      (png_int_32)(*(buf + 3));

   return (i);
}

/* Grab an unsigned 16-bit integer from a buffer in big-endian format. */
png_uint_16 PNGAPI
png_get_uint_16(png_bytep buf)
{
   png_uint_16 i = (png_uint_16)(((png_uint_16)(*buf) << 8) +
      (png_uint_16)(*(buf + 1)));

   return (i);
}
#endif /* PNG_READ_BIG_ENDIAN_SUPPORTED */

/* Read the chunk header (length + type name).
 * Put the type name into png_ptr->chunk_name, and return the length.
 */
png_uint_32 /* PRIVATE */
png_read_chunk_header(png_structp png_ptr)
{
   png_byte buf[8];
   png_uint_32 length;

   /* Read the length and the chunk name */
   png_read_data(png_ptr, buf, 8);
   length = png_get_uint_31(png_ptr, buf);

   /* Put the chunk name into png_ptr->chunk_name */
   png_memcpy(png_ptr->chunk_name, buf + 4, 4);

   png_debug2(0, "Reading %s chunk, length = %lu",
      png_ptr->chunk_name, length);

   /* Reset the crc and run it over the chunk name */
   png_reset_crc(png_ptr);
   png_calculate_crc(png_ptr, png_ptr->chunk_name, 4);

   /* Check to see if chunk name is valid */
   png_check_chunk_name(png_ptr, png_ptr->chunk_name);

   return length;
}

/* Read data, and (optionally) run it through the CRC. */
void /* PRIVATE */
png_crc_read(png_structp png_ptr, png_bytep buf, png_size_t length)
{
   if (png_ptr == NULL)
      return;
   png_read_data(png_ptr, buf, length);
   png_calculate_crc(png_ptr, buf, length);
}

/* Optionally skip data and then check the CRC.  Depending on whether we
 * are reading a ancillary or critical chunk, and how the program has set
 * things up, we may calculate the CRC on the data and print a message.
 * Returns '1' if there was a CRC error, '0' otherwise.
 */
int /* PRIVATE */
png_crc_finish(png_structp png_ptr, png_uint_32 skip)
{
   png_size_t i;
   png_size_t istop = png_ptr->zbuf_size;

   for (i = (png_size_t)skip; i > istop; i -= istop)
   {
      png_crc_read(png_ptr, png_ptr->zbuf, png_ptr->zbuf_size);
   }
   if (i)
   {
      png_crc_read(png_ptr, png_ptr->zbuf, i);
   }

   if (png_crc_error(png_ptr))
   {
      if (((png_ptr->chunk_name[0] & 0x20) &&                /* Ancillary */
          !(png_ptr->flags & PNG_FLAG_CRC_ANCILLARY_NOWARN)) ||
          (!(png_ptr->chunk_name[0] & 0x20) &&             /* Critical  */
          (png_ptr->flags & PNG_FLAG_CRC_CRITICAL_USE)))
      {
         png_chunk_warning(png_ptr, "CRC error");
      }
      else
      {
         png_chunk_error(png_ptr, "CRC error");
      }
      return (1);
   }

   return (0);
}

/* Compare the CRC stored in the PNG file with that calculated by libpng from
 * the data it has read thus far.
 */
int /* PRIVATE */
png_crc_error(png_structp png_ptr)
{
   png_byte crc_bytes[4];
   png_uint_32 crc;
   int need_crc = 1;

   if (png_ptr->chunk_name[0] & 0x20)                     /* ancillary */
   {
      if ((png_ptr->flags & PNG_FLAG_CRC_ANCILLARY_MASK) ==
          (PNG_FLAG_CRC_ANCILLARY_USE | PNG_FLAG_CRC_ANCILLARY_NOWARN))
         need_crc = 0;
   }
   else                                                    /* critical */
   {
      if (png_ptr->flags & PNG_FLAG_CRC_CRITICAL_IGNORE)
         need_crc = 0;
   }

   png_read_data(png_ptr, crc_bytes, 4);

   if (need_crc)
   {
      crc = png_get_uint_32(crc_bytes);
      return ((int)(crc != png_ptr->crc));
   }
   else
      return (0);
}

#if defined(PNG_READ_zTXt_SUPPORTED) || defined(PNG_READ_iTXt_SUPPORTED) || \
    defined(PNG_READ_iCCP_SUPPORTED)
/*
 * Decompress trailing data in a chunk.  The assumption is that chunkdata
 * points at an allocated area holding the contents of a chunk with a
 * trailing compressed part.  What we get back is an allocated area
 * holding the original prefix part and an uncompressed version of the
 * trailing part (the malloc area passed in is freed).
 */
void /* PRIVATE */
png_decompress_chunk(png_structp png_ptr, int comp_type,
                              png_size_t chunklength,
                              png_size_t prefix_size, png_size_t *newlength)
{
   static PNG_CONST char msg[] = "Error decoding compressed text";
   png_charp text;
   png_size_t text_size;

   if (comp_type == PNG_COMPRESSION_TYPE_BASE)
   {
      int ret = Z_OK;
      png_ptr->zstream.next_in = (png_bytep)(png_ptr->chunkdata + prefix_size);
      png_ptr->zstream.avail_in = (uInt)(chunklength - prefix_size);
      png_ptr->zstream.next_out = png_ptr->zbuf;
      png_ptr->zstream.avail_out = (uInt)png_ptr->zbuf_size;

      text_size = 0;
      text = NULL;

      while (png_ptr->zstream.avail_in)
      {
         ret = inflate(&png_ptr->zstream, Z_PARTIAL_FLUSH);
         if (ret != Z_OK && ret != Z_STREAM_END)
         {
            if (png_ptr->zstream.msg != NULL)
               png_warning(png_ptr, png_ptr->zstream.msg);
            else
               png_warning(png_ptr, msg);
            inflateReset(&png_ptr->zstream);
            png_ptr->zstream.avail_in = 0;

            if (text ==  NULL)
            {
               text_size = prefix_size + png_sizeof(msg) + 1;
               text = (png_charp)png_malloc_warn(png_ptr, text_size);
               if (text ==  NULL)
                 {
                    png_free(png_ptr, png_ptr->chunkdata);
                    png_ptr->chunkdata = NULL;
                    png_error(png_ptr, "Not enough memory to decompress chunk");
                 }
               png_memcpy(text, png_ptr->chunkdata, prefix_size);
            }

            text[text_size - 1] = 0x00;

            /* Copy what we can of the error message into the text chunk */
            text_size = (png_size_t)(chunklength -
              (text - png_ptr->chunkdata) - 1);
            if (text_size > png_sizeof(msg))
               text_size = png_sizeof(msg);
            png_memcpy(text + prefix_size, msg, text_size);
            break;
         }
         if (!png_ptr->zstream.avail_out || ret == Z_STREAM_END)
         {
            if (text == NULL)
            {
               text_size = prefix_size +
                   png_ptr->zbuf_size - png_ptr->zstream.avail_out;
               text = (png_charp)png_malloc_warn(png_ptr, text_size + 1);
               if (text ==  NULL)
               {
                  png_free(png_ptr, png_ptr->chunkdata);
                  png_ptr->chunkdata = NULL;
                  png_error(png_ptr,
                    "Not enough memory to decompress chunk.");
               }
               png_memcpy(text + prefix_size, png_ptr->zbuf,
                    text_size - prefix_size);
               png_memcpy(text, png_ptr->chunkdata, prefix_size);
               *(text + text_size) = 0x00;
            }
            else
            {
               png_charp tmp;

               tmp = text;
               text = (png_charp)png_malloc_warn(png_ptr,
                  (png_uint_32)(text_size +
                  png_ptr->zbuf_size - png_ptr->zstream.avail_out + 1));
               if (text == NULL)
               {
                  png_free(png_ptr, tmp);
                  png_free(png_ptr, png_ptr->chunkdata);
                  png_ptr->chunkdata = NULL;
                  png_error(png_ptr,
                    "Not enough memory to decompress chunk..");
               }
               png_memcpy(text, tmp, text_size);
               png_free(png_ptr, tmp);
               png_memcpy(text + text_size, png_ptr->zbuf,
                  (png_ptr->zbuf_size - png_ptr->zstream.avail_out));
               text_size += png_ptr->zbuf_size - png_ptr->zstream.avail_out;
               *(text + text_size) = 0x00;
            }
            if (ret == Z_STREAM_END)
               break;
            else
            {
               png_ptr->zstream.next_out = png_ptr->zbuf;
               png_ptr->zstream.avail_out = (uInt)png_ptr->zbuf_size;
            }
         }
      }
      if (ret != Z_STREAM_END)
      {
#if !defined(PNG_NO_STDIO) && !defined(_WIN32_WCE)
         char umsg[52];

         if (ret == Z_BUF_ERROR)
            png_snprintf(umsg, 52,
                "Buffer error in compressed datastream in %s chunk",
                png_ptr->chunk_name);

         else if (ret == Z_DATA_ERROR)
            png_snprintf(umsg, 52,
                "Data error in compressed datastream in %s chunk",
                png_ptr->chunk_name);

         else
            png_snprintf(umsg, 52,
                "Incomplete compressed datastream in %s chunk",
                png_ptr->chunk_name);

         png_warning(png_ptr, umsg);
#else
         png_warning(png_ptr,
            "Incomplete compressed datastream in chunk other than IDAT");
#endif
         text_size = prefix_size;
         if (text ==  NULL)
         {
            text = (png_charp)png_malloc_warn(png_ptr, text_size+1);
            if (text == NULL)
              {
                png_free(png_ptr, png_ptr->chunkdata);
                png_ptr->chunkdata = NULL;
                png_error(png_ptr, "Not enough memory for text.");
              }
            png_memcpy(text, png_ptr->chunkdata, prefix_size);
         }
         *(text + text_size) = 0x00;
      }

      inflateReset(&png_ptr->zstream);
      png_ptr->zstream.avail_in = 0;

      png_free(png_ptr, png_ptr->chunkdata);
      png_ptr->chunkdata = text;
      *newlength=text_size;
   }
   else /* if (comp_type != PNG_COMPRESSION_TYPE_BASE) */
   {
#if !defined(PNG_NO_STDIO) && !defined(_WIN32_WCE)
      char umsg[50];

      png_snprintf(umsg, 50, "Unknown zTXt compression type %d", comp_type);
      png_warning(png_ptr, umsg);
#else
      png_warning(png_ptr, "Unknown zTXt compression type");
#endif

      *(png_ptr->chunkdata + prefix_size) = 0x00;
      *newlength = prefix_size;
   }
}
#endif

/* Read and check the IDHR chunk */
void /* PRIVATE */
png_handle_IHDR(png_structp png_ptr, png_infop info_ptr, png_uint_32 length)
{
   png_byte buf[13];
   png_uint_32 width, height;
   int bit_depth, color_type, compression_type, filter_type;
   int interlace_type;

   png_debug(1, "in png_handle_IHDR");

   if (png_ptr->mode & PNG_HAVE_IHDR)
      png_error(png_ptr, "Out of place IHDR");

   /* Check the length */
   if (length != 13)
      png_error(png_ptr, "Invalid IHDR chunk");

   png_ptr->mode |= PNG_HAVE_IHDR;

   png_crc_read(png_ptr, buf, 13);
   png_crc_finish(png_ptr, 0);

   width = png_get_uint_31(png_ptr, buf);
   height = png_get_uint_31(png_ptr, buf + 4);
   bit_depth = buf[8];
   color_type = buf[9];
   compression_type = buf[10];
   filter_type = buf[11];
   interlace_type = buf[12];

   /* Set internal variables */
   png_ptr->width = width;
   png_ptr->height = height;
   png_ptr->bit_depth = (png_byte)bit_depth;
   png_ptr->interlaced = (png_byte)interlace_type;
   png_ptr->color_type = (png_byte)color_type;
#if defined(PNG_MNG_FEATURES_SUPPORTED)
   png_ptr->filter_type = (png_byte)filter_type;
#endif
   png_ptr->compression_type = (png_byte)compression_type;

   /* Find number of channels */
   switch (png_ptr->color_type)
   {
      case PNG_COLOR_TYPE_GRAY:
      case PNG_COLOR_TYPE_PALETTE:
         png_ptr->channels = 1;
         break;

      case PNG_COLOR_TYPE_RGB:
         png_ptr->channels = 3;
         break;

      case PNG_COLOR_TYPE_GRAY_ALPHA:
         png_ptr->channels = 2;
         break;

      case PNG_COLOR_TYPE_RGB_ALPHA:
         png_ptr->channels = 4;
         break;
   }

   /* Set up other useful info */
   png_ptr->pixel_depth = (png_byte)(png_ptr->bit_depth *
   png_ptr->channels);
   png_ptr->rowbytes = PNG_ROWBYTES(png_ptr->pixel_depth, png_ptr->width);
   png_debug1(3, "bit_depth = %d", png_ptr->bit_depth);
   png_debug1(3, "channels = %d", png_ptr->channels);
   png_debug1(3, "rowbytes = %lu", png_ptr->rowbytes);
   png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth,
      color_type, interlace_type, compression_type, filter_type);
}

/* Read and check the palette */
void /* PRIVATE */
png_handle_PLTE(png_structp png_ptr, png_infop info_ptr, png_uint_32 length)
{
   png_color palette[PNG_MAX_PALETTE_LENGTH];
   int num, i;
#ifndef PNG_NO_POINTER_INDEXING
   png_colorp pal_ptr;
#endif

   png_debug(1, "in png_handle_PLTE");

   if (!(png_ptr->mode & PNG_HAVE_IHDR))
      png_error(png_ptr, "Missing IHDR before PLTE");

   else if (png_ptr->mode & PNG_HAVE_IDAT)
   {
      png_warning(png_ptr, "Invalid PLTE after IDAT");
      png_crc_finish(png_ptr, length);
      return;
   }

   else if (png_ptr->mode & PNG_HAVE_PLTE)
      png_error(png_ptr, "Duplicate PLTE chunk");

   png_ptr->mode |= PNG_HAVE_PLTE;

   if (!(png_ptr->color_type&PNG_COLOR_MASK_COLOR))
   {
      png_warning(png_ptr,
        "Ignoring PLTE chunk in grayscale PNG");
      png_crc_finish(png_ptr, length);
      return;
   }
#if !defined(PNG_READ_OPT_PLTE_SUPPORTED)
   if (png_ptr->color_type != PNG_COLOR_TYPE_PALETTE)
   {
      png_crc_finish(png_ptr, length);
      return;
   }
#endif

   if (length > 3*PNG_MAX_PALETTE_LENGTH || length % 3)
   {
      if (png_ptr->color_type != PNG_COLOR_TYPE_PALETTE)
      {
         png_warning(png_ptr, "Invalid palette chunk");
         png_crc_finish(png_ptr, length);
         return;
      }

      else
      {
         png_error(png_ptr, "Invalid palette chunk");
      }
   }

   num = (int)length / 3;

#ifndef PNG_NO_POINTER_INDEXING
   for (i = 0, pal_ptr = palette; i < num; i++, pal_ptr++)
   {
      png_byte buf[3];

      png_crc_read(png_ptr, buf, 3);
      pal_ptr->red = buf[0];
      pal_ptr->green = buf[1];
      pal_ptr->blue = buf[2];
   }
#else
   for (i = 0; i < num; i++)
   {
      png_byte buf[3];

      png_crc_read(png_ptr, buf, 3);
      /* Don't depend upon png_color being any order */
      palette[i].red = buf[0];
      palette[i].green = buf[1];
      palette[i].blue = buf[2];
   }
#endif

   /* If we actually NEED the PLTE chunk (ie for a paletted image), we do
    * whatever the normal CRC configuration tells us.  However, if we
    * have an RGB image, the PLTE can be considered ancillary, so
    * we will act as though it is.
    */
#if !defined(PNG_READ_OPT_PLTE_SUPPORTED)
   if (png_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
#endif
   {
      png_crc_finish(png_ptr, 0);
   }
#if !defined(PNG_READ_OPT_PLTE_SUPPORTED)
   else if (png_crc_error(png_ptr))  /* Only if we have a CRC error */
   {
      /* If we don't want to use the data from an ancillary chunk,
         we have two options: an error abort, or a warning and we
         ignore the data in this chunk (which should be OK, since
         it's considered ancillary for a RGB or RGBA image). */
      if (!(png_ptr->flags & PNG_FLAG_CRC_ANCILLARY_USE))
      {
         if (png_ptr->flags & PNG_FLAG_CRC_ANCILLARY_NOWARN)
         {
            png_chunk_error(png_ptr, "CRC error");
         }
         else
         {
            png_chunk_warning(png_ptr, "CRC error");
            return;
         }
      }
      /* Otherwise, we (optionally) emit a warning and use the chunk. */
      else if (!(png_ptr->flags & PNG_FLAG_CRC_ANCILLARY_NOWARN))
      {
         png_chunk_warning(png_ptr, "CRC error");
      }
   }
#endif

   png_set_PLTE(png_ptr, info_ptr, palette, num);

#if defined(PNG_READ_tRNS_SUPPORTED)
   if (png_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
   {
      if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_tRNS))
      {
         if (png_ptr->num_trans > (png_uint_16)num)
         {
            png_warning(png_ptr, "Truncating incorrect tRNS chunk length");
            png_ptr->num_trans = (png_uint_16)num;
         }
         if (info_ptr->num_trans > (png_uint_16)num)
         {
            png_warning(png_ptr, "Truncating incorrect info tRNS chunk length");
            info_ptr->num_trans = (png_uint_16)num;
         }
      }
   }
#endif

}

void /* PRIVATE */
png_handle_IEND(png_structp png_ptr, png_infop info_ptr, png_uint_32 length)
{
   png_debug(1, "in png_handle_IEND");

   if (!(png_ptr->mode & PNG_HAVE_IHDR) || !(png_ptr->mode & PNG_HAVE_IDAT))
   {
      png_error(png_ptr, "No image in file");
   }

   png_ptr->mode |= (PNG_AFTER_IDAT | PNG_HAVE_IEND);

   if (length != 0)
   {
      png_warning(png_ptr, "Incorrect IEND chunk length");
   }
   png_crc_finish(png_ptr, length);

   info_ptr = info_ptr; /* Quiet compiler warnings about unused info_ptr */
}

#if defined(PNG_READ_gAMA_SUPPORTED)
void /* PRIVATE */
png_handle_gAMA(png_structp png_ptr, png_infop info_ptr, png_uint_32 length)
{
   png_fixed_point igamma;
#ifdef PNG_FLOATING_POINT_SUPPORTED
   float file_gamma;
#endif
   png_byte buf[4];

   png_debug(1, "in png_handle_gAMA");

   if (!(png_ptr->mode & PNG_HAVE_IHDR))
      png_error(png_ptr, "Missing IHDR before gAMA");
   else if (png_ptr->mode & PNG_HAVE_IDAT)
   {
      png_warning(png_ptr, "Invalid gAMA after IDAT");
      png_crc_finish(png_ptr, length);
      return;
   }
   else if (png_ptr->mode & PNG_HAVE_PLTE)
      /* Should be an error, but we can cope with it */
      png_warning(png_ptr, "Out of place gAMA chunk");

   if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_gAMA)
#if defined(PNG_READ_sRGB_SUPPORTED)
      && !(info_ptr->valid & PNG_INFO_sRGB)
#endif
      )
   {
      png_warning(png_ptr, "Duplicate gAMA chunk");
      png_crc_finish(png_ptr, length);
      return;
   }

   if (length != 4)
   {
      png_warning(png_ptr, "Incorrect gAMA chunk length");
      png_crc_finish(png_ptr, length);
      return;
   }

   png_crc_read(png_ptr, buf, 4);
   if (png_crc_finish(png_ptr, 0))
      return;

   igamma = (png_fixed_point)png_get_uint_32(buf);
   /* Check for zero gamma */
   if (igamma == 0)
      {
         png_warning(png_ptr,
           "Ignoring gAMA chunk with gamma=0");
         return;
      }

#if defined(PNG_READ_sRGB_SUPPORTED)
   if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_sRGB))
      if (PNG_OUT_OF_RANGE(igamma, 45500L, 500))
      {
         png_warning(png_ptr,
           "Ignoring incorrect gAMA value when sRGB is also present");
#ifndef PNG_NO_CONSOLE_IO
         fprintf(stderr, "gamma = (%d/100000)", (int)igamma);
#endif
         return;
      }
#endif /* PNG_READ_sRGB_SUPPORTED */

#ifdef PNG_FLOATING_POINT_SUPPORTED
   file_gamma = (float)igamma / (float)100000.0;
#  ifdef PNG_READ_GAMMA_SUPPORTED
     png_ptr->gamma = file_gamma;
#  endif
     png_set_gAMA(png_ptr, info_ptr, file_gamma);
#endif
#ifdef PNG_FIXED_POINT_SUPPORTED
   png_set_gAMA_fixed(png_ptr, info_ptr, igamma);
#endif
}
#endif

#if defined(PNG_READ_sBIT_SUPPORTED)
void /* PRIVATE */
png_handle_sBIT(png_structp png_ptr, png_infop info_ptr, png_uint_32 length)
{
   png_size_t truelen;
   png_byte buf[4];

   png_debug(1, "in png_handle_sBIT");

   buf[0] = buf[1] = buf[2] = buf[3] = 0;

   if (!(png_ptr->mode & PNG_HAVE_IHDR))
      png_error(png_ptr, "Missing IHDR before sBIT");
   else if (png_ptr->mode & PNG_HAVE_IDAT)
   {
      png_warning(png_ptr, "Invalid sBIT after IDAT");
      png_crc_finish(png_ptr, length);
      return;
   }
   else if (png_ptr->mode & PNG_HAVE_PLTE)
   {
      /* Should be an error, but we can cope with it */
      png_warning(png_ptr, "Out of place sBIT chunk");
   }
   if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_sBIT))
   {
      png_warning(png_ptr, "Duplicate sBIT chunk");
      png_crc_finish(png_ptr, length);
      return;
   }

   if (png_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
      truelen = 3;
   else
      truelen = (png_size_t)png_ptr->channels;

   if (length != truelen || length > 4)
   {
      png_warning(png_ptr, "Incorrect sBIT chunk length");
      png_crc_finish(png_ptr, length);
      return;
   }

   png_crc_read(png_ptr, buf, truelen);
   if (png_crc_finish(png_ptr, 0))
      return;

   if (png_ptr->color_type & PNG_COLOR_MASK_COLOR)
   {
      png_ptr->sig_bit.red = buf[0];
      png_ptr->sig_bit.green = buf[1];
      png_ptr->sig_bit.blue = buf[2];
      png_ptr->sig_bit.alpha = buf[3];
   }
   else
   {
      png_ptr->sig_bit.gray = buf[0];
      png_ptr->sig_bit.red = buf[0];
      png_ptr->sig_bit.green = buf[0];
      png_ptr->sig_bit.blue = buf[0];
      png_ptr->sig_bit.alpha = buf[1];
   }
   png_set_sBIT(png_ptr, info_ptr, &(png_ptr->sig_bit));
}
#endif

#if defined(PNG_READ_cHRM_SUPPORTED)
void /* PRIVATE */
png_handle_cHRM(png_structp png_ptr, png_infop info_ptr, png_uint_32 length)
{
   png_byte buf[32];
#ifdef PNG_FLOATING_POINT_SUPPORTED
   float white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y;
#endif
   png_fixed_point int_x_white, int_y_white, int_x_red, int_y_red, int_x_green,
      int_y_green, int_x_blue, int_y_blue;

   png_uint_32 uint_x, uint_y;

   png_debug(1, "in png_handle_cHRM");

   if (!(png_ptr->mode & PNG_HAVE_IHDR))
      png_error(png_ptr, "Missing IHDR before cHRM");
   else if (png_ptr->mode & PNG_HAVE_IDAT)
   {
      png_warning(png_ptr, "Invalid cHRM after IDAT");
      png_crc_finish(png_ptr, length);
      return;
   }
   else if (png_ptr->mode & PNG_HAVE_PLTE)
      /* Should be an error, but we can cope with it */
      png_warning(png_ptr, "Missing PLTE before cHRM");

   if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_cHRM)
#if defined(PNG_READ_sRGB_SUPPORTED)
      && !(info_ptr->valid & PNG_INFO_sRGB)
#endif
      )
   {
      png_warning(png_ptr, "Duplicate cHRM chunk");
      png_crc_finish(png_ptr, length);
      return;
   }

   if (length != 32)
   {
      png_warning(png_ptr, "Incorrect cHRM chunk length");
      png_crc_finish(png_ptr, length);
      return;
   }

   png_crc_read(png_ptr, buf, 32);
   if (png_crc_finish(png_ptr, 0))
      return;

   uint_x = png_get_uint_32(buf);
   uint_y = png_get_uint_32(buf + 4);
   int_x_white = (png_fixed_point)uint_x;
   int_y_white = (png_fixed_point)uint_y;

   uint_x = png_get_uint_32(buf + 8);
   uint_y = png_get_uint_32(buf + 12);
   int_x_red = (png_fixed_point)uint_x;
   int_y_red = (png_fixed_point)uint_y;

   uint_x = png_get_uint_32(buf + 16);
   uint_y = png_get_uint_32(buf + 20);
   int_x_green = (png_fixed_point)uint_x;
   int_y_green = (png_fixed_point)uint_y;

   uint_x = png_get_uint_32(buf + 24);
   uint_y = png_get_uint_32(buf + 28);
   int_x_blue = (png_fixed_point)uint_x;
   int_y_blue = (png_fixed_point)uint_y;

#ifdef PNG_FLOATING_POINT_SUPPORTED
   white_x = (float)int_x_white / (float)100000.0;
   white_y = (float)int_y_white / (float)100000.0;
   red_x   = (float)int_x_red   / (float)100000.0;
   red_y   = (float)int_y_red   / (float)100000.0;
   green_x = (float)int_x_green / (float)100000.0;
   green_y = (float)int_y_green / (float)100000.0;
   blue_x  = (float)int_x_blue  / (float)100000.0;
   blue_y  = (float)int_y_blue  / (float)100000.0;
#endif

#if defined(PNG_READ_sRGB_SUPPORTED)
   if ((info_ptr != NULL) && (info_ptr->valid & PNG_INFO_sRGB))
      {
      if (PNG_OUT_OF_RANGE(int_x_white, 31270,  1000) ||
          PNG_OUT_OF_RANGE(int_y_white, 32900,  1000) ||
          PNG_OUT_OF_RANGE(int_x_red,   64000L, 1000) ||
          PNG_OUT_OF_RANGE(int_y_red,   33000,  1000) ||
          PNG_OUT_OF_RANGE(int_x_green, 30000,  1000) ||
          PNG_OUT_OF_RANGE(int_y_green, 60000L, 1000) ||
          PNG_OUT_OF_RANGE(int_x_blue,  15000,  1000) ||
          PNG_OUT_OF_RANGE(int_y_blue,   6000,  1000))
         {
            png_warning(png_ptr,
              "Ignoring incorrect cHRM value when sRGB is also present");
#ifndef PNG_NO_CONSOLE_IO
#ifdef PNG_FLOATING_POINT_SUPPORTED
            fprintf(stderr, "wx=%f, wy=%f, rx=%f, ry=%f\n",
               white_x, white_y, red_x, red_y);
            fprintf(stderr, "gx=%f, gy=%f, bx=%f, by=%f\n",
               green_x, green_y, blue_x, blue_y);
#else
            fprintf(stderr, "wx=%ld, wy=%ld, rx=%ld, ry=%ld\n",
               int_x_white, int_y_white, int_x_red, int_y_red);
            fprintf(stderr, "gx=%ld, gy=%ld, bx=%ld, by=%ld\n",
               int_x_green, int_y_green, int_x_blue, int_y_blue);
#endif
#endif /* PNG_NO_CONSOLE_IO */
         }
         return;
      }
#endif /* PNG_READ_sRGB_SUPPORTED */

#ifdef PNG_FLOATING_POINT_SUPPORTED
   png_set_cHRM(png_ptr, info_ptr,
      white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y);
#endif
#ifdef PNG_FIXED_POINT_SUPPORTED
   png_set_cHRM_fixed(png_ptr, info_ptr,
      int_x_white, int_y_white, int_x_red, int_y_red, int_x_green,
      int_y_green, int_x_blue, int_y_blue);
#endif
}
#endif

#if defined(PNG_READ_sRGB_SUPPORTED)
void /* PRIVATE */
png_handle_sRGB(png_structp png_ptr, png_infop info_ptr, png_uint_32 length)
{
   int intent;
   png_byte buf[1];

   png_debug(1, "in png_handle_sRGB");

   if (!(png_ptr->mode & PNG_HAVE_IHDR))
      png_error(png_ptr, "Missing IHDR before sRGB");
   else if (png_ptr->mode & PNG_HAVE_IDAT)
   {
      png_warning(png_ptr, "Invalid sRGB after IDAT");
      png_crc_finish(png_ptr, length);
      return;
   }
   else if (png_ptr->mode & PNG_HAVE_PLTE)
      /* Should be an error, but we can cope with it */
      png_warning(png_ptr, "Out of place sRGB chunk");

   if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_sRGB))
   {
      png_warning(png_ptr, "Duplicate sRGB chunk");
      png_crc_finish(png_ptr, length);
      return;
   }

   if (length != 1)
   {
      png_warning(png_ptr, "Incorrect sRGB chunk length");
      png_crc_finish(png_ptr, length);
      return;
   }

   png_crc_read(png_ptr, buf, 1);
   if (png_crc_finish(png_ptr, 0))
      return;

   intent = buf[0];
   /* Check for bad intent */
   if (intent >= PNG_sRGB_INTENT_LAST)
   {
      png_warning(png_ptr, "Unknown sRGB intent");
      return;
   }

#if defined(PNG_READ_gAMA_SUPPORTED) && defined(PNG_READ_GAMMA_SUPPORTED)
   if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_gAMA))
   {
   png_fixed_point igamma;
#ifdef PNG_FIXED_POINT_SUPPORTED
      igamma=info_ptr->int_gamma;
#else
#  ifdef PNG_FLOATING_POINT_SUPPORTED
      igamma=(png_fixed_point)(info_ptr->gamma * 100000.);
#  endif
#endif
      if (PNG_OUT_OF_RANGE(igamma, 45500L, 500))
      {
         png_warning(png_ptr,
           "Ignoring incorrect gAMA value when sRGB is also present");
#ifndef PNG_NO_CONSOLE_IO
#  ifdef PNG_FIXED_POINT_SUPPORTED
         fprintf(stderr, "incorrect gamma=(%d/100000)\n",
            (int)png_ptr->int_gamma);
#  else
#    ifdef PNG_FLOATING_POINT_SUPPORTED
         fprintf(stderr, "incorrect gamma=%f\n", png_ptr->gamma);
#    endif
#  endif
#endif
      }
   }
#endif /* PNG_READ_gAMA_SUPPORTED */

#ifdef PNG_READ_cHRM_SUPPORTED
#ifdef PNG_FIXED_POINT_SUPPORTED
   if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_cHRM))
      if (PNG_OUT_OF_RANGE(info_ptr->int_x_white, 31270,  1000) ||
          PNG_OUT_OF_RANGE(info_ptr->int_y_white, 32900,  1000) ||
          PNG_OUT_OF_RANGE(info_ptr->int_x_red,   64000L, 1000) ||
          PNG_OUT_OF_RANGE(info_ptr->int_y_red,   33000,  1000) ||
          PNG_OUT_OF_RANGE(info_ptr->int_x_green, 30000,  1000) ||
          PNG_OUT_OF_RANGE(info_ptr->int_y_green, 60000L, 1000) ||
          PNG_OUT_OF_RANGE(info_ptr->int_x_blue,  15000,  1000) ||
          PNG_OUT_OF_RANGE(info_ptr->int_y_blue,   6000,  1000))
         {
            png_warning(png_ptr,
              "Ignoring incorrect cHRM value when sRGB is also present");
         }
#endif /* PNG_FIXED_POINT_SUPPORTED */
#endif /* PNG_READ_cHRM_SUPPORTED */

   png_set_sRGB_gAMA_and_cHRM(png_ptr, info_ptr, intent);
}
#endif /* PNG_READ_sRGB_SUPPORTED */

#if defined(PNG_READ_iCCP_SUPPORTED)
void /* PRIVATE */
png_handle_iCCP(png_structp png_ptr, png_infop info_ptr, png_uint_32 length)
/* Note: this does not properly handle chunks that are > 64K under DOS */
{
   png_byte compression_type;
   png_bytep pC;
   png_charp profile;
   png_uint_32 skip = 0;
   png_uint_32 profile_size, profile_length;
   png_size_t slength, prefix_length, data_length;

   png_debug(1, "in png_handle_iCCP");

   if (!(png_ptr->mode & PNG_HAVE_IHDR))
      png_error(png_ptr, "Missing IHDR before iCCP");
   else if (png_ptr->mode & PNG_HAVE_IDAT)
   {
      png_warning(png_ptr, "Invalid iCCP after IDAT");
      png_crc_finish(png_ptr, length);
      return;
   }
   else if (png_ptr->mode & PNG_HAVE_PLTE)
      /* Should be an error, but we can cope with it */
      png_warning(png_ptr, "Out of place iCCP chunk");

   if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_iCCP))
   {
      png_warning(png_ptr, "Duplicate iCCP chunk");
      png_crc_finish(png_ptr, length);
      return;
   }

#ifdef PNG_MAX_MALLOC_64K
   if (length > (png_uint_32)65535L)
   {
      png_warning(png_ptr, "iCCP chunk too large to fit in memory");
      skip = length - (png_uint_32)65535L;
      length = (png_uint_32)65535L;
   }
#endif

   png_free(png_ptr, png_ptr->chunkdata);
   png_ptr->chunkdata = (png_charp)png_malloc(png_ptr, length + 1);
   slength = (png_size_t)length;
   png_crc_read(png_ptr, (png_bytep)png_ptr->chunkdata, slength);

   if (png_crc_finish(png_ptr, skip))
   {
      png_free(png_ptr, png_ptr->chunkdata);
      png_ptr->chunkdata = NULL;
      return;
   }

   png_ptr->chunkdata[slength] = 0x00;

   for (profile = png_ptr->chunkdata; *profile; profile++)
      /* Empty loop to find end of name */ ;

   ++profile;

   /* There should be at least one zero (the compression type byte)
    * following the separator, and we should be on it
    */
   if ( profile >= png_ptr->chunkdata + slength - 1)
   {
      png_free(png_ptr, png_ptr->chunkdata);
      png_ptr->chunkdata = NULL;
      png_warning(png_ptr, "Malformed iCCP chunk");
      return;
   }

   /* Compression_type should always be zero */
   compression_type = *profile++;
   if (compression_type)
   {
      png_warning(png_ptr, "Ignoring nonzero compression type in iCCP chunk");
      compression_type = 0x00;  /* Reset it to zero (libpng-1.0.6 through 1.0.8
                                 wrote nonzero) */
   }

   prefix_length = profile - png_ptr->chunkdata;
   png_decompress_chunk(png_ptr, compression_type,
     slength, prefix_length, &data_length);

   profile_length = data_length - prefix_length;

   if ( prefix_length > data_length || profile_length < 4)
   {
      png_free(png_ptr, png_ptr->chunkdata);
      png_ptr->chun