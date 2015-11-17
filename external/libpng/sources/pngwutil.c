
/* pngwutil.c - utilities to write a PNG file
 *
 * Last changed in libpng 1.2.40 [September 10, 2009]
 * Copyright (c) 1998-2009 Glenn Randers-Pehrson
 * (Version 0.96 Copyright (c) 1996, 1997 Andreas Dilger)
 * (Version 0.88 Copyright (c) 1995, 1996 Guy Eric Schalnat, Group 42, Inc.)
 *
 * This code is released under the libpng license.
 * For conditions of distribution and use, see the disclaimer
 * and license in png.h
 */

#define PNG_INTERNAL
#include "png.h"
#ifdef PNG_WRITE_SUPPORTED

/* Place a 32-bit number into a buffer in PNG byte order.  We work
 * with unsigned numbers for convenience, although one supported
 * ancillary chunk uses signed (two's complement) numbers.
 */
void PNGAPI
png_save_uint_32(png_bytep buf, png_uint_32 i)
{
   buf[0] = (png_byte)((i >> 24) & 0xff);
   buf[1] = (png_byte)((i >> 16) & 0xff);
   buf[2] = (png_byte)((i >> 8) & 0xff);
   buf[3] = (png_byte)(i & 0xff);
}

/* The png_save_int_32 function assumes integers are stored in two's
 * complement format.  If this isn't the case, then this routine needs to
 * be modified to write data in two's complement format.
 */
void PNGAPI
png_save_int_32(png_bytep buf, png_int_32 i)
{
   buf[0] = (png_byte)((i >> 24) & 0xff);
   buf[1] = (png_byte)((i >> 16) & 0xff);
   buf[2] = (png_byte)((i >> 8) & 0xff);
   buf[3] = (png_byte)(i & 0xff);
}

/* Place a 16-bit number into a buffer in PNG byte order.
 * The parameter is declared unsigned int, not png_uint_16,
 * just to avoid potential problems on pre-ANSI C compilers.
 */
void PNGAPI
png_save_uint_16(png_bytep buf, unsigned int i)
{
   buf[0] = (png_byte)((i >> 8) & 0xff);
   buf[1] = (png_byte)(i & 0xff);
}

/* Simple function to write the signature.  If we have already written
 * the magic bytes of the signature, or more likely, the PNG stream is
 * being embedded into another stream and doesn't need its own signature,
 * we should call png_set_sig_bytes() to tell libpng how many of the
 * bytes have already been written.
 */
void /* PRIVATE */
png_write_sig(png_structp png_ptr)
{
   png_byte png_signature[8] = {137, 80, 78, 71, 13, 10, 26, 10};

   /* Write the rest of the 8 byte signature */
   png_write_data(png_ptr, &png_signature[png_ptr->sig_bytes],
      (png_size_t)(8 - png_ptr->sig_bytes));
   if (png_ptr->sig_bytes < 3)
      png_ptr->mode |= PNG_HAVE_PNG_SIGNATURE;
}

/* Write a PNG chunk all at once.  The type is an array of ASCII characters
 * representing the chunk name.  The array must be at least 4 bytes in
 * length, and does not need to be null terminated.  To be safe, pass the
 * pre-defined chunk names here, and if you need a new one, define it
 * where the others are defined.  The length is the length of the data.
 * All the data must be present.  If that is not possible, use the
 * png_write_chunk_start(), png_write_chunk_data(), and png_write_chunk_end()
 * functions instead.
 */
void PNGAPI
png_write_chunk(png_structp png_ptr, png_bytep chunk_name,
   png_bytep data, png_size_t length)
{
   if (png_ptr == NULL)
      return;
   png_write_chunk_start(png_ptr, chunk_name, (png_uint_32)length);
   png_write_chunk_data(png_ptr, data, (png_size_t)length);
   png_write_chunk_end(png_ptr);
}

/* Write the start of a PNG chunk.  The type is the chunk type.
 * The total_length is the sum of the lengths of all the data you will be
 * passing in png_write_chunk_data().
 */
void PNGAPI
png_write_chunk_start(png_structp png_ptr, png_bytep chunk_name,
   png_uint_32 length)
{
   png_byte buf[8];

   png_debug2(0, "Writing %s chunk, length = %lu", chunk_name,
      (unsigned long)length);

   if (png_ptr == NULL)
      return;

   /* Write the length and the chunk name */
   png_save_uint_32(buf, length);
   png_memcpy(buf + 4, chunk_name, 4);
   png_write_data(png_ptr, buf, (png_size_t)8);
   /* Put the chunk name into png_ptr->chunk_name */
   png_memcpy(png_ptr->chunk_name, chunk_name, 4);
   /* Reset the crc and run it over the chunk name */
   png_reset_crc(png_ptr);
   png_calculate_crc(png_ptr, chunk_name, (png_size_t)4);
}

/* Write the data of a PNG chunk started with png_write_chunk_start().
 * Note that multiple calls to this function are allowed, and that the
 * sum of the lengths from these calls *must* add up to the total_length
 * given to png_write_chunk_start().
 */
void PNGAPI
png_write_chunk_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
   /* Write the data, and run the CRC over it */
   if (png_ptr == NULL)
      return;
   if (data != NULL && length > 0)
   {
      png_write_data(png_ptr, data, length);
      /* Update the CRC after writing the data,
       * in case that the user I/O routine alters it.
       */
      png_calculate_crc(png_ptr, data, length);
   }
}

/* Finish a chunk started with png_write_chunk_start(). */
void PNGAPI
png_write_chunk_end(png_structp png_ptr)
{
   png_byte buf[4];

   if (png_ptr == NULL) return;

   /* Write the crc in a single operation */
   png_save_uint_32(buf, png_ptr->crc);

   png_write_data(png_ptr, buf, (png_size_t)4);
}

#if defined(PNG_WRITE_TEXT_SUPPORTED) || defined(PNG_WRITE_iCCP_SUPPORTED)
/* This pair of functions encapsulates the operation of (a) compressing a
 * text string, and (b) issuing it later as a series of chunk data writes.
 * The compression_state structure is shared context for these functions
 * set up by the caller in order to make the whole mess thread-safe.
 */

typedef struct
{
   char *input;   /* The uncompressed input data */
   int input_len;   /* Its length */
   int num_output_ptr; /* Number of output pointers used */
   int max_output_ptr; /* Size of output_ptr */
   png_charpp output_ptr; /* Array of pointers to output */
} compression_state;

/* Compress given text into storage in the png_ptr structure */
static int /* PRIVATE */
png_text_compress(png_structp png_ptr,
        png_charp text, png_size_t text_len, int compression,
        compression_state *comp)
{
   int ret;

   comp->num_output_ptr = 0;
   comp->max_output_ptr = 0;
   comp->output_ptr = NULL;
   comp->input = NULL;
   comp->input_len = 0;

   /* We may just want to pass the text right through */
   if (compression == PNG_TEXT_COMPRESSION_NONE)
   {
       comp->input = text;
       comp->input_len = text_len;
       return((int)text_len);
   }

   if (compression >= PNG_TEXT_COMPRESSION_LAST)
   {
#if !defined(PNG_NO_STDIO) && !defined(_WIN32_WCE)
      char msg[50];
      png_snprintf(msg, 50, "Unknown compression type %d", compression);
      png_warning(png_ptr, msg);
#else
      png_warning(png_ptr, "Unknown compression type");
#endif
   }

   /* We can't write the chunk until we find out how much data we have,
    * which means we need to run the compressor first and save the
    * output.  This shouldn't be a problem, as the vast majority of
    * comments should be reasonable, but we will set up an array of
    * malloc'd pointers to be sure.
    *
    * If we knew the application was well behaved, we could simplify this
    * greatly by assuming we can always malloc an output buffer large
    * enough to hold the compressed text ((1001 * text_len / 1000) + 12)
    * and malloc this directly.  The only time this would be a bad idea is
    * if we can't malloc more than 64K and we have 64K of random input
    * data, or if the input string is incredibly large (although this
    * wouldn't cause a failure, just a slowdown due to swapping).
    */

   /* Set up the compression buffers */
   png_ptr->zstream.avail_in = (uInt)text_len;
   png_ptr->zstream.next_in = (Bytef *)text;
   png_ptr->zstream.avail_out = (uInt)png_ptr->zbuf_size;
   png_ptr->zstream.next_out = (Bytef *)png_ptr->zbuf;

   /* This is the same compression loop as in png_write_row() */
   do
   {
      /* Compress the data */
      ret = deflate(&png_ptr->zstream, Z_NO_FLUSH);
      if (ret != Z_OK)
      {
         /* Error */
         if (png_ptr->zstream.msg != NULL)
            png_error(png_ptr, png_ptr->zstream.msg);
         else
            png_error(png_ptr, "zlib error");
      }
      /* Check to see if we need more room */
      if (!(png_ptr->zstream.avail_out))
      {
         /* Make sure the output array has room */
         if (comp->num_output_ptr >= comp->max_output_ptr)
         {
            int old_max;

            old_max = comp->max_output_ptr;
            comp->max_output_ptr = comp->num_output_ptr + 4;
            if (comp->output_ptr != NULL)
            {
               png_charpp old_ptr;

               old_ptr = comp->output_ptr;
               comp->output_ptr = (png_charpp)png_malloc(png_ptr,
                  (png_uint_32)
                  (comp->max_output_ptr * png_sizeof(png_charpp)));
               png_memcpy(comp->output_ptr, old_ptr, old_max
                  * png_sizeof(png_charp));
               png_free(png_ptr, old_ptr);
            }
            else
               comp->output_ptr = (png_charpp)png_malloc(png_ptr,
                  (png_uint_32)
                  (comp->max_output_ptr * png_sizeof(png_charp)));
         }

         /* Save the data */
         comp->output_ptr[comp->num_output_ptr] =
            (png_charp)png_malloc(png_ptr,
            (png_uint_32)png_ptr->zbuf_size);
         png_memcpy(comp->output_ptr[comp->num_output_ptr], png_ptr->zbuf,
            png_ptr->zbuf_size);
         comp->num_output_ptr++;

         /* and reset the buffer */
         png_ptr->zstream.avail_out = (uInt)png_ptr->zbuf_size;
         png_ptr->zstream.next_out = png_ptr->zbuf;
      }
   /* Continue until we don't have any more to compress */
   } while (png_ptr->zstream.avail_in);

   /* Finish the compression */
   do
   {
      /* Tell zlib we are finished */
      ret = deflate(&png_ptr->zstream, Z_FINISH);

      if (ret == Z_OK)
      {
         /* Check to see if we need more room */
         if (!(png_ptr->zstream.avail_out))
         {
            /* Check to make sure our output array has room */
            if (comp->num_output_ptr >= comp->max_output_ptr)
            {
               int old_max;

               old_max = comp->max_output_ptr;
               comp->max_output_ptr = comp->num_output_ptr + 4;
               if (comp->output_ptr != NULL)
               {
                  png_charpp old_ptr;

                  old_ptr = comp->output_ptr;
                  /* This could be optimized to realloc() */
                  comp->output_ptr = (png_charpp)png_malloc(png_ptr,
                     (png_uint_32)(comp->max_output_ptr *
                     png_sizeof(png_charp)));
                  png_memcpy(comp->output_ptr, old_ptr,
                     old_max * png_sizeof(png_charp));
                  png_free(png_ptr, old_ptr);
               }
               else
                  comp->output_ptr = (png_charpp)png_malloc(png_ptr,
                     (png_uint_32)(comp->max_output_ptr *
                     png_sizeof(png_charp)));
            }

            /* Save the data */
            comp->output_ptr[comp->num_output_ptr] =
               (png_charp)png_malloc(png_ptr,
               (png_uint_32)png_ptr->zbuf_size);
            png_memcpy(comp->output_ptr[comp->num_output_ptr], png_ptr->zbuf,
               png_ptr->zbuf_size);
            comp->num_output_ptr++;

            /* and reset the buffer pointers */
            png_ptr->zstream.avail_out = (uInt)png_ptr->zbuf_size;
            png_ptr->zstream.next_out = png_ptr->zbuf;
         }
      }
      else if (ret != Z_STREAM_END)
      {
         /* We got an error */
         if (png_ptr->zstream.msg != NULL)
            png_error(png_ptr, png_ptr->zstream.msg);
         else
            png_error(png_ptr, "zlib error");
      }
   } while (ret != Z_STREAM_END);

   /* Text length is number of buffers plus last buffer */
   text_len = png_ptr->zbuf_size * comp->num_output_ptr;
   if (png_ptr->zstream.avail_out < png_ptr->zbuf_size)
      text_len += png_ptr->zbuf_size - (png_size_t)png_ptr->zstream.avail_out;

   return((int)text_len);
}

/* Ship the compressed text out via chunk writes */
static void /* PRIVATE */
png_write_compressed_data_out(png_structp png_ptr, compression_state *comp)
{
   int i;

   /* Handle the no-compression case */
   if (comp->input)
   {
      png_write_chunk_data(png_ptr, (png_bytep)comp->input,
                            (png_size_t)comp->input_len);
      return;
   }

   /* Write saved output buffers, if any */
   for (i = 0; i < comp->num_output_ptr; i++)
   {
      png_write_chunk_data(png_ptr, (png_bytep)comp->output_ptr[i],
         (png_size_t)png_ptr->zbuf_size);
      png_free(png_ptr, comp->output_ptr[i]);
       comp->output_ptr[i]=NULL;
   }
   if (comp->max_output_ptr != 0)
      png_free(png_ptr, comp->output_ptr);
       comp->output_ptr=NULL;
   /* Write anything left in zbuf */
   if (png_ptr->zstream.avail_out < (png_uint_32)png_ptr->zbuf_size)
      png_write_chunk_data(png_ptr, png_ptr->zbuf,
         (png_size_t)(png_ptr->zbuf_size - png_ptr->zstream.avail_out));

   /* Reset zlib for another zTXt/iTXt or image data */
   deflateReset(&png_ptr->zstream);
   png_ptr->zstream.data_type = Z_BINARY;
}
#endif

/* Write the IHDR chunk, and update the png_struct with the necessary
 * information.  Note that the rest of this code depends upon this
 * information being correct.
 */
void /* PRIVATE */
png_write_IHDR(png_structp png_ptr, png_uint_32 width, png_uint_32 height,
   int bit_depth, int color_type, int compression_type, int filter_type,
   int interlace_type)
{
#ifdef PNG_USE_LOCAL_ARRAYS
   PNG_IHDR;
#endif
   int ret;

   png_byte buf[13]; /* Buffer to store the IHDR info */

   png_debug(1, "in png_write_IHDR");

   /* Check that we have valid input data from the application info */
   switch (color_type)
   {
      case PNG_COLOR_TYPE_GRAY:
         switch (bit_depth)
         {
            case 1:
            case 2:
            case 4:
            case 8:
            case 16: png_ptr->channels = 1; break;
            default: png_error(png_ptr, "Invalid bit depth for grayscale image");
         }
         break;
      case PNG_COLOR_TYPE_RGB:
         if (bit_depth != 8 && bit_depth != 16)
            png_error(png_ptr, "Invalid bit depth for RGB image");
         png_ptr->channels = 3;
         break;
      case PNG_COLOR_TYPE_PALETTE:
         switch (bit_depth)
         {
            case 1:
            case 2:
            case 4:
            case 8: png_ptr->channels = 1; break;
            default: png_error(png_ptr, "Invalid bit depth for paletted image");
         }
         break;
      case PNG_COLOR_TYPE_GRAY_ALPHA:
         if (bit_depth != 8 && bit_depth != 16)
            png_error(png_ptr, "Invalid bit depth for grayscale+alpha image");
         png_ptr->channels = 2;
         break;
      case PNG_COLOR_TYPE_RGB_ALPHA:
         if (bit_depth != 8 && bit_depth != 16)
            png_error(png_ptr, "Invalid bit depth for RGBA image");
         png_ptr->channels = 4;
         break;
      default:
         png_error(png_ptr, "Invalid image color type specified");
   }

   if (compression_type != PNG_COMPRESSION_TYPE_BASE)
   {
      png_warning(png_ptr, "Invalid compression type specified");
      compression_type = PNG_COMPRESSION_TYPE_BASE;
   }

   /* Write filter_method 64 (intrapixel differencing) only if
    * 1. Libpng was compiled with PNG_MNG_FEATURES_SUPPORTED and
    * 2. Libpng did not write a PNG signature (this filter_method is only
    *    used in PNG datastreams that are embedded in MNG datastreams) and
    * 3. The application called png_permit_mng_features with a mask that
    *    included PNG_FLAG_MNG_FILTER_64 and
    * 4. The filter_method is 64 and
    * 5. The color_type is RGB or RGBA
    */
   if (
#if defined(PNG_MNG_FEATURES_SUPPORTED)
      !((png_ptr->mng_features_permitted & PNG_FLAG_MNG_FILTER_64) &&
      ((png_ptr->mode&PNG_HAVE_PNG_SIGNATURE) == 0) &&
      (color_type == PNG_COLOR_TYPE_RGB ||
       color_type == PNG_COLOR_TYPE_RGB_ALPHA) &&
      (filter_type == PNG_INTRAPIXEL_DIFFERENCING)) &&
#endif
      filter_type != PNG_FILTER_TYPE_BASE)
   {
      png_warning(png_ptr, "Invalid filter type specified");
      filter_type = PNG_FILTER_TYPE_BASE;
   }

#ifdef PNG_WRITE_INTERLACING_SUPPORTED
   if (interlace_type != PNG_INTERLACE_NONE &&
      interlace_type != PNG_INTERLACE_ADAM7)
   {
      png_warning(png_ptr, "Invalid interlace type specified");
      interlace_type = PNG_INTERLACE_ADAM7;
   }
#else
   interlace_type=PNG_INTERLACE_NONE;
#endif

   /* Save the relevent information */
   png_ptr->bit_depth = (png_byte)bit_depth;
   png_ptr->color_type = (png_byte)color_type;
   png_ptr->interlaced = (png_byte)interlace_type;
#if defined(PNG_MNG_FEATURES_SUPPORTED)
   png_ptr->filter_type = (png_byte)filter_type;
#endif
   png_ptr->compression_type = (png_byte)compression_type;
   png_ptr->width = width;
   png_ptr->height = height;

   png_ptr->pixel_depth = (png_byte)(bit_depth * png_ptr->channels);
   png_ptr->rowbytes = PNG_ROWBYTES(png_ptr->pixel_depth, width);
   /* Set the usr info, so any transformations can modify it */
   png_ptr->usr_width = png_ptr->width;
   png_ptr->usr_bit_depth = png_ptr->bit_depth;
   png_ptr->usr_channels = png_ptr->channels;

   /* Pack the header information into the buffer */
   png_save_uint_32(buf, width);
   png_save_uint_32(buf + 4, height);
   buf[8] = (png_byte)bit_depth;
   buf[9] = (png_byte)color_type;
   buf[10] = (png_byte)compression_type;
   buf[11] = (png_byte)filter_type;
   buf[12] = (png_byte)interlace_type;

   /* Write the chunk */
   png_write_chunk(png_ptr, (png_bytep)png_IHDR, buf, (png_size_t)13);

   /* Initialize zlib with PNG info */
   png_ptr->zstream.zalloc = png_zalloc;
   png_ptr->zstream.zfree = png_zfree;
   png_ptr->zstream.opaque = (voidpf)png_ptr;
   if (!(png_ptr->do_filter))
   {
      if (png_ptr->color_type == PNG_COLOR_TYPE_PALETTE ||
         png_ptr->bit_depth < 8)
         png_ptr->do_filter = PNG_FILTER_NONE;
      else
         png_ptr->do_filter = PNG_ALL_FILTERS;
   }
   if (!(png_ptr->flags & PNG_FLAG_ZLIB_CUSTOM_STRATEGY))
   {
      if (png_ptr->do_filter != PNG_FILTER_NONE)
         png_ptr->zlib_strategy = Z_FILTERED;
      else
         png_ptr->zlib_strategy = Z_DEFAULT_STRATEGY;
   }
   if (!(png_ptr->flags & PNG_FLAG_ZLIB_CUSTOM_LEVEL))
      png_ptr->zlib_level = Z_DEFAULT_COMPRESSION;
   if (!(png_ptr->flags & PNG_FLAG_ZLIB_CUSTOM_MEM_LEVEL))
      png_ptr->zlib_mem_level = 8;
   if (!(png_ptr->flags & PNG_FLAG_ZLIB_CUSTOM_WINDOW_BITS))
      png_ptr->zlib_window_bits = 15;
   if (!(png_ptr->flags & PNG_FLAG_ZLIB_CUSTOM_METHOD))
      png_ptr->zlib_method = 8;
   ret = deflateInit2(&png_ptr->zstream, png_ptr->zlib_level,
         png_ptr->zlib_method, png_ptr->zlib_window_bits,
         png_ptr->zlib_mem_level, png_ptr->zlib_strategy);
   if (ret != Z_OK)
   {
      if (ret == Z_VERSION_ERROR) png_error(png_ptr,
          "zlib failed to initialize compressor -- version error");
      if (ret == Z_STREAM_ERROR) png_error(png_ptr,
           "zlib failed to initialize compressor -- stream error");
      if (ret == Z_MEM_ERROR) png_error(png_ptr,
           "zlib failed to initialize compressor -- mem error");
      png_error(png_ptr, "zlib failed to initialize compressor");
   }
   png_ptr->zstream.next_out = png_ptr->zbuf;
   png_ptr->zstream.avail_out = (uInt)png_ptr->zbuf_size;
   /* libpng is not interested in zstream.data_type */
   /* Set it to a predefined value, to avoid its evaluation inside zlib */
   png_ptr->zstream.data_type = Z_BINARY;

   png_ptr->mode = PNG_HAVE_IHDR;
}

/* Write the palette.  We are careful not to trust png_color to be in the
 * correct order for PNG, so people can redefine it to any convenient
 * structure.
 */
void /* PRIVATE */
png_write_PLTE(png_structp png_ptr, png_colorp palette, png_uint_32 num_pal)
{
#ifdef PNG_USE_LOCAL_ARRAYS
   PNG_PLTE;
#endif
   png_uint_32 i;
   png_colorp pal_ptr;
   png_byte buf[3];

   png_debug(1, "in png_write_PLTE");

   if ((
#if defined(PNG_MNG_FEATURES_SUPPORTED)
        !(png_ptr->mng_features_permitted & PNG_FLAG_MNG_EMPTY_PLTE) &&
#endif
        num_pal == 0) || num_pal > 256)
   {
     if (png_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
     {
        png_error(png_ptr, "Invalid number of colors in palette");
     }
     else
     {
        png_warning(png_ptr, "Invalid number of colors in palette");
        return;
     }
   }

   if (!(png_ptr->color_type&PNG_COLOR_MASK_COLOR))
   {
      png_warning(png_ptr,
        "Ignoring request to write a PLTE chunk in grayscale PNG");
      return;
   }

   png_ptr->num_palette = (png_uint_16)num_pal;
   png_debug1(3, "num_palette = %d", png_ptr->num_palette);

   png_write_chunk_start(png_ptr, (png_bytep)png_PLTE,
     (png_uint_32)(num_pal * 3));
#ifndef PNG_NO_POINTER_INDEXING
   for (i = 0, pal_ptr = palette; i < num_pal; i++, pal_ptr++)
   {
      buf[0] = pal_ptr->red;
      buf[1] = pal_ptr->green;
      buf[2] = pal_ptr->blue;
      png_write_chunk_data(png_ptr, buf, (png_size_t)3);
   }
#else
   /* This is a little slower but some buggy compilers need to do this instead */
   pal_ptr=palette;
   for (i = 0; i < num_pal; i++)
   {
      buf[0] = pal_ptr[i].red;
      buf[1] = pal_ptr[i].green;
      buf[2] = pal_ptr[i].blue;
      png_write_chunk_data(png_ptr, buf, (png_size_t)3);
   }
#endif
   png_write_chunk_end(png_ptr);
   png_ptr->mode |= PNG_HAVE_PLTE;
}

/* Write an IDAT chunk */
void /* PRIVATE */
png_write_IDAT(png_structp png_ptr, png_bytep data, png_size_t length)
{
#ifdef PNG_USE_LOCAL_ARRAYS
   PNG_IDAT;
#endif

   png_debug(1, "in png_write_IDAT");

   /* Optimize the CMF field in the zlib stream. */
   /* This hack of the zlib stream is compliant to the stream specification. */
   if (!(png_ptr->mode & PNG_HAVE_IDAT) &&
       png_ptr->compression_type == PNG_COMPRESSION_TYPE_BASE)
   {
      unsigned int z_cmf = data[0];  /* zlib compression method and flags */
      if ((z_cmf & 0x0f) == 8 && (z_cmf & 0xf0) <= 0x70)
      {
         /* Avoid memory underflows and multiplication overflows.
          *
          * The conditions below are practically always satisfied;
          * however, they still must be checked.
          */
         if (length >= 2 &&
             png_ptr->height < 16384 && png_ptr->width < 16384)
         {
            png_uint_32 uncompressed_idat_size = png_ptr->height *
               ((png_ptr->width *
               png_ptr->channels * png_ptr->bit_depth + 15) >> 3);
            unsigned int z_cinfo = z_cmf >> 4;
            unsigned int half_z_window_size = 1 << (z_cinfo + 7);
            while (uncompressed_idat_size <= half_z_window_size &&
                   half_z_window_size >= 256)
            {
               z_cinfo--;
               half_z_window_size >>= 1;
            }
            z_cmf = (z_cmf & 0x0f) | (z_cinfo << 4);
            if (data[0] != (png_byte)z_cmf)
            {
               data[0] = (png_byte)z_cmf;
               data[1] &= 0xe0;
               data[1] += (png_byte)(0x1f - ((z_cmf << 8) + data[1]) % 0x1f);
            }
         }
      }
      else
         png_error(png_ptr,
            "Invalid zlib compression method or flags in IDAT");
   }

   png_write_chunk(png_ptr, (png_bytep)png_IDAT, data, length);
   png_ptr->mode |= PNG_HAVE_IDAT;
}

/* Write an IEND chunk */
void /* PRIVATE */
png_write_IEND(png_structp png_ptr)
{
#ifdef PNG_USE_LOCAL_ARRAYS
   PNG_IEND;
#endif

   png_debug(1, "in png_write_IEND");

   png_write_chunk(png_ptr, (png_bytep)png_IEND, png_bytep_NULL,
     (png_size_t)0);
   png_ptr->mode |= PNG_HAVE_IEND;
}

#if defined(PNG_WRITE_gAMA_SUPPORTED)
/* Write a gAMA chunk */
#ifdef PNG_FLOATING_POINT_SUPPORTED
void /* PRIVATE */
png_write_gAMA(png_structp png_ptr, double file_gamma)
{
#ifdef PNG_USE_LOCAL_ARRAYS
   PNG_gAMA;
#endif
   png_uint_32 igamma;
   png_byte buf[4];

   png_debug(1, "in png_write_gAMA");

   /* file_gamma is saved in 1/100,000ths */
   igamma = (png_uint_32)(file_gamma * 100000.0 + 0.5);
   png_save_uint_32(buf, igamma);
   png_write_chunk(png_ptr, (png_bytep)png_gAMA, buf, (png_size_t)4);
}
#endif
#ifdef PNG_FIXED_POINT_SUPPORTED
void /* PRIVATE */
png_write_gAMA_fixed(png_structp png_ptr, png_fixed_point file_gamma)
{
#ifdef PNG_USE_LOCAL_ARRAYS
   PNG_gAMA;
#endif
   png_byte buf[4];

   png_debug(1, "in png_write_gAMA");

   /* file_gamma is saved in 1/100,000ths */
   png_save_uint_32(buf, (png_uint_32)file_gamma);
   png_write_chunk(png_ptr, (png_bytep)png_gAMA, buf, (png_size_t)4);
}
#endif
#endif

#if defined(PNG_WRITE_sRGB_SUPPORTED)
/* Write a sRGB chunk */
void /* PRIVATE */
png_write_sRGB(png_structp png_ptr, int srgb_intent)
{
#ifdef PNG_USE_LOCAL_ARRAYS
   PNG_sRGB;
#endif
   png_byte buf[1];

   png_debug(1, "in png_write_sRGB");

   if (srgb_intent >= PNG_sRGB_INTENT_LAST)
         png_warning(png_ptr,
            "Invalid sRGB rendering intent specified");
   buf[0]=(png_byte)srgb_intent;
   png_write_chunk(png_ptr, (png_bytep)png_sRGB, buf, (png_size_t)1);
}
#endif

#if defined(PNG_WRITE_iCCP_SUPPORTED)
/* Write an iCCP chunk */
void /* PRIVATE */
png_write_iCCP(png_structp png_ptr, png_charp name, int compression_type,
   png_charp profile, int profile_len)
{
#ifdef PNG_USE_LOCAL_ARRAYS
   PNG_iCCP;
#endif
   png_size_t name_len;
   png_charp new_name;
   compression_state comp;
   int embedded_profile_len = 0;

   png_debug(1, "in png_write_iCCP");

   comp.num_output_ptr = 0;
   comp.max_output_ptr = 0;
   comp.output_ptr = NULL;
   comp.input = NULL;
   comp.input_len = 0;

   if ((name_len = png_check_keyword(png_ptr, name,
      &new_name)) == 0)
      return;

   if (compression_type != PNG_COMPRESSION_TYPE_BASE)
      png_warning(png_ptr, "Unknown compression type in iCCP chunk");

   if (profile == NULL)
      profile_len = 0;

   if (profile_len > 3)
      embedded_profile_len =
          ((*( (png_bytep)profile    ))<<24) |
          ((*( (png_bytep)profile + 1))<<16) |
          ((*( (png_bytep)profile + 2))<< 8) |
          ((*( (png_bytep)profile + 3))    );

   if (embedded_profile_len < 0)
   {
      png_warning(png_ptr,
        "Embedded profile length in iCCP chunk is negative");
      png_free(png_ptr, new_name);
      return;
   }

   if (profile_len < embedded_profile_len)
   {
      png_warning(png_ptr,
        "Embedded profile length too large in iCCP chunk");
      png_free(png_ptr, new_name);
      return;
   }

   if (profile_len > embedded_profile_len)
   {
      png_warning(png_ptr,
        "Truncating profile to actual length in iCCP chunk");
      profile_len = embedded_profile_len;
   }

   if (profile_len)
      profile_len = png_text_compress(png_ptr, profile,
        (png_size_t)profile_len, PNG_COMPRESSION_TYPE_BASE, &comp);

   /* Make sure we include the NULL after the name and the compression type */
   png_write_chunk_start(png_ptr, (png_bytep)png_iCCP,
          (png_uint_32)(name_len + profile_len + 2));
   new_name[name_len + 1] = 0x00;
   png_write_chunk_data(png_ptr, (png_bytep)new_name,
     (png_size_t)(name_len + 2));

   if (profile_len)
      png_write_compressed_data_out(png_ptr, &comp);

   png_write_chunk_end(png_ptr);
   png_free(png_ptr, new_name);
}
#endif

#if defined(PNG_WRITE_sPLT_SUPPORTED)
/* Write a sPLT chunk */
void /* PRIVATE */
png_write_sPLT(png_structp png_ptr, png_sPLT_tp spalette)
{
#ifdef PNG_USE_LOCAL_ARRAYS
   PNG_sPLT;
#endif
   png_size_t name_len;
   png_charp new_name;
   png_byte entrybuf[10];
   int entry_size = (spalette->depth == 8 ? 6 : 10);
   int palette_size = entry_size * spalette->nentries;
   png_sPLT_entryp ep;
#ifdef PNG_NO_POINTER_INDEXING
   int i;
#endif

   png_debug(1, "in png_write_sPLT");

   if ((name_len = png_check_keyword(png_ptr,spalette->name, &new_name))==0)
      return;

   /* Make sure we include the NULL after the name */
   png_write_chunk_start(png_ptr, (png_bytep)png_sPLT,
     (png_uint_32)(name_len + 2 + palette_size));
   png_write_chunk_data(png_ptr, (png_bytep)new_name,
     (png_size_t)(name_len + 1));
   png_write_chunk_data(png_ptr, (png_bytep)&spalette->depth, (png_size_t)1);

   /* Loop through each palette entry, writing appropriately */
#ifndef PNG_NO_POINTER_INDEXING
   for (ep = spalette->entries; ep<spalette->entries + spalette->nentries; ep++)
   {
      if (spalette->depth == 8)
      {
          entrybuf[0] = (png_byte)ep->red;
          entrybuf[1] = (png_byte)ep->green;
          entrybuf[2] = (png_byte)ep->blue;
          entrybuf[3] = (png_byte)ep->alpha;
          png_save_uint_16(entrybuf + 4, ep->frequency);
      }
      else
      {
          png_save_uint_16(entrybuf + 0, ep->red);
          png_save_uint_16(entrybuf + 2, ep->green);
          png_save_uint_16(entrybuf + 4, ep->blue);
          png_save_uint_16(entrybuf + 6, ep->alpha);
          png_save_uint_16(entrybuf + 8, ep->frequency);
      }
      png_write_chunk_data(png_ptr, entrybuf, (png_size_t)entry_size);
   }
#else
   ep=spalette->entries;
   for (i=0; i>spalette->nentries; i++)
   {
      if (spalette->depth == 8)
      {
          entrybuf[0] = (png_byte)ep[i].red;
          entrybuf[1] = (png_byte)ep[i].green;
          entrybuf[2] = (png_byte)ep[i].blue;
          entrybuf[3] = (png_byte)ep[i].alpha;
          png_save_uint_16(entrybuf + 4, ep[i].frequency);
      }
      else
      {
          png_save_uint_16(entrybuf + 0, ep[i].red);
          png_save_uint_16(entrybuf + 2, ep[i].green);
          png_save_uint_16(entrybuf + 4, ep[i].blue);
          png_save_uint_16(entrybuf + 6, ep[i].alpha);
          png_save_uint_16(entrybuf + 8, ep[i].frequency);
      }
      png_write_chunk_data(png_ptr, entrybuf, (png_size_t)entry_size);
   }
#endif

   png_write_chunk_end(png_ptr);
   png_free(png_ptr, new_name);
}
#endif

#if defined(PNG_WRITE_sBIT_SUPPORTED)
/* Write the sBIT chunk */
void /* PRIVATE */
png_write_sBIT(png_structp png_ptr, png_color_8p sbit, int color_type)
{
#ifdef PNG_USE_LOCAL_ARRAYS
   PNG_sBIT;
#endif
   png_byte buf[4];
   png_size_t size;

   png_debug(1, "in png_write_sBIT");

   /* Make sure we don't depend upon the order of PNG_COLOR_8 */
   if (color_type & PNG_COLOR_MASK_COLOR)
   {
      png_byte maxbits;

      maxbits = (png_byte)(color_type==PNG_COLOR_TYPE_PALETTE ? 8 :
                png_ptr->usr_bit_depth);
      if (sbit->red == 0 || sbit->red > maxbits ||
          sbit->green == 0 || sbit->green > maxbits ||
          sbit->blue == 0 || sbit->blue > maxbits)
      {
         png_warning(png_ptr, "Invalid sBIT depth specified");
         return;
      }
      buf[0] = sbit->red;
      buf[1] = sbit->green;
      buf[2] = sbit->blue;
      size = 3;
   }
   else
   {
      if (sbit->gray == 0 || sbit->gray > png_ptr->usr_bit_depth)
      {
         png_warning(png_ptr, "Invalid sBIT depth specified");
         return;
      }
      buf[0] = sbit->gray;
      size = 1;
   }

   if (color_type & PNG_COLOR_MASK_ALPHA)
   {
      if (sbit->alpha == 0 || sbit->alpha > png_ptr->usr_bit_depth)
      {
         png_warning(png_ptr, "Invalid sBIT depth specified");
         return;
      }
      buf[size++] = sbit->alpha;
   }

   png_write_chunk(png_ptr, (png_bytep)png_sBIT, buf, size);
}
#endif

#if defined(PNG_WRITE_cHRM_SUPPORTED)
/* Write the cHRM chunk */
#ifdef PNG_FLOATING_POINT_SUPPORTED
void /* PRIVATE */
png_write_cHRM(png_structp png_ptr, double white_x, double white_y,
   double red_x, double red_y, double green_x, double green_y,
   double blue_x, double blue_y)
{
#ifdef PNG_USE_LOCAL_ARRAYS
   PNG_cHRM;
#endif
   png_byte buf[32];

   png_fixed_point int_white_x, int_white_y, int_red_x, int_red_y,
      int_green_x, int_green_y, int_blue_x, int_blue_y;

   png_debug(1, "in png_write_cHRM");

   int_white_x = (png_uint_32)(white_x * 100000.0 + 0.5);
   int_white_y = (png_uint_32)(white_y * 100000.0 + 0.5);
   int_red_x   = (png_uint_32)(red_x   * 100000.0 + 0.5);
   int_red_y   = (png_uint_32)(red_y   * 100000.0 + 0.5);
   int_green_x = (png_uint_32)(green_x * 100000.0 + 0.5);
   int_green_y = (png_uint_32)(green_y * 100000.0 + 0.5);
   int_blue_x  = (png_uint_32)(blue_x  * 100000.0 + 0.5);
   int_blue_y  = (png_uint_32)(blue_y  * 100000.0 + 0.5);

#if !defined(PNG_NO_CHECK_cHRM)
   if (png_check_cHRM_fixed(png_ptr, int_white_x, int_white_y,
      int_red_x, int_red_y, int_green_x, int_green_y, int_blue_x, int_blue_y))
#endif
   {
      /* Each value is saved in 1/100,000ths */

      png_save_uint_32(buf, int_white_x);
      png_save_uint_32(buf + 4, int_white_y);

      png_save_uint_32(buf + 8, int_red_x);
      png_save_uint_32(buf + 12, int_red_y);

      png_save_uint_32(buf + 16, int_green_x);
      png_save_uint_32(buf + 20, int_green_y);

      png_save_uint_32(buf + 24, int_blue_