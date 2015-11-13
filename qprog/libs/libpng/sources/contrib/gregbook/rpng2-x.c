/*---------------------------------------------------------------------------

   rpng2 - progressive-model PNG display program                  rpng2-x.c

   This program decodes and displays PNG files progressively, as if it were
   a web browser (though the front end is only set up to read from files).
   It supports gamma correction, user-specified background colors, and user-
   specified background patterns (for transparent images).  This version is
   for the X Window System (tested by the author under Unix and by Martin
   Zinser under OpenVMS; may work under OS/2 with a little tweaking).

   Thanks to Adam Costello and Pieter S. van der Meulen for the "diamond"
   and "radial waves" patterns, respectively.

   to do (someday, maybe):
    - fix expose/redraw code:  don't draw entire row if only part exposed
    - 8-bit (colormapped) X support
    - finish resizable checkerboard-gradient (sizes 4-128?)
    - use %.1023s to simplify truncation of title-bar string?

  ---------------------------------------------------------------------------

   Changelog:
    - 1.01:  initial public release
    - 1.02:  modified to allow abbreviated options; fixed char/uchar mismatch
    - 1.10:  added support for non-default visuals; fixed X pixel-conversion
    - 1.11:  added -usleep option for demos; fixed command-line parsing bug
    - 1.12:  added -pause option for demos and testing
    - 1.20:  added runtime MMX-enabling/disabling and new -mmx* options
    - 1.21:  fixed some small X memory leaks (thanks to FranÃÂ§ois Petitjean)
    - 1.22:  fixed XFreeGC() crash bug (thanks to Patrick Welche)
    - 1.23:  added -bgpat 0 mode (std white/gray checkerboard, 8x8 squares)
    - 1.30:  added -loop option for -bgpat (ifdef FEATURE_LOOP); fixed bpp =
              24; added support for X resources (thanks to Gerhard Niklasch)
    - 1.31:  added code to skip unused chunks (thanks to Glenn Randers-Pehrson)
    - 1.32:  added AMD64/EM64T support (__x86_64__); added basic expose/redraw
              handling
    - 2.00:  dual-licensed (added GNU GPL)
    - 2.01:  fixed 64-bit typo in readpng2.c; fixed -pause usage description
    - 2.02:  fixed improper display of usage screen on PNG error(s); fixed
              unexpected-EOF and file-read-error cases; fixed Trace() cut-and-
              paste bugs

  ---------------------------------------------------------------------------

      Copyright (c) 1998-2008 Greg Roelofs.  All rights reserved.

      This software is provided "as is," without warranty of any kind,
      express or implied.  In no event shall the author or contributors
      be held liable for any damages arising in any way from the use of
      this software.

      The contents of this file are DUAL-LICENSED.  You may modify and/or
      redistribute this software according to the terms of one of the
      following two licenses (at your option):


      LICENSE 1 ("BSD-like with advertising clause"):

      Permission is granted to anyone to use this software for any purpose,
      including commercial applications, and to alter it and redistribute
      it freely, subject to the following restrictions:

      1. Redistributions of source code must retain the above copyright
         notice, disclaimer, and this list of conditions.
      2. Redistributions in binary form must reproduce the above copyright
         notice, disclaimer, and this list of conditions in the documenta-
         tion and/or other materials provided with the distribution.
      3. All advertising materials mentioning features or use of this
         software must display the following acknowledgment:

            This product includes software developed by Greg Roelofs
            and contributors for the book, "PNG: The Definitive Guide,"
            published by O'Reilly and Associates.


      LICENSE 2 (GNU GPL v2 or later):

      This program is free software; you can redistribute it and/or modify
      it under the terms of the GNU General Public License as published by
      the Free Software Foundation; either version 2 of the License, or
      (at your option) any later version.

      This program is distributed in the hope that it will be useful,
      but WITHOUT ANY WARRANTY; without even the implied warranty of
      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
      GNU General Public License for more details.

      You should have received a copy of the GNU General Public License
      along with this program; if not, write to the Free Software Foundation,
      Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  ---------------------------------------------------------------------------*/

#define PROGNAME  "rpng2-x"
#define LONGNAME  "Progressive PNG Viewer for X"
#define VERSION   "2.02 of 16 March 2008"
#define RESNAME   "rpng2"	/* our X resource application name */
#define RESCLASS  "Rpng"	/* our X resource class name */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <setjmp.h>       /* for jmpbuf declaration in readpng2.h */
#include <time.h>
#include <math.h>         /* only for PvdM background code */
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/keysym.h>   /* defines XK_* macros */

#ifdef VMS
#  include <unistd.h>
#endif

/* all for PvdM background code: */
#ifndef PI
#  define PI             3.141592653589793238
#endif
#define PI_2             (PI*0.5)
#define INV_PI_360       (360.0 / PI)
#define MAX(a,b)         (a>b?a:b)
#define MIN(a,b)         (a<b?a:b)
#define CLIP(a,min,max)  MAX(min,MIN((a),max))
#define ABS(a)           ((a)<0?-(a):(a))
#define CLIP8P(c)        MAX(0,(MIN((c),255)))   /* 8-bit pos. integer (uch) */
#define ROUNDF(f)        ((int)(f + 0.5))

#define QUIT(e,k) ((e.type == ButtonPress && e.xbutton.button == Button1) ||  \
                  (e.type == KeyPress &&   /*  v--- or 1 for shifted keys */  \
                  ((k = XLookupKeysym(&e.xkey, 0)) == XK_q || k == XK_Escape)))

#define NO_24BIT_MASKS	/* undef case not fully written--only for redisplay() */

#define rgb1_max   bg_freq
#define rgb1_min   bg_gray
#define rgb2_max   bg_bsat
#define rgb2_min   bg_brot

/* #define DEBUG */     /* this enables the Trace() macros */

#include "readpng2.h"   /* typedefs, common macros, readpng2 prototypes */


/* could just include png.h, but this macro is the only thing we need
 * (name and typedefs changed to local versions); note that side effects
 * only happen with alpha (which could easily be avoided with
 * "ush acopy = (alpha);") */

#define alpha_composite(composite, fg, alpha, bg) {               \
    ush temp = ((ush)(fg)*(ush)(alpha) +                          \
                (ush)(bg)*(ush)(255 - (ush)(alpha)) + (ush)128);  \
    (composite) = (uch)((temp + (temp >> 8)) >> 8);               \
}


#define INBUFSIZE 4096   /* with pseudo-timing on (1 sec delay/block), this
                          *  block size corresponds roughly to a download
                          *  speed 10% faster than theoretical 33.6K maximum
                          *  (assuming 8 data bits, 1 stop bit and no other
                          *  overhead) */

/* local prototypes */
static void rpng2_x_init (void);
static int  rpng2_x_create_window (void);
static int  rpng2_x_load_bg_image (void);
static void rpng2_x_display_row (ulg row);
static void rpng2_x_finish_display (void);
static void rpng2_x_redisplay_image (ulg startcol, ulg startrow,
                                     ulg width, ulg height);
#ifdef FEATURE_LOOP
static void rpng2_x_reload_bg_image (void);
static int  is_number (char *p);
#endif
static void rpng2_x_cleanup (void);
static int  rpng2_x_msb (ulg u32val);


static char titlebar[1024], *window_name = titlebar;
static char *appname = LONGNAME;
static char *icon_name = PROGNAME;
static char *res_name = RESNAME;
static char *res_class = RESCLASS;
static char *filename;
static FILE *infile;

static mainprog_info rpng2_info;

static uch inbuf[INBUFSIZE];
static int incount;

static int pat = 6;        /* must be less than num_bgpat */
static int bg_image = 0;
static int bgscale, bgscale_default = 16;
static ulg bg_rowbytes;
static uch *bg_data;

int pause_after_pass = FALSE;
int demo_timing = FALSE;
ulg usleep_duration = 0L;

static struct rgb_color {
    uch r, g, b;
} rgb[] = {
    {  0,   0,   0},    /*  0:  black */
    {255, 255, 255},    /*  1:  white */
    {173, 132,  57},    /*  2:  tan */
    { 64, 132,   0},    /*  3:  medium green */
    {189, 117,   1},    /*  4:  gold */
    {253, 249,   1},    /*  5:  yellow */
    {  0,   0, 255},    /*  6:  blue */
    {  0,   0, 120},    /*  7:  medium blue */
    {255,   0, 255},    /*  8:  magenta */
    { 64,   0,  64},    /*  9:  dark magenta */
    {255,   0,   0},    /* 10:  red */
    { 64,   0,   0},    /* 11:  dark red */
    {255, 127,   0},    /* 12:  orange */
    {192,  96,   0},    /* 13:  darker orange */
    { 24,  60,   0},    /* 14:  dark green-yellow */
    { 85, 125, 200},    /* 15:  ice blue */
    {192, 192, 192}     /* 16:  Netscape/Mosaic gray */
};
/* not used for now, but should be for error-checking:
static int num_rgb = sizeof(rgb) / sizeof(struct rgb_color);
 */

/*
    This whole struct is a fairly cheesy way to keep the number of
    command-line options to a minimum.  The radial-waves background
    type is a particularly poor fit to the integer elements of the
    struct...but a few macros and a little fixed-point math will do
    wonders for ya.

    type bits:
       F E D C B A 9 8 7 6 5 4 3 2 1 0
                             | | | | |
                             | | +-+-+-- 0 = sharp-edged checkerboard
                             | |         1 = soft diamonds
                             | |         2 = radial waves
                             | |       3-7 = undefined
                             | +-- gradient #2 inverted?
                             +-- alternating columns inverted?
 */
static struct background_pattern {
    ush type;
    int rgb1_max, rgb1_min;     /* or bg_freq, bg_gray */
    int rgb2_max, rgb2_min;     /* or bg_bsat, bg_brot (both scaled by 10)*/
} bg[] = {
    {0,     1,1, 16,16},        /* checkered:  white vs. light gray (basic) */
    {0+8,   2,0,  1,15},        /* checkered:  tan/black vs. white/ice blue */
    {0+24,  2,0,  1,0},         /* checkered:  tan/black vs. white/black */
    {0+8,   4,5,  0,2},         /* checkered:  gold/yellow vs. black/tan */
    {0+8,   4,5,  0,6},         /* checkered:  gold/yellow vs. black/blue */
    {0,     7,0,  8,9},         /* checkered:  deep blue/black vs. magenta */
    {0+8,  13,0,  5,14},        /* checkered:  orange/black vs. yellow */
    {0+8,  12,0, 10,11},        /* checkered:  orange/black vs. red */
    {1,     7,0,  8,0},         /* diamonds:  deep blue/black vs. magenta */
    {1,    12,0, 11,0},         /* diamonds:  orange vs. dark red */
    {1,    10,0,  7,0},         /* diamonds:  red vs. medium blue */
    {1,     4,0,  5,0},         /* diamonds:  gold vs. yellow */
    {1,     3,0,  0,0},         /* diamonds:  medium green vs. black */
    {2,    16, 100,  20,   0},  /* radial:  ~hard radial color-beams */
    {2,    18, 100,  10,   2},  /* radial:  soft, curved radial color-beams */
    {2,    16, 256, 100, 250},  /* radial:  very tight spiral */
    {2, 10000, 256,  11,   0}   /* radial:  dipole-moire' (almost fractal) */
};
static int num_bgpat = sizeof(bg) / sizeof(struct background_pattern);


/* X-specific variables */
static char *displayname;
static XImage *ximage;
static Display *display;
static int depth;
static Visual *visual;
static XVisualInfo *visual_list;
static int RShift, GShift, BShift;
static ulg RMask, GMask, BMask;
static Window window;
static GC gc;
static Colormap colormap;

static int have_nondefault_visual = FALSE;
static int have_colormap = FALSE;
static int have_window = FALSE;
static int have_gc = FALSE;




int main(int argc, char **argv)
{
#ifdef sgi
    char tmpline[80];
#endif
    char *p, *bgstr = NULL;
    int rc, alen, flen;
    int error = 0;
    int timing = FALSE;
    int have_bg = FALSE;
#ifdef FEATURE_LOOP
    int loop = FALSE;
    long loop_interval = -1;		/* seconds (100,000 max) */
#endif
    double LUT_exponent;                /* just the lookup table */
    double CRT_exponent = 2.2;          /* just the monitor */
    double default_display_exponent;    /* whole display system */
    XEvent e;
    KeySym k;


    /* First initialize a few things, just to be sure--memset takes care of
     * default background color (black), booleans (FALSE), pointers (NULL),
     * etc. */

    displayname = (char *)NULL;
    filename = (char *)NULL;
    memset(&rpng2_info, 0, sizeof(mainprog_info));


    /* Set the default value for our display-system exponent, i.e., the
     * product of the CRT exponent and the exponent corresponding to
     * the frame-buffer's lookup table (LUT), if any.  This is not an
     * exhaustive list of LUT values (e.g., OpenStep has a lot of weird
     * ones), but it should cover 99% of the current possibilities. */

#if defined(NeXT)
    /* third-party utilities can modify the default LUT exponent */
    LUT_exponent = 1.0 / 2.2;
    /*
    if (some_next_function_that_returns_gamma(&next_gamma))
        LUT_exponent = 1.0 / next_gamma;
     */
#elif defined(sgi)
    LUT_exponent = 1.0 / 1.7;
    /* there doesn't seem to be any documented function to
     * get the "gamma" value, so we do it the hard way */
    infile = fopen("/etc/config/system.glGammaVal", "r");
    if (infile) {
        double sgi_gamma;

        fgets(tmpline, 80, infile);
        fclose(infile);
        sgi_gamma = atof(tmpline);
        if (sgi_gamma > 0.0)
            LUT_exponent = 1.0 / sgi_gamma;
    }
#elif defined(Macintosh)
    LUT_exponent = 1.8 / 2.61;
    /*
    if (some_mac_function_that_returns_gamma(&mac_gamma))
        LUT_exponent = mac_gamma / 2.61;
     */
#else
    LUT_exponent = 1.0;   /* assume no LUT:  most PCs */
#endif

    /* the defaults above give 1.0, 1.3, 1.5 and 2.2, respectively: */
    default_display_exponent = LUT_exponent * CRT_exponent;


    /* If the user has set the SCREEN_GAMMA environment variable as suggested
     * (somewhat imprecisely) in the libpng documentation, use that; otherwise
     * use the default value we just calculated.  Either way, the user may
     * override this via a command-line option. */

    if ((p = getenv("SCREEN_GAMMA")) != NULL)
        rpng2_info.display_exponent = atof(p);
    else
        rpng2_info.display_exponent = default_display_exponent;


    /* Now parse the command line for options and the PNG filename. */

    while (*++argv && !error) {
        if (!strncmp(*argv, "-display", 2)) {
            if (!*++argv)
                ++error;
            else
                displayname = *argv;
        } else if (!strncmp(*argv, "-gamma", 2)) {
            if (!*++argv)
                ++error;
            else {
                rpng2_info.display_exponent = atof(*argv);
                if (rpng2_info.display_exponent <= 0.0)
                    ++error;
            }
        } else if (!strncmp(*argv, "-bgcolor", 4)) {
            if (!*++argv)
                ++error;
            else {
                bgstr = *argv;
                if (strlen(bgstr) != 7 || bgstr[0] != '#')
                    ++error;
                else {
                    have_bg = TRUE;
                    bg_image = FALSE;
                }
            }
        } else if (!strncmp(*argv, "-bgpat", 4)) {
            if (!*++argv)
                ++error;
            else {
                pat = atoi(*argv);
                if (pat >= 0 && pat < num_bgpat) {
                    bg_image = TRUE;
                    have_bg = FALSE;
                } else
                    ++error;
            }
        } else if (!strncmp(*argv, "-usleep", 2)) {
            if (!*++argv)
                ++error;
            else {
                usleep_duration = (ulg)atol(*argv);
                demo_timing = TRUE;
            }
        } else if (!strncmp(*argv, "-pause", 2)) {
            pause_after_pass = TRUE;
        } else if (!strncmp(*argv, "-timing", 2)) {
            timing = TRUE;
#ifdef FEATURE_LOOP
        } else if (!strncmp(*argv, "-loop", 2)) {
            loop = TRUE;
            if (!argv[1] || !is_number(argv[1]))
                loop_interval = 2;
            else {
                ++argv;
                loop_interval = atol(*argv);
                if (loop_interval < 0)
                    loop_interval = 2;
                else if (loop_interval > 100000)   /* bit more than one day */
                    loop_interval = 100000;
            }
#endif
#if (defined(__i386__) || defined(_M_IX86) || defined(__x86_64__))
        } else if (!strncmp(*argv, "-nommxfilters", 7)) {
            rpng2_info.nommxfilters = TRUE;
        } else if (!strncmp(*argv, "-nommxcombine", 7)) {
            rpng2_info.nommxcombine = TRUE;
        } else if (!strncmp(*argv, "-nommxinterlace", 7)) {
            rpng2_info.nommxinterlace = TRUE;
        } else if (!strcmp(*argv, "-nommx")) {
            rpng2_info.nommxfilters = TRUE;
            rpng2_info.nommxcombine = TRUE;
            rpng2_info.nommxinterlace = TRUE;
#endif
        } else {
            if (**argv != '-') {
                filename = *argv;
                if (argv[1])   /* shouldn't be any more args after filename */
                    ++error;
            } else
                ++error;   /* not expecting any other options */
        }
    }

    if (!filename)
        ++error;


    /* print usage screen if any errors up to this point */

    if (error) {
        fprintf(stderr, "\n%s %s:  %s\n\n", PROGNAME, VERSION, appname);
        readpng2_version_info();
        fprintf(stderr, "\n"
          "Usage:  %s [-display xdpy] [-gamma exp] [-bgcolor bg | -bgpat pat]\n"
#if (defined(__i386__) || defined(_M_IX86) || defined(__x86_64__))
          "        %*s [[-nommxfilters] [-nommxcombine] [-nommxinterlace] | -nommx]\n"
#endif
#ifdef FEATURE_LOOP
          "        %*s [-usleep dur | -timing] [-pause] [-loop [sec]] file.png\n\n"
#else
          "        %*s [-usleep dur | -timing] [-pause] file.png\n\n"
#endif
          "    xdpy\tname of the target X display (e.g., ``hostname:0'')\n"
          "    exp \ttransfer-function exponent (``gamma'') of the display\n"
          "\t\t  system in floating-point format (e.g., ``%.1f''); equal\n"
          "\t\t  to the product of the lookup-table exponent (varies)\n"
          "\t\t  and the CRT exponent (usually 2.2); must be positive\n"
          "    bg  \tdesired background color in 7-character hex RGB format\n"
          "\t\t  (e.g., ``#ff7700'' for orange:  same as HTML colors);\n"
          "\t\t  used with transparent images; overrides -bgpat\n"
          "    pat \tdesired background pattern number (0-%d); used with\n"
          "\t\t  transparent images; overrides -bgcolor\n"
#ifdef FEATURE_LOOP
          "    -loop\tloops through background images after initial display\n"
          "\t\t  is complete (depends on -bgpat)\n"
          "    sec \tseconds to display each background image (default = 2)\n"
#endif
#if (defined(__i386__) || defined(_M_IX86) || defined(__x86_64__))
          "    -nommx*\tdisable optimized MMX routines for decoding row filters,\n"
          "\t\t  combining rows, and expanding interlacing, respectively\n"
#endif
          "    dur \tduration in microseconds to wait after displaying each\n"
          "\t\t  row (for demo purposes)\n"
          "    -timing\tenables delay for every block read, to simulate modem\n"
          "\t\t  download of image (~36 Kbps)\n"
          "    -pause\tpauses after displaying each pass until mouse clicked\n"
          "\nPress Q, Esc or mouse button 1 (within image window, after image\n"
          "is displayed) to quit.\n"
          "\n", PROGNAME,
#if (defined(__i386__) || defined(_M_IX86) || defined(__x86_64__))
          (int)strlen(PROGNAME), " ",
#endif
          (int)strlen(PROGNAME), " ", default_display_exponent, num_bgpat-1);
        exit(1);
    }


    if (!(infile = fopen(filename, "rb"))) {
        fprintf(stderr, PROGNAME ":  can't open PNG file [%s]\n", filename);
        ++error;
    } else {
        incount = fread(inbuf, 1, INBUFSIZE, infile);
        if (incount < 8 || !readpng2_check_sig(inbuf, 8)) {
            fprintf(stderr, PROGNAME
              ":  [%s] is not a PNG file: incorrect signature\n",
              filename);
            ++error;
        } else if ((rc = readpng2_init(&rpng2_info)) != 0) {
            switch (rc) {
                case 2:
                    fprintf(stderr, PROGNAME
                      ":  [%s] has bad IHDR (libpng longjmp)\n", filename);
                    break;
                case 4:
                    fprintf(stderr, PROGNAME ":  insufficient memory\n");
                    break;
                default:
                    fprintf(stderr, PROGNAME
                      ":  unknown readpng2_init() error\n");
                    break;
            }
            ++error;
        } else {
            Trace((stderr, "about to call XOpenDisplay()\n"))
            display = XOpenDisplay(displayname);
            if (!display) {
                readpng2_cleanup(&rpng2_info);
                fprintf(stderr, PROGNAME ":  can't open X display [%s]\n",
                  displayname? displayname : "default");
                ++error;
            }
        }
        if (error)
            fclose(infile);
    }


    if (error) {
        fprintf(stderr, PROGNAME ":  aborting.\n");
        exit(2);
    }


    /* set the title-bar string, but make sure buffer doesn't overflow */

    alen = strlen(appname);
    flen = strlen(filename);
    if (alen + flen + 3 > 1023)
        sprintf(titlebar, "%s:  ...%s", appname, filename+(alen+flen+6-1023));
    else
        sprintf(titlebar, "%s:  %s", appname, filename);


    /* set some final rpng2_info variables before entering main data loop */

    if (have_bg) {
        unsigned r, g, b;   /* this approach quiets compiler warnings */

        sscanf(bgstr+1, "%2x%2x%2x", &r, &g, &b);
        rpng2_info.bg_red   = (uch)r;
        rpng2_info.bg_green = (uch)g;
        rpng2_info.bg_blue  = (uch)b;
    } else
        rpng2_info.need_bgcolor = TRUE;

    rpng2_info.state = kPreInit;
    rpng2_info.mainprog_init = rpng2_x_init;
    rpng2_info.mainprog_display_row = rpng2_x_display_row;
    rpng2_info.mainprog_finish_display = rpng2_x_finish_display;


    /* OK, this is the fun part:  call readpng2_decode_data() at the start of
     * the loop to deal with our first buffer of data (read in above to verify
     * that the file is a PNG image), then loop through the file and continue
     * calling the same routine to handle each chunk of data.  It in turn
     * passes the data to libpng, which will invoke one or more of our call-
     * backs as decoded data become available.  We optionally call sleep() for
     * one second per iteration to simulate downloading the image via an analog
     * modem. */

    for (;;) {
        Trace((stderr, "about to call readpng2_decode_data()\n"))
        if (readpng2_decode_data(&rpng2_info, inbuf, incount))
            ++error;
        Trace((stderr, "done with readpng2_decode_data()\n"))

        if (error || incount != INBUFSIZE || rpng2_info.state == kDone) {
            if (rpng2_info.state == kDone) {
                Trace((stderr, "done decoding PNG image\n"))
            } else if (ferror(infile)) {
                fprintf(stderr, PROGNAME
                  ":  error while reading PNG image file\n");
                exit(3);
            } else if (feof(infile)) {
                fprintf(stderr, PROGNAME ":  end of file reached "
                  "(unexpectedly) while reading PNG image file\n");
                exit(3);
            } else /* if (error) */ {
                // will print error message below
            }
            break;
        }

        if (timing)
            sleep(1);

        incount = fread(inbuf, 1, INBUFSIZE, infile);
    }


    /* clean up PNG stuff and report any decoding errors */

    fclose(infile);
    Trace((stderr, "about to call readpng2_cleanup()\n"))
    readpng2_cleanup(&rpng2_info);

    if (error) {
        fprintf(stderr, PROGNAME ":  libpng error while decoding PNG image\n");
        exit(3);
    }


#ifdef FEATURE_LOOP

    if (loop && bg_image) {
        Trace((stderr, "entering -loop loop (FEATURE_LOOP)\n"))
        for (;;) {
            int i, use_sleep;
            struct timeval now, then;

            /* get current time and add loop_interval to get target time */
            if (gettimeofday(&then, NULL) == 0) {
                then.tv_sec += loop_interval;
                use_sleep = FALSE;
            } else
                use_sleep = TRUE;

            /* do quick check for a quit event but don't wait for it */
            /* GRR BUG:  should also check for Expose events and redraw... */
            if (XCheckMaskEvent(display, KeyPressMask | ButtonPressMask, &e))
                if (QUIT(e,k))
                    break;

            /* generate next background image */
            if (++pat >= num_bgpat)
                pat = 0;
            rpng2_x_reload_bg_image();

            /* wait for timeout, using whatever means are available */
            if (use_sleep || gettimeofday(&now, NULL) != 0) {
                for (i = loop_interval;  i > 0;  --i) {
                    sleep(1);
                    /* GRR BUG:  also need to check for Expose (and redraw!) */
                    if (XCheckMaskEvent(display, KeyPressMask | ButtonPressMask,
                        &e) && QUIT(e,k))
                        break;
                }
            } else {
                /* Y2038 BUG! */
                if (now.tv_sec < then.tv_sec ||
                    (now.tv_sec == then.tv_sec && now.tv_usec < then.tv_usec))
                {
                    int quit = FALSE;
                    long seconds_to_go = then.tv_sec - now.tv_sec;
                    long usleep_usec;

                    /* basically chew up most of remaining loop-interval with
                     *  calls to sleep(1) interleaved with checks for quit
                     *  events, but also recalc time-to-go periodically; when
                     *  done, clean up any remaining time with usleep() call
                     *  (could also use SIGALRM, but signals are a pain...) */
                    while (seconds_to_go-- > 1) {
                        int seconds_done = 0;

                        for (i = seconds_to_go;  i > 0 && !quit;  --i) {
                            sleep(1);
                            /* GRR BUG:  need to check for Expose and redraw */
                            if (XCheckMaskEvent(display, KeyPressMask |
                                ButtonPressMask, &e) && QUIT(e,k))
                                quit = TRUE;
                            if (++seconds_done > 1000)
                                break;   /* time to redo seconds_to_go meas. */
                        }
                        if (quit)
                            break;

                        /* OK, more than 1000 seconds since last check:
                         *  correct the time-to-go measurement for drift */
                        if (gettimeofday(&now, NULL) == 0) {
                            if (now.tv_sec >= then.tv_sec)
                                break;
                            seconds_to_go = then.tv_sec - now.tv_sec;
                        } else
                            ++seconds_to_go;  /* restore what we subtracted */
                    }
                    if (quit)
                        break;   /* breaks outer do-loop, skips redisplay */

                    /* since difference between "now" and "then" is already
                     *  eaten up to within a couple of seconds, don't need to
                     *  worry about overflow--but might have overshot (neg.) */
                    if (gettimeofday(&now, NULL) == 0) {
                        usleep_usec = 1000000L*(then.tv_sec - now.tv_sec) +
                          then.tv_usec - now.tv_usec;
                        if (usleep_usec > 0)
                            usleep((ulg)usleep_usec);
                    }
                }
            }

            /* composite image against new background and display (note that
             *  we do not take into account the time spent doing this...) */
            rpng2_x_redisplay_image (0, 0, rpng2_info.width, rpng2_info.height);
        }

    } else /* FALL THROUGH and do the normal thing */

#endif /* FEATURE_LOOP */

    /* wait for the user to tell us when to quit */

    if (rpng2_info.state >= kWindowInit) {
        Trace((stderr, "entering final wait-for-quit-event loop\n"))
        do {
            XNextEvent(display, &e);
            if (e.type == Expose) {
                XExposeEvent *ex = (XExposeEvent *)&e;
                rpng2_x_redisplay_image (ex->x, ex->y, ex->width, ex->height);
            }
        } while (!QUIT(e,k));
    } else {
        fprintf(stderr, PROGNAME ":  init callback never called:  probable "
          "libpng error while decoding PNG metadata\n");
        exit(4);
    }


    /* we're done:  clean up all image and X resources and go away */

    Trace((stderr, "about to call rpng2_x_cleanup()\n"))
    rpng2_x_cleanup();

    return 0;
}





/* this function is called by readpng2_info_callback() in readpng2.c, which
 * in turn is called by libpng after all of the pre-IDAT chunks have been
 * read and processed--i.e., we now have enough info to finish initializing */

static void rpng2_x_init(void)
{
    ulg i;
    ulg rowbytes = rpng2_info.rowbytes;

    Trace((stderr, "beginning rpng2_x_init()\n"))
    Trace((stderr, "  rowbytes = %d\n", rpng2_info.rowbytes))
    Trace((stderr, "  width  = %ld\n", rpng2_info.width))
    Trace((stderr, "  height = %ld\n", rpng2_info.height))

    rpng2_info.image_data = (uch *)malloc(rowbytes * rpng2_info.height);
    if (!rpng2_info.image_data) {
        readpng2_cleanup(&rpng2_info);
        return;
    }

    rpng2_info.row_pointers = (uch **)malloc(rpng2_info.height * sizeof(uch *));
    if (!rpng2_info.row_pointers) {
        free(rpng2_info.image_data);
        rpng2_info.image_data = NULL;
        readpng2_cleanup(&rpng2_info);
        return;
    }

    for (i = 0;  i < rpng2_info.height;  ++i)
        rpng2_info.row_pointers[i] = rpng2_info.image_data + i*rowbytes;


    /* do the basic X initialization stuff, make the window, and fill it with
     * the user-specified, file-specified or default background color or
     * pattern */

    if (rpng2_x_create_window()) {

        /* GRR TEMPORARY HACK:  this is fundamentally no different from cases
         * above; libpng should longjmp() back to us when png_ptr goes away.
         * If we/it segfault instead, seems like a libpng bug... */

        /* we're here via libpng callback, so if window fails, clean and bail */
        readpng2_cleanup(&rpng2_info);
        rpng2_x_cleanup();
        exit(2);
    }

    rpng2_info.state = kWindowInit;
}





static int rpng2_x_create_window(void)
{
    ulg bg_red   = rpng2_info.bg_red;
    ulg bg_green = rpng2_info.bg_green;
    ulg bg_blue  = rpng2_info.bg_blue;
    ulg bg_pixel = 0L;
    ulg attrmask;
    int need_colormap = FALSE;
    int screen, pad;
    uch *xdata;
    Window root;
    XEvent e;
    XGCValues gcvalues;
    XSetWindowAttributes attr;
    XTextProperty windowName, *pWindowName = &windowName;
    XTextProperty iconName, *pIconName = &iconName;
    XVisualInfo visual_info;
    XSizeHints *size_hints;
    XWMHints *wm_hints;
    XClassHint *class_hints;


    Trace((stderr, "beginning rpng2_x_create_window()\n"))

    screen = DefaultScreen(display);
    depth = DisplayPlanes(display, screen);
    root = RootWindow(display, screen);

#ifdef DEBUG
    XSynchronize(display, True);
#endif

    if (depth != 16 && depth != 24 && depth != 32) {
        int visuals_matched = 0;

        Trace((stderr, "default depth is %d:  checking other visuals\n",
          depth))

        /* 24-bit first */
        visual_info.screen = screen;
        visual_info.depth = 24;
        visual_list = XGetVisualInfo(display,
          VisualScreenMask | VisualDepthMask, &visual_info, &visuals_matched);
        if (visuals_matched == 0) {
/* GRR:  add 15-, 16- and 32-bit TrueColor visuals (also DirectColor?) */
            fprintf(stderr, "default screen depth %d not supported, and no"
              " 24-bit