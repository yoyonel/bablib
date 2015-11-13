
/* pngrtran.c - transforms the data in a row for PNG readers
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
 * This file contains functions optionally called by an application
 * in order to tell libpng how to handle data when reading a PNG.
 * Transformations that are used in both reading and writing are
 * in pngtrans.c.
 */

#define PNG_INTERNAL
#include "png.h"
#if defined(PNG_READ_SUPPORTED)

/* Set the action on getting a CRC error for an ancillary or critical chunk. */
void PNGAPI
png_set_crc_action(png_structp png_ptr, int crit_action, int ancil_action)
{
   png_debug(1, "in png_set_crc_action");
   /* Tell libpng how we react to CRC errors in critical chunks */
   if (png_ptr == NULL)
      return;
   switch (crit_action)
   {
      case PNG_CRC_NO_CHANGE:                        /* Leave setting as is */
         break;

      case PNG_CRC_WARN_USE:                               /* Warn/use data */
         png_ptr->flags &= ~PNG_FLAG_CRC_CRITICAL_MASK;
         png_ptr->flags |= PNG_FLAG_CRC_CRITICAL_USE;
         break;

      case PNG_CRC_QUIET_USE:                             /* Quiet/use data */
         png_ptr->flags &= ~PNG_FLAG_CRC_CRITICAL_MASK;
         png_ptr->flags |= PNG_FLAG_CRC_CRITICAL_USE |
                           PNG_FLAG_CRC_CRITICAL_IGNORE;
         break;

      case PNG_CRC_WARN_DISCARD:    /* Not a valid action for critical data */
         png_warning(png_ptr,
            "Can't discard critical data on CRC error.");
      case PNG_CRC_ERROR_QUIT:                                /* Error/quit */

      case PNG_CRC_DEFAULT:
      default:
         png_ptr->flags &= ~PNG_FLAG_CRC_CRITICAL_MASK;
         break;
   }

   switch (ancil_action)
   {
      case PNG_CRC_NO_CHANGE:                       /* Leave setting as is */
         break;

      case PNG_CRC_WARN_USE:                              /* Warn/use data */
         png_ptr->flags &= ~PNG_FLAG_CRC_ANCILLARY_MASK;
         png_ptr->flags |= PNG_FLAG_CRC_ANCILLARY_USE;
         break;

      case PNG_CRC_QUIET_USE:                            /* Quiet/use data */
         png_ptr->flags &= ~PNG_FLAG_CRC_ANCILLARY_MASK;
         png_ptr->flags |= PNG_FLAG_CRC_ANCILLARY_USE |
                           PNG_FLAG_CRC_ANCILLARY_NOWARN;
         break;

      case PNG_CRC_ERROR_QUIT:                               /* Error/quit */
         png_ptr->flags &= ~PNG_FLAG_CRC_ANCILLARY_MASK;
         png_ptr->flags |= PNG_FLAG_CRC_ANCILLARY_NOWARN;
         break;

      case PNG_CRC_WARN_DISCARD:                      /* Warn/discard data */

      case PNG_CRC_DEFAULT:
      default:
         png_ptr->flags &= ~PNG_FLAG_CRC_ANCILLARY_MASK;
         break;
   }
}

#if defined(PNG_READ_BACKGROUND_SUPPORTED) && \
    defined(PNG_FLOATING_POINT_SUPPORTED)
/* Handle alpha and tRNS via a background color */
void PNGAPI
png_set_background(png_structp png_ptr,
   png_color_16p background_color, int background_gamma_code,
   int need_expand, double background_gamma)
{
   png_debug(1, "in png_set_background");
   if (png_ptr == NULL)
      return;
   if (background_gamma_code == PNG_BACKGROUND_GAMMA_UNKNOWN)
   {
      png_warning(png_ptr, "Application must supply a known background gamma");
      return;
   }

   png_ptr->transformations |= PNG_BACKGROUND;
   png_memcpy(&(png_ptr->background), background_color,
      png_sizeof(png_color_16));
   png_ptr->background_gamma = (float)background_gamma;
   png_ptr->background_gamma_type = (png_byte)(background_gamma_code);
   png_ptr->transformations |= (need_expand ? PNG_BACKGROUND_EXPAND : 0);
}
#endif

#if defined(PNG_READ_16_TO_8_SUPPORTED)
/* Strip 16 bit depth files to 8 bit depth */
void PNGAPI
png_set_strip_16(png_structp png_ptr)
{
   png_debug(1, "in png_set_strip_16");
   if (png_ptr == NULL)
      return;
   png_ptr->transformations |= PNG_16_TO_8;
}
#endif

#if defined(PNG_READ_STRIP_ALPHA_SUPPORTED)
void PNGAPI
png_set_strip_alpha(png_structp png_ptr)
{
   png_debug(1, "in png_set_strip_alpha");
   if (png_ptr == NULL)
      return;
   png_ptr->flags |= PNG_FLAG_STRIP_ALPHA;
}
#endif

#if defined(PNG_READ_DITHER_SUPPORTED)
/* Dither file to 8 bit.  Supply a palette, the current number
 * of elements in the palette, the maximum number of elements
 * allowed, and a histogram if possible.  If the current number
 * of colors is greater then the maximum number, the palette will be
 * modified to fit in the maximum number.  "full_dither" indicates
 * whether we need a dithering cube set up for RGB images, or if we
 * simply are reducing the number of colors in a paletted image.
 */

typedef struct png_dsort_struct
{
   struct png_dsort_struct FAR * next;
   png_byte left;
   png_byte right;
} png_dsort;
typedef png_dsort FAR *       png_dsortp;
typedef png_dsort FAR * FAR * png_dsortpp;

void PNGAPI
png_set_dither(png_structp png_ptr, png_colorp palette,
   int num_palette, int maximum_colors, png_uint_16p histogram,
   int full_dither)
{
   png_debug(1, "in png_set_dither");
   if (png_ptr == NULL)
      return;
   png_ptr->transformations |= PNG_DITHER;

   if (!full_dither)
   {
      int i;

      png_ptr->dither_index = (png_bytep)png_malloc(png_ptr,
         (png_uint_32)(num_palette * png_sizeof(png_byte)));
      for (i = 0; i < num_palette; i++)
         png_ptr->dither_index[i] = (png_byte)i;
   }

   if (num_palette > maximum_colors)
   {
      if (histogram != NULL)
      {
         /* This is easy enough, just throw out the least used colors.
          * Perhaps not the best solution, but good enough.
          */

         int i;

         /* Initialize an array to sort colors */
         png_ptr->dither_sort = (png_bytep)png_malloc(png_ptr,
            (png_uint_32)(num_palette * png_sizeof(png_byte)));

         /* Initialize the dither_sort array */
         for (i = 0; i < num_palette; i++)
            png_ptr->dither_sort[i] = (png_byte)i;

         /* Find the least used palette entries by starting a
          * bubble sort, and running it until we have sorted
          * out enough colors.  Note that we don't care about
          * sorting all the colors, just finding which are
          * least used.
          */

         for (i = num_palette - 1; i >= maximum_colors; i--)
         {
            int done; /* To stop early if the list is pre-sorted */
            int j;

            done = 1;
            for (j = 0; j < i; j++)
            {
               if (histogram[png_ptr->dither_sort[j]]
                   < histogram[png_ptr->dither_sort[j + 1]])
               {
                  png_byte t;

                  t = png_ptr->dither_sort[j];
                  png_ptr->dither_sort[j] = png_ptr->dither_sort[j + 1];
                  png_ptr->dither_sort[j + 1] = t;
                  done = 0;
               }
            }
            if (done)
               break;
         }

         /* Swap the palette around, and set up a table, if necessary */
         if (full_dither)
         {
            int j = num_palette;

            /* Put all the useful colors within the max, but don't
             * move the others.
             */
            for (i = 0; i < maximum_colors; i++)
            {
               if ((int)png_ptr->dither_sort[i] >= maximum_colors)
               {
                  do
                     j--;
                  while ((int)png_ptr->dither_sort[j] >= maximum_colors);
                  palette[i] = palette[j];
               }
            }
         }
         else
         {
            int j = num_palette;

            /* Move all the used colors inside the max limit, and
             * develop a translation table.
             */
            for (i = 0; i < maximum_colors; i++)
            {
               /* Only move the colors we need to */
               if ((int)png_ptr->dither_sort[i] >= maximum_colors)
               {
                  png_color tmp_color;

                  do
                     j--;
                  while ((int)png_ptr->dither_sort[j] >= maximum_colors);

                  tmp_color = palette[j];
                  palette[j] = palette[i];
                  palette[i] = tmp_color;
                  /* Indicate where the color went */
                  png_ptr->dither_index[j] = (png_byte)i;
                  png_ptr->dither_index[i] = (png_byte)j;
               }
            }

            /* Find closest color for those colors we are not using */
            for (i = 0; i < num_palette; i++)
            {
               if ((int)png_ptr->dither_index[i] >= maximum_colors)
               {
                  int min_d, k, min_k, d_index;

                  /* Find the closest color to one we threw out */
                  d_index = png_ptr->dither_index[i];
                  min_d = PNG_COLOR_DIST(palette[d_index], palette[0]);
                  for (k = 1, min_k = 0; k < maximum_colors; k++)
                  {
                     int d;

                     d = PNG_COLOR_DIST(palette[d_index], palette[k]);

                     if (d < min_d)
                     {
                        min_d = d;
                        min_k = k;
                     }
                  }
                  /* Point to closest color */
                  png_ptr->dither_index[i] = (png_byte)min_k;
               }
            }
         }
         png_free(png_ptr, png_ptr->dither_sort);
         png_ptr->dither_sort = NULL;
      }
      else
      {
         /* This is much harder to do simply (and quickly).  Perhaps
          * we need to go through a median cut routine, but those
          * don't always behave themselves with only a few colors
          * as input.  So we will just find the closest two colors,
          * and throw out one of them (chosen somewhat randomly).
          * [We don't understand this at all, so if someone wants to
          *  work on improving it, be our guest - AED, GRP]
          */
         int i;
         int max_d;
         int num_new_palette;
         png_dsortp t;
         png_dsortpp hash;

         t = NULL;

         /* Initialize palette index arrays */
         png_ptr->index_to_palette = (png_bytep)png_malloc(png_ptr,
            (png_uint_32)(num_palette * png_sizeof(png_byte)));
         png_ptr->palette_to_index = (png_bytep)png_malloc(png_ptr,
            (png_uint_32)(num_palette * png_sizeof(png_byte)));

         /* Initialize the sort array */
         for (i = 0; i < num_palette; i++)
         {
            png_ptr->index_to_palette[i] = (png_byte)i;
            png_ptr->palette_to_index[i] = (png_byte)i;
         }

         hash = (png_dsortpp)png_malloc(png_ptr, (png_uint_32)(769 *
            png_sizeof(png_dsortp)));
         png_memset(hash, 0, 769 * png_sizeof(png_dsortp));

         num_new_palette = num_palette;

         /* Initial wild guess at how far apart the farthest pixel
          * pair we will be eliminating will be.  Larger
          * numbers mean more areas will be allocated, Smaller
          * numbers run the risk of not saving enough data, and
          * having to do this all over again.
          *
          * I have not done extensive checking on this number.
          */
         max_d = 96;

         while (num_new_palette > maximum_colors)
         {
            for (i = 0; i < num_new_palette - 1; i++)
            {
               int j;

               for (j = i + 1; j < num_new_palette; j++)
               {
                  int d;

                  d = PNG_COLOR_DIST(palette[i], palette[j]);

                  if (d <= max_d)
                  {

                     t = (png_dsortp)png_malloc_warn(png_ptr,
                         (png_uint_32)(png_sizeof(png_dsort)));
                     if (t == NULL)
                         break;
                     t->next = hash[d];
                     t->left = (png_byte)i;
                     t->right = (png_byte)j;
                     hash[d] = t;
                  }
               }
               if (t == NULL)
                  break;
            }

            if (t != NULL)
            for (i = 0; i <= max_d; i++)
            {
               if (hash[i] != NULL)
               {
                  png_dsortp p;

                  for (p = hash[i]; p; p = p->next)
                  {
                     if ((int)png_ptr->index_to_palette[p->left]
                        < num_new_palette &&
                        (int)png_ptr->index_to_palette[p->right]
                        < num_new_palette)
                     {
                        int j, next_j;

                        if (num_new_palette & 0x01)
                        {
                           j = p->left;
                           next_j = p->right;
                        }
                        else
                        {
                           j = p->right;
                           next_j = p->left;
                        }

                        num_new_palette--;
                        palette[png_ptr->index_to_palette[j]]
                          = palette[num_new_palette];
                        if (!full_dither)
                        {
                           int k;

                           for (k = 0; k < num_palette; k++)
                           {
                              if (png_ptr->dither_index[k] ==
                                 png_ptr->index_to_palette[j])
                                 png_ptr->dither_index[k] =
                                    png_ptr->index_to_palette[next_j];
                              if ((int)png_ptr->dither_index[k] ==
                                 num_new_palette)
                                 png_ptr->dither_index[k] =
                                    png_ptr->index_to_palette[j];
                           }
                        }

                        png_ptr->index_to_palette[png_ptr->palette_to_index
                           [num_new_palette]] = png_ptr->index_to_palette[j];
                        png_ptr->palette_to_index[png_ptr->index_to_palette[j]]
                           = png_ptr->palette_to_index[num_new_palette];

                        png_ptr->index_to_palette[j] = (png_byte)num_new_palette;
                        png_ptr->palette_to_index[num_new_palette] = (png_byte)j;
                     }
                     if (num_new_palette <= maximum_colors)
                        break;
                  }
                  if (num_new_palette <= maximum_colors)
                     break;
               }
            }

            for (i = 0; i < 769; i++)
            {
               if (hash[i] != NULL)
               {
                  png_dsortp p = hash[i];
                  while (p)
                  {
                     t = p->next;
                     png_free(png_ptr, p);
                     p = t;
                  }
               }
               hash[i] = 0;
            }
            max_d += 96;
         }
         png_free(png_ptr, hash);
         png_free(png_ptr, png_ptr->palette_to_index);
         png_free(png_ptr, png_ptr->index_to_palette);
         png_ptr->palette_to_index = NULL;
         png_ptr->index_to_palette = NULL;
      }
      num_palette = maximum_colors;
   }
   if (png_ptr->palette == NULL)
   {
      png_ptr->palette = palette;
   }
   png_ptr->num_palette = (png_uint_16)num_palette;

   if (full_dither)
   {
      int i;
      png_bytep distance;
      int total_bits = PNG_DITHER_RED_BITS + PNG_DITHER_GREEN_BITS +
         PNG_DITHER_BLUE_BITS;
      int num_red = (1 << PNG_DITHER_RED_BITS);
      int num_green = (1 << PNG_DITHER_GREEN_BITS);
      int num_blue = (1 << PNG_DITHER_BLUE_BITS);
      png_size_t num_entries = ((png_size_t)1 << total_bits);
      png_ptr->palette_lookup = (png_bytep )png_malloc(png_ptr,
         (png_uint_32)(num_entries * png_sizeof(png_byte)));
      png_memset(png_ptr->palette_lookup, 0, num_entries *
         png_sizeof(png_byte));

      distance = (png_bytep)png_malloc(png_ptr, (png_uint_32)(num_entries *
         png_sizeof(png_byte)));

      png_memset(distance, 0xff, num_entries * png_sizeof(png_byte));

      for (i = 0; i < num_palette; i++)
      {
         int ir, ig, ib;
         int r = (palette[i].red >> (8 - PNG_DITHER_RED_BITS));
         int g = (palette[i].green >> (8 - PNG_DITHER_GREEN_BITS));
         int b = (palette[i].blue >> (8 - PNG_DITHER_BLUE_BITS));

         for (ir = 0; ir < num_red; ir++)
         {
            /* int dr = abs(ir - r); */
            int dr = ((ir > r) ? ir - r : r - ir);
            int index_r = (ir << (PNG_DITHER_BLUE_BITS + PNG_DITHER_GREEN_BITS));

            for (ig = 0; ig < num_green; ig++)
            {
               /* int dg = abs(ig - g); */
               int dg = ((ig > g) ? ig - g : g - ig);
               int dt = dr + dg;
               int dm = ((dr > dg) ? dr : dg);
               int index_g = index_r | (ig << PNG_DITHER_BLUE_BITS);

               for (ib = 0; ib < num_blue; ib++)
               {
                  int d_index = index_g | ib;
                  /* int db = abs(ib - b); */
                  int db = ((ib > b) ? ib - b : b - ib);
                  int dmax = ((dm > db) ? dm : db);
                  int d = dmax + dt + db;

                  if (d < (int)distance[d_index])
                  {
                     distance[d_index] = (png_byte)d;
                     png_ptr->palette_lookup[d_index] = (png_byte)i;
                  }
               }
            }
         }
      }

      png_free(png_ptr, distance);
   }
}
#endif

#if defined(PNG_READ_GAMMA_SUPPORTED) && defined(PNG_FLOATING_POINT_SUPPORTED)
/* Transform the image from the file_gamma to the screen_gamma.  We
 * only do transformations on images where the file_gamma and screen_gamma
 * are not close reciprocals, otherwise it slows things down slightly, and
 * also needlessly introduces small errors.
 *
 * We will turn off gamma transformation later if no semitransparent entries
 * are present in the tRNS array for palette images.  We can't do it here
 * because we don't necessarily have the tRNS chunk yet.
 */
void PNGAPI
png_set_gamma(png_structp png_ptr, double scrn_gamma, double file_gamma)
{
   png_debug(1, "in png_set_gamma");
   if (png_ptr == NULL)
      return;
   if ((fabs(scrn_gamma * file_gamma - 1.0) > PNG_GAMMA_THRESHOLD) ||
       (png_ptr->color_type & PNG_COLOR_MASK_ALPHA) ||
       (png_ptr->color_type == PNG_COLOR_TYPE_PALETTE))
     png_ptr->transformations |= PNG_GAMMA;
   png_ptr->gamma = (float)file_gamma;
   png_ptr->screen_gamma = (float)scrn_gamma;
}
#endif

#if defined(PNG_READ_EXPAND_SUPPORTED)
/* Expand paletted images to RGB, expand grayscale images of
 * less than 8-bit depth to 8-bit depth, and expand tRNS chunks
 * to alpha channels.
 */
void PNGAPI
png_set_expand(png_structp png_ptr)
{
   png_debug(1, "in png_set_expand");
   if (png_ptr == NULL)
      return;
   png_ptr->transformations |= (PNG_EXPAND | PNG_EXPAND_tRNS);
   png_ptr->flags &= ~PNG_FLAG_ROW_INIT;
}

/* GRR 19990627:  the following three functions currently are identical
 *  to png_set_expand().  However, it is entirely reasonable that someone
 *  might wish to expand an indexed image to RGB but *not* expand a single,
 *  fully transparent palette entry to a full alpha channel--perhaps instead
 *  convert tRNS to the grayscale/RGB format (16-bit RGB value), or replace
 *  the transparent color with a particular RGB value, or drop tRNS entirely.
 *  IOW, a future version of the library may make the transformations flag
 *  a bit more fine-grained, with separate bits for each of these three
 *  functions.
 *
 *  More to the point, these functions make it obvious what libpng will be
 *  doing, whereas "expand" can (and does) mean any number of things.
 *
 *  GRP 20060307: In libpng-1.4.0, png_set_gray_1_2_4_to_8() was modified
 *  to expand only the sample depth but not to expand the tRNS to alpha.
 */

/* Expand paletted images to RGB. */
void PNGAPI
png_set_palette_to_rgb(png_structp png_ptr)
{
   png_debug(1, "in png_set_palette_to_rgb");
   if (png_ptr == NULL)
      return;
   png_ptr->transformations |= (PNG_EXPAND | PNG_EXPAND_tRNS);
   png_ptr->flags &= ~PNG_FLAG_ROW_INIT;
}

#if !defined(PNG_1_0_X)
/* Expand grayscale images of less than 8-bit depth to 8 bits. */
void PNGAPI
png_set_expand_gray_1_2_4_to_8(png_structp png_ptr)
{
   png_debug(1, "in png_set_expand_gray_1_2_4_to_8");
   if (png_ptr == NULL)
      return;
   png_ptr->transformations |= PNG_EXPAND;
   png_ptr->flags &= ~PNG_FLAG_ROW_INIT;
}
#endif

#if defined(PNG_1_0_X) || defined(PNG_1_2_X)
/* Expand grayscale images of less than 8-bit depth to 8 bits. */
/* Deprecated as of libpng-1.2.9 */
void PNGAPI
png_set_gray_1_2_4_to_8(png_structp png_ptr)
{
   png_debug(1, "in png_set_gray_1_2_4_to_8");
   if (png_ptr == NULL)
      return;
   png_ptr->transformations |= (PNG_EXPAND | PNG_EXPAND_tRNS);
}
#endif


/* Expand tRNS chunks to alpha channels. */
void PNGAPI
png_set_tRNS_to_alpha(png_structp png_ptr)
{
   png_debug(1, "in png_set_tRNS_to_alpha");
   png_ptr->transformations |= (PNG_EXPAND | PNG_EXPAND_tRNS);
   png_ptr->flags &= ~PNG_FLAG_ROW_INIT;
}
#endif /* defined(PNG_READ_EXPAND_SUPPORTED) */

#if defined(PNG_READ_GRAY_TO_RGB_SUPPORTED)
void PNGAPI
png_set_gray_to_rgb(png_structp png_ptr)
{
   png_debug(1, "in png_set_gray_to_rgb");
   png_ptr->transformations |= PNG_GRAY_TO_RGB;
   png_ptr->flags &= ~PNG_FLAG_ROW_INIT;
}
#endif

#if defined(PNG_READ_RGB_TO_GRAY_SUPPORTED)
#if defined(PNG_FLOATING_POINT_SUPPORTED)
/* Convert a RGB image to a grayscale of the same width.  This allows us,
 * for example, to convert a 24 bpp RGB image into an 8 bpp grayscale image.
 */

void PNGAPI
png_set_rgb_to_gray(png_structp png_ptr, int error_action, double red,
   double green)
{
   int red_fixed = (int)((float)red*100000.0 + 0.5);
   int green_fixed = (int)((float)green*100000.0 + 0.5);
   if (png_ptr == NULL)
      return;
   png_set_rgb_to_gray_fixed(png_ptr, error_action, red_fixed, green_fixed);
}
#endif

void PNGAPI
png_set_rgb_to_gray_fixed(png_structp png_ptr, int error_action,
   png_fixed_point red, png_fixed_point green)
{
   png_debug(1, "in png_set_rgb_to_gray");
   if (png_ptr == NULL)
      return;
   switch(error_action)
   {
      case 1: png_ptr->transformations |= PNG_RGB_TO_GRAY;
              break;

      case 2: png_ptr->transformations |= PNG_RGB_TO_GRAY_WARN;
              break;

      case 3: png_ptr->transformations |= PNG_RGB_TO_GRAY_ERR;
   }
   if (png_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
#if defined(PNG_READ_EXPAND_SUPPORTED)
      png_ptr->transformations |= PNG_EXPAND;
#else
   {
      png_warning(png_ptr,
        "Cannot do RGB_TO_GRAY without EXPAND_SUPPORTED.");
      png_ptr->transformations &= ~PNG_RGB_TO_GRAY;
   }
#endif
   {
      png_uint_16 red_int, green_int;
      if (red < 0 || green < 0)
      {
         red_int   =  6968; /* .212671 * 32768 + .5 */
         green_int = 23434; /* .715160 * 32768 + .5 */
      }
      else if (red + green < 100000L)
      {
         red_int = (png_uint_16)(((png_uint_32)red*32768L)/100000L);
         green_int = (png_uint_16)(((png_uint_32)green*32768L)/100000L);
      }
      else
      {
         png_warning(png_ptr, "ignoring out of range rgb_to_gray coefficients");
         red_int   =  6968;
         green_int = 23434;
      }
      png_ptr->rgb_to_gray_red_coeff   = red_int;
      png_ptr->rgb_to_gray_green_coeff = green_int;
      png_ptr->rgb_to_gray_blue_coeff  =
         (png_uint_16)(32768 - red_int - green_int);
   }
}
#endif

#if defined(PNG_READ_USER_TRANSFORM_SUPPORTED) || \
    defined(PNG_LEGACY_SUPPORTED) || \
    defined(PNG_WRITE_USER_TRANSFORM_SUPPORTED)
void PNGAPI
png_set_read_user_transform_fn(png_structp png_ptr, png_user_transform_ptr
   read_user_transform_fn)
{
   png_debug(1, "in png_set_read_user_transform_fn");
   if (png_ptr == NULL)
      return;
#if defined(PNG_READ_USER_TRANSFORM_SUPPORTED)
   png_ptr->transformations |= PNG_USER_TRANSFORM;
   png_ptr->read_user_transform_fn = read_user_transform_fn;
#endif
#ifdef PNG_LEGACY_SUPPORTED
   if (read_user_transform_fn)
      png_warning(png_ptr,
        "This version of libpng does not support user transforms");
#endif
}
#endif

/* Initialize everything needed for the read.  This includes modifying
 * the palette.
 */
void /* PRIVATE */
png_init_read_transformations(png_structp png_ptr)
{
   png_debug(1, "in png_init_read_transformations");
#if defined(PNG_USELESS_TESTS_SUPPORTED)
   if (png_ptr != NULL)
#endif
  {
#if defined(PNG_READ_BACKGROUND_SUPPORTED) || defined(PNG_READ_SHIFT_SUPPORTED) \
 || defined(PNG_READ_GAMMA_SUPPORTED)
   int color_type = png_ptr->color_type;
#endif

#if defined(PNG_READ_EXPAND_SUPPORTED) && defined(PNG_READ_BACKGROUND_SUPPORTED)

#if defined(PNG_READ_GRAY_TO_RGB_SUPPORTED)
   /* Detect gray background and attempt to enable optimization
    * for gray --> RGB case
    *
    * Note:  if PNG_BACKGROUND_EXPAND is set and color_type is either RGB or
    * RGB_ALPHA (in which case need_expand is superfluous anyway), the
    * background color might actually be gray yet not be flagged as such.
    * This is not a problem for the current code, which uses
    * PNG_BACKGROUND_IS_GRAY only to decide when to do the
    * png_do_gray_to_rgb() transformation.
    */
   if ((png_ptr->transformations & PNG_BACKGROUND_EXPAND) &&
       !(color_type & PNG_COLOR_MASK_COLOR))
   {
          png_ptr->mode |= PNG_BACKGROUND_IS_GRAY;
   } else if ((png_ptr->transformations & PNG_BACKGROUND) &&
              !(png_ptr->transformations & PNG_BACKGROUND_EXPAND) &&
              (png_ptr->transformations & PNG_GRAY_TO_RGB) &&
              png_ptr->background.red == png_ptr->background.green &&
              png_ptr->background.red == png_ptr->background.blue)
   {
          png_ptr->mode |= PNG_BACKGROUND_IS_GRAY;
          png_ptr->background.gray = png_ptr->background.red;
   }
#endif

   if ((png_ptr->transformations & PNG_BACKGROUND_EXPAND) &&
       (png_ptr->transformations & PNG_EXPAND))
   {
      if (!(color_type & PNG_COLOR_MASK_COLOR))  /* i.e., GRAY or GRAY_ALPHA */
      {
         /* Expand background and tRNS chunks */
         switch (png_ptr->bit_depth)
         {
            case 1:
               png_ptr->background.gray *= (png_uint_16)0xff;
               png_ptr->background.red = png_ptr->background.green
                 =  png_ptr->background.blue = png_ptr->background.gray;
               if (!(png_ptr->transformations & PNG_EXPAND_tRNS))
               {
                 png_ptr->trans_values.gray *= (png_uint_16)0xff;
                 png_ptr->trans_values.red = png_ptr->trans_values.green
                   = png_ptr->trans_values.blue = png_ptr->trans_values.gray;
               }
               break;

            case 2:
               png_ptr->background.gray *= (png_uint_16)0x55;
               png_ptr->background.red = png_ptr->background.green
                 = png_ptr->background.blue = png_ptr->background.gray;
               if (!(png_ptr->transformations & PNG_EXPAND_tRNS))
               {
                 png_ptr->trans_values.gray *= (png_uint_16)0x55;
                 png_ptr->trans_values.red = png_ptr->trans_values.green
                   = png_ptr->trans_values.blue = png_ptr->trans_values.gray;
               }
               break;

            case 4:
               png_ptr->background.gray *= (png_uint_16)0x11;
               png_ptr->background.red = png_ptr->background.green
                 = png_ptr->background.blue = png_ptr->background.gray;
               if (!(png_ptr->transformations & PNG_EXPAND_tRNS))
               {
                 png_ptr->trans_values.gray *= (png_uint_16)0x11;
                 png_ptr->trans_values.red = png_ptr->trans_values.green
                   = png_ptr->trans_values.blue = png_ptr->trans_values.gray;
               }
               break;

            case 8:

            case 16:
               png_ptr->background.red = png_ptr->background.green
                 = png_ptr->background.blue = png_ptr->background.gray;
               break;
         }
      }
      else if (color_type == PNG_COLOR_TYPE_PALETTE)
      {
         png_ptr->background.red   =
            png_ptr->palette[png_ptr->background.index].red;
         png_ptr->background.green =
            png_ptr->palette[png_ptr->background.index].green;
         png_ptr->background.blue  =
            png_ptr->palette[png_ptr->background.index].blue;

#if defined(PNG_READ_INVERT_ALPHA_SUPPORTED)
        if (png_ptr->transformations & PNG_INVERT_ALPHA)
        {
#if defined(PNG_READ_EXPAND_SUPPORTED)
           if (!(png_ptr->transformations & PNG_EXPAND_tRNS))
#endif
           {
           /* Invert the alpha channel (in tRNS) unless the pixels are
            * going to be expanded, in which case leave it for later
            */
              int i, istop;
              istop=(int)png_ptr->num_trans;
              for (i=0; i<istop; i++)
                 png_ptr->trans[i] = (png_byte)(255 - png_ptr->trans[i]);
           }
        }
#endif

      }
   }
#endif

#if defined(PNG_READ_BACKGROUND_SUPPORTED) && defined(PNG_READ_GAMMA_SUPPORTED)
   png_ptr->background_1 = png_ptr->background;
#endif
#if defined(PNG_READ_GAMMA_SUPPORTED) && defined(PNG_FLOATING_POINT_SUPPORTED)

   if ((color_type == PNG_COLOR_TYPE_PALETTE && png_ptr->num_trans != 0)
       && (fabs(png_ptr->screen_gamma * png_ptr->gamma - 1.0)
         < PNG_GAMMA_THRESHOLD))
   {
    int i, k;
    k=0;
    for (i=0; i<png_ptr->num_trans; i++)
    {
      if (png_ptr->trans[i] != 0 && png_ptr->trans[i] != 0xff)
        k=1; /* Partial transparency is present */
    }
    if (k == 0)
      png_ptr->transformations &= ~PNG_GAMMA;
   }

   if ((png_ptr->transformations & (PNG_GAMMA | PNG_RGB_TO_GRAY)) &&
        png_ptr->gamma != 0.0)
   {
      png_build_gamma_table(png_ptr);
#if defined(PNG_READ_BACKGROUND_SUPPORTED)
      if (png_ptr->transformations & PNG_BACKGROUND)
      {
         if (color_type == PNG_COLOR_TYPE_PALETTE)
         {
           /* Could skip if no transparency */
            png_color back, back_1;
            png_colorp palette = png_ptr->palette;
            int num_palette = png_ptr->num_palette;
            int i;
            if (png_ptr->background_gamma_type == PNG_BACKGROUND_GAMMA_FILE)
            {
               back.red = png_ptr->gamma_table[png_ptr->background.red];
               back.green = png_ptr->gamma_table[png_ptr->background.green];
               back.blue = png_ptr->gamma_table[png_ptr->background.blue];

               back_1.red = png_ptr->gamma_to_1[png_ptr->background.red];
               back_1.green = png_ptr->gamma_to_1[png_ptr->background.green];
               back_1.blue = png_ptr->gamma_to_1[png_ptr->background.blue];
            }
            else
            {
               double g, gs;

               switch (png_ptr->background_gamma_type)
               {
                  case PNG_BACKGROUND_GAMMA_SCREEN:
                     g = (png_ptr->screen_gamma);
                     gs = 1.0;
                     break;

                  case PNG_BACKGROUND_GAMMA_FILE:
                     g = 1.0 / (png_ptr->gamma);
                     gs = 1.0 / (png_ptr->gamma * png_ptr->screen_gamma);
                     break;

                  case PNG_BACKGROUND_GAMMA_UNIQUE:
                     g = 1.0 / (png_ptr->background_gamma);
                     gs = 1.0 / (png_ptr->background_gamma *
                                 png_ptr->screen_gamma);
                     break;
                  default:
                     g = 1.0;    /* back_1 */
                     gs = 1.0;   /* back */
               }

               if ( fabs(gs - 1.0) < PNG_GAMMA_THRESHOLD)
               {
                  back.red   = (png_byte)png_ptr->background.red;
                  back.green = (png_byte)png_ptr->background.green;
                  back.blue  = (png_byte)png_ptr->background.blue;
               }
               else
               {
                  back.red = (png_byte)(pow(
                     (double)png_ptr->background.red/255, gs) * 255.0 + .5);
                  back.green = (png_byte)(pow(
                     (double)png_ptr->background.green/255, gs) * 255.0 + .5);
                  back.blue = (png_byte)(pow(
                     (double)png