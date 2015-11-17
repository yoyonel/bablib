/* png.h - header file for PNG reference library
 *
 * libpng version 1.2.40 - September 10, 2009
 * Copyright (c) 1998-2009 Glenn Randers-Pehrson
 * (Version 0.96 Copyright (c) 1996, 1997 Andreas Dilger)
 * (Version 0.88 Copyright (c) 1995, 1996 Guy Eric Schalnat, Group 42, Inc.)
 *
 * This code is released under the libpng license (See LICENSE, below)
 *
 * Authors and maintainers:
 *  libpng versions 0.71, May 1995, through 0.88, January 1996: Guy Schalnat
 *  libpng versions 0.89c, June 1996, through 0.96, May 1997: Andreas Dilger
 *  libpng versions 0.97, January 1998, through 1.2.40 - September 10, 2009: Glenn
 *  See also "Contributing Authors", below.
 *
 * Note about libpng version numbers:
 *
 *    Due to various miscommunications, unforeseen code incompatibilities
 *    and occasional factors outside the authors' control, version numbering
 *    on the library has not always been consistent and straightforward.
 *    The following table summarizes matters since version 0.89c, which was
 *    the first widely used release:
 *
 *    source                 png.h  png.h  shared-lib
 *    version                string   int  version
 *    -------                ------ -----  ----------
 *    0.89c "1.0 beta 3"     0.89      89  1.0.89
 *    0.90  "1.0 beta 4"     0.90      90  0.90  [should have been 2.0.90]
 *    0.95  "1.0 beta 5"     0.95      95  0.95  [should have been 2.0.95]
 *    0.96  "1.0 beta 6"     0.96      96  0.96  [should have been 2.0.96]
 *    0.97b "1.00.97 beta 7" 1.00.97   97  1.0.1 [should have been 2.0.97]
 *    0.97c                  0.97      97  2.0.97
 *    0.98                   0.98      98  2.0.98
 *    0.99                   0.99      98  2.0.99
 *    0.99a-m                0.99      99  2.0.99
 *    1.00                   1.00     100  2.1.0 [100 should be 10000]
 *    1.0.0      (from here on, the   100  2.1.0 [100 should be 10000]
 *    1.0.1       png.h string is   10001  2.1.0
 *    1.0.1a-e    identical to the  10002  from here on, the shared library
 *    1.0.2       source version)   10002  is 2.V where V is the source code
 *    1.0.2a-b                      10003  version, except as noted.
 *    1.0.3                         10003
 *    1.0.3a-d                      10004
 *    1.0.4                         10004
 *    1.0.4a-f                      10005
 *    1.0.5 (+ 2 patches)           10005
 *    1.0.5a-d                      10006
 *    1.0.5e-r                      10100 (not source compatible)
 *    1.0.5s-v                      10006 (not binary compatible)
 *    1.0.6 (+ 3 patches)           10006 (still binary incompatible)
 *    1.0.6d-f                      10007 (still binary incompatible)
 *    1.0.6g                        10007
 *    1.0.6h                        10007  10.6h (testing xy.z so-numbering)
 *    1.0.6i                        10007  10.6i
 *    1.0.6j                        10007  2.1.0.6j (incompatible with 1.0.0)
 *    1.0.7beta11-14        DLLNUM  10007  2.1.0.7beta11-14 (binary compatible)
 *    1.0.7beta15-18           1    10007  2.1.0.7beta15-18 (binary compatible)
 *    1.0.7rc1-2               1    10007  2.1.0.7rc1-2 (binary compatible)
 *    1.0.7                    1    10007  (still compatible)
 *    1.0.8beta1-4             1    10008  2.1.0.8beta1-4
 *    1.0.8rc1                 1    10008  2.1.0.8rc1
 *    1.0.8                    1    10008  2.1.0.8
 *    1.0.9beta1-6             1    10009  2.1.0.9beta1-6
 *    1.0.9rc1                 1    10009  2.1.0.9rc1
 *    1.0.9beta7-10            1    10009  2.1.0.9beta7-10
 *    1.0.9rc2                 1    10009  2.1.0.9rc2
 *    1.0.9                    1    10009  2.1.0.9
 *    1.0.10beta1              1    10010  2.1.0.10beta1
 *    1.0.10rc1                1    10010  2.1.0.10rc1
 *    1.0.10                   1    10010  2.1.0.10
 *    1.0.11beta1-3            1    10011  2.1.0.11beta1-3
 *    1.0.11rc1                1    10011  2.1.0.11rc1
 *    1.0.11                   1    10011  2.1.0.11
 *    1.0.12beta1-2            2    10012  2.1.0.12beta1-2
 *    1.0.12rc1                2    10012  2.1.0.12rc1
 *    1.0.12                   2    10012  2.1.0.12
 *    1.1.0a-f                 -    10100  2.1.1.0a-f (branch abandoned)
 *    1.2.0beta1-2             2    10200  2.1.2.0beta1-2
 *    1.2.0beta3-5             3    10200  3.1.2.0beta3-5
 *    1.2.0rc1                 3    10200  3.1.2.0rc1
 *    1.2.0                    3    10200  3.1.2.0
 *    1.2.1beta1-4             3    10201  3.1.2.1beta1-4
 *    1.2.1rc1-2               3    10201  3.1.2.1rc1-2
 *    1.2.1                    3    10201  3.1.2.1
 *    1.2.2beta1-6            12    10202  12.so.0.1.2.2beta1-6
 *    1.0.13beta1             10    10013  10.so.0.1.0.13beta1
 *    1.0.13rc1               10    10013  10.so.0.1.0.13rc1
 *    1.2.2rc1                12    10202  12.so.0.1.2.2rc1
 *    1.0.13                  10    10013  10.so.0.1.0.13
 *    1.2.2                   12    10202  12.so.0.1.2.2
 *    1.2.3rc1-6              12    10203  12.so.0.1.2.3rc1-6
 *    1.2.3                   12    10203  12.so.0.1.2.3
 *    1.2.4beta1-3            13    10204  12.so.0.1.2.4beta1-3
 *    1.0.14rc1               13    10014  10.so.0.1.0.14rc1
 *    1.2.4rc1                13    10204  12.so.0.1.2.4rc1
 *    1.0.14                  10    10014  10.so.0.1.0.14
 *    1.2.4                   13    10204  12.so.0.1.2.4
 *    1.2.5beta1-2            13    10205  12.so.0.1.2.5beta1-2
 *    1.0.15rc1-3             10    10015  10.so.0.1.0.15rc1-3
 *    1.2.5rc1-3              13    10205  12.so.0.1.2.5rc1-3
 *    1.0.15                  10    10015  10.so.0.1.0.15
 *    1.2.5                   13    10205  12.so.0.1.2.5
 *    1.2.6beta1-4            13    10206  12.so.0.1.2.6beta1-4
 *    1.0.16                  10    10016  10.so.0.1.0.16
 *    1.2.6                   13    10206  12.so.0.1.2.6
 *    1.2.7beta1-2            13    10207  12.so.0.1.2.7beta1-2
 *    1.0.17rc1               10    10017  10.so.0.1.0.17rc1
 *    1.2.7rc1                13    10207  12.so.0.1.2.7rc1
 *    1.0.17                  10    10017  10.so.0.1.0.17
 *    1.2.7                   13    10207  12.so.0.1.2.7
 *    1.2.8beta1-5            13    10208  12.so.0.1.2.8beta1-5
 *    1.0.18rc1-5             10    10018  10.so.0.1.0.18rc1-5
 *    1.2.8rc1-5              13    10208  12.so.0.1.2.8rc1-5
 *    1.0.18                  10    10018  10.so.0.1.0.18
 *    1.2.8                   13    10208  12.so.0.1.2.8
 *    1.2.9beta1-3            13    10209  12.so.0.1.2.9beta1-3
 *    1.2.9beta4-11           13    10209  12.so.0.9[.0]
 *    1.2.9rc1                13    10209  12.so.0.9[.0]
 *    1.2.9                   13    10209  12.so.0.9[.0]
 *    1.2.10beta1-8           13    10210  12.so.0.10[.0]
 *    1.2.10rc1-3             13    10210  12.so.0.10[.0]
 *    1.2.10                  13    10210  12.so.0.10[.0]
 *    1.2.11beta1-4           13    10211  12.so.0.11[.0]
 *    1.0.19rc1-5             10    10019  10.so.0.19[.0]
 *    1.2.11rc1-5             13    10211  12.so.0.11[.0]
 *    1.0.19                  10    10019  10.so.0.19[.0]
 *    1.2.11                  13    10211  12.so.0.11[.0]
 *    1.0.20                  10    10020  10.so.0.20[.0]
 *    1.2.12                  13    10212  12.so.0.12[.0]
 *    1.2.13beta1             13    10213  12.so.0.13[.0]
 *    1.0.21                  10    10021  10.so.0.21[.0]
 *    1.2.13                  13    10213  12.so.0.13[.0]
 *    1.2.14beta1-2           13    10214  12.so.0.14[.0]
 *    1.0.22rc1               10    10022  10.so.0.22[.0]
 *    1.2.14rc1               13    10214  12.so.0.14[.0]
 *    1.0.22                  10    10022  10.so.0.22[.0]
 *    1.2.14                  13    10214  12.so.0.14[.0]
 *    1.2.15beta1-6           13    10215  12.so.0.15[.0]
 *    1.0.23rc1-5             10    10023  10.so.0.23[.0]
 *    1.2.15rc1-5             13    10215  12.so.0.15[.0]
 *    1.0.23                  10    10023  10.so.0.23[.0]
 *    1.2.15                  13    10215  12.so.0.15[.0]
 *    1.2.16beta1-2           13    10216  12.so.0.16[.0]
 *    1.2.16rc1               13    10216  12.so.0.16[.0]
 *    1.0.24                  10    10024  10.so.0.24[.0]
 *    1.2.16                  13    10216  12.so.0.16[.0]
 *    1.2.17beta1-2           13    10217  12.so.0.17[.0]
 *    1.0.25rc1               10    10025  10.so.0.25[.0]
 *    1.2.17rc1-3             13    10217  12.so.0.17[.0]
 *    1.0.25                  10    10025  10.so.0.25[.0]
 *    1.2.17                  13    10217  12.so.0.17[.0]
 *    1.0.26                  10    10026  10.so.0.26[.0]
 *    1.2.18                  13    10218  12.so.0.18[.0]
 *    1.2.19beta1-31          13    10219  12.so.0.19[.0]
 *    1.0.27rc1-6             10    10027  10.so.0.27[.0]
 *    1.2.19rc1-6             13    10219  12.so.0.19[.0]
 *    1.0.27                  10    10027  10.so.0.27[.0]
 *    1.2.19                  13    10219  12.so.0.19[.0]
 *    1.2.20beta01-04         13    10220  12.so.0.20[.0]
 *    1.0.28rc1-6             10    10028  10.so.0.28[.0]
 *    1.2.20rc1-6             13    10220  12.so.0.20[.0]
 *    1.0.28                  10    10028  10.so.0.28[.0]
 *    1.2.20                  13    10220  12.so.0.20[.0]
 *    1.2.21beta1-2           13    10221  12.so.0.21[.0]
 *    1.2.21rc1-3             13    10221  12.so.0.21[.0]
 *    1.0.29                  10    10029  10.so.0.29[.0]
 *    1.2.21                  13    10221  12.so.0.21[.0]
 *    1.2.22beta1-4           13    10222  12.so.0.22[.0]
 *    1.0.30rc1               10    10030  10.so.0.30[.0]
 *    1.2.22rc1               13    10222  12.so.0.22[.0]
 *    1.0.30                  10    10030  10.so.0.30[.0]
 *    1.2.22                  13    10222  12.so.0.22[.0]
 *    1.2.23beta01-05         13    10223  12.so.0.23[.0]
 *    1.2.23rc01              13    10223  12.so.0.23[.0]
 *    1.2.23                  13    10223  12.so.0.23[.0]
 *    1.2.24beta01-02         13    10224  12.so.0.24[.0]
 *    1.2.24rc01              13    10224  12.so.0.24[.0]
 *    1.2.24                  13    10224  12.so.0.24[.0]
 *    1.2.25beta01-06         13    10225  12.so.0.25[.0]
 *    1.2.25rc01-02           13    10225  12.so.0.25[.0]
 *    1.0.31                  10    10031  10.so.0.31[.0]
 *    1.2.25                  13    10225  12.so.0.25[.0]
 *    1.2.26beta01-06         13    10226  12.so.0.26[.0]
 *    1.2.26rc01              13    10226  12.so.0.26[.0]
 *    1.2.26                  13    10226  12.so.0.26[.0]
 *    1.0.32                  10    10032  10.so.0.32[.0]
 *    1.2.27beta01-06         13    10227  12.so.0.27[.0]
 *    1.2.27rc01              13    10227  12.so.0.27[.0]
 *    1.0.33                  10    10033  10.so.0.33[.0]
 *    1.2.27                  13    10227  12.so.0.27[.0]
 *    1.0.34                  10    10034  10.so.0.34[.0]
 *    1.2.28                  13    10228  12.so.0.28[.0]
 *    1.2.29beta01-03         13    10229  12.so.0.29[.0]
 *    1.2.29rc01              13    10229  12.so.0.29[.0]
 *    1.0.35                  10    10035  10.so.0.35[.0]
 *    1.2.29                  13    10229  12.so.0.29[.0]
 *    1.0.37                  10    10037  10.so.0.37[.0]
 *    1.2.30beta01-04         13    10230  12.so.0.30[.0]
 *    1.0.38rc01-08           10    10038  10.so.0.38[.0]
 *    1.2.30rc01-08           13    10230  12.so.0.30[.0]
 *    1.0.38                  10    10038  10.so.0.38[.0]
 *    1.2.30                  13    10230  12.so.0.30[.0]
 *    1.0.39rc01-03           10    10039  10.so.0.39[.0]
 *    1.2.31rc01-03           13    10231  12.so.0.31[.0]
 *    1.0.39                  10    10039  10.so.0.39[.0]
 *    1.2.31                  13    10231  12.so.0.31[.0]
 *    1.2.32beta01-02         13    10232  12.so.0.32[.0]
 *    1.0.40rc01              10    10040  10.so.0.40[.0]
 *    1.2.32rc01              13    10232  12.so.0.32[.0]
 *    1.0.40                  10    10040  10.so.0.40[.0]
 *    1.2.32                  13    10232  12.so.0.32[.0]
 *    1.2.33beta01-02         13    10233  12.so.0.33[.0]
 *    1.2.33rc01-02           13    10233  12.so.0.33[.0]
 *    1.0.41rc01              10    10041  10.so.0.41[.0]
 *    1.2.33                  13    10233  12.so.0.33[.0]
 *    1.0.41                  10    10041  10.so.0.41[.0]
 *    1.2.34beta01-07         13    10234  12.so.0.34[.0]
 *    1.0.42rc01              10    10042  10.so.0.42[.0]
 *    1.2.34rc01              13    10234  12.so.0.34[.0]
 *    1.0.42                  10    10042  10.so.0.42[.0]
 *    1.2.34                  13    10234  12.so.0.34[.0]
 *    1.2.35beta01-03         13    10235  12.so.0.35[.0]
 *    1.0.43rc01-02           10    10043  10.so.0.43[.0]
 *    1.2.35rc01-02           13    10235  12.so.0.35[.0]
 *    1.0.43                  10    10043  10.so.0.43[.0]
 *    1.2.35                  13    10235  12.so.0.35[.0]
 *    1.2.36beta01-05         13    10236  12.so.0.36[.0]
 *    1.2.36rc01              13    10236  12.so.0.36[.0]
 *    1.0.44                  10    10044  10.so.0.44[.0]
 *    1.2.36                  13    10236  12.so.0.36[.0]
 *    1.2.37beta01-03         13    10237  12.so.0.37[.0]
 *    1.2.37rc01              13    10237  12.so.0.37[.0]
 *    1.2.37                  13    10237  12.so.0.37[.0]
 *    1.2.45                  10    10045  12.so.0.45[.0]
 *    1.0.46                  10    10046  10.so.0.46[.0]
 *    1.2.38beta01            13    10238  12.so.0.38[.0]
 *    1.2.38rc01-03           13    10238  12.so.0.38[.0]
 *    1.0.47                  10    10047  10.so.0.47[.0]
 *    1.2.38                  13    10238  12.so.0.38[.0]
 *    1.2.39beta01-05         13    10239  12.so.0.39[.0]
 *    1.2.39rc01              13    10239  12.so.0.39[.0]
 *    1.0.48                  10    10048  10.so.0.48[.0]
 *    1.2.39                  13    10239  12.so.0.39[.0]
 *    1.2.40beta01            13    10240  12.so.0.40[.0]
 *    1.2.40rc01              13    10240  12.so.0.40[.0]
 *    1.0.49                  10    10049  10.so.0.49[.0]
 *    1.2.40                  13    10240  12.so.0.40[.0]
 *
 *    Henceforth the source version will match the shared-library major
 *    and minor numbers; the shared-library major version number will be
 *    used for changes in backward compatibility, as it is intended.  The
 *    PNG_LIBPNG_VER macro, which is not used within libpng but is available
 *    for applications, is an unsigned integer of the form xyyzz corresponding
 *    to the source version x.y.z (leading zeros in y and z).  Beta versions
 *    were given the previous public release number plus a letter, until
 *    version 1.0.6j; from then on they were given the upcoming public
 *    release number plus "betaNN" or "rcNN".
 *
 *    Binary incompatibility exists only when applications make direct access
 *    to the info_ptr or png_ptr members through png.h, and the compiled
 *    application is loaded with a different version of the library.
 *
 *    DLLNUM will change each time there are forward or backward changes
 *    in binary compatibility (e.g., when a new feature is added).
 *
 * See libpng.txt or libpng.3 for more information.  The PNG specification
 * is available as a W3C Recommendation and as an ISO Specification,
 * <http://www.w3.org/TR/2003/REC-PNG-20031110/
 */

/*
 * COPYRIGHT NOTICE, DISCLAIMER, and LICENSE:
 *
 * If you modify libpng you may insert additional notices immediately following
 * this sentence.
 *
 * This code is released under the libpng license.
 *
 * libpng versions 1.2.6, August 15, 2004, through 1.2.40, September 10, 2009, are
 * Copyright (c) 2004, 2006-2009 Glenn Randers-Pehrson, and are
 * distributed according to the same disclaimer and license as libpng-1.2.5
 * with the following individual added to the list of Contributing Authors:
 *
 *    Cosmin Truta
 *
 * libpng versions 1.0.7, July 1, 2000, through 1.2.5, October 3, 2002, are
 * Copyright (c) 2000-2002 Glenn Randers-Pehrson, and are
 * distributed according to the same disclaimer and license as libpng-1.0.6
 * with the following individuals added to the list of Contributing Authors:
 *
 *    Simon-Pierre Cadieux
 *    Eric S. Raymond
 *    Gilles Vollant
 *
 * and with the following additions to the disclaimer:
 *
 *    There is no warranty against interference with your enjoyment of the
 *    library or against infringement.  There is no warranty that our
 *    efforts or the library will fulfill any of your particular purposes
 *    or needs.  This library is provided with all faults, and the entire
 *    risk of satisfactory quality, performance, accuracy, and effort is with
 *    the user.
 *
 * libpng versions 0.97, January 1998, through 1.0.6, March 20, 2000, are
 * Copyright (c) 1998, 1999, 2000 Glenn Randers-Pehrson, and are
 * distributed according to the same disclaimer and license as libpng-0.96,
 * with the following individuals added to the list of Contributing Authors:
 *
 *    Tom Lane
 *    Glenn Randers-Pehrson
 *    Willem van Schaik
 *
 * libpng versions 0.89, June 1996, through 0.96, May 1997, are
 * Copyright (c) 1996, 1997 Andreas Dilger
 * Distributed according to the same disclaimer and license as libpng-0.88,
 * with the following individuals added to the list of Contributing Authors:
 *
 *    John Bowler
 *    Kevin Bracey
 *    Sam Bushell
 *    Magnus Holmgren
 *    Greg Roelofs
 *    Tom Tanner
 *
 * libpng versions 0.5, May 1995, through 0.88, January 1996, are
 * Copyright (c) 1995, 1996 Guy Eric Schalnat, Group 42, Inc.
 *
 * For the purposes of this copyright and license, "Contributing Authors"
 * is defined as the following set of individuals:
 *
 *    Andreas Dilger
 *    Dave Martindale
 *    Guy Eric Schalnat
 *    Paul Schmidt
 *    Tim Wegner
 *
 * The PNG Reference Library is supplied "AS IS".  The Contributing Authors
 * and Group 42, Inc. disclaim all warranties, expressed or implied,
 * including, without limitation, the warranties of merchantability and of
 * fitness for any purpose.  The Contributing Authors and Group 42, Inc.
 * assume no liability for direct, indirect, incidental, special, exemplary,
 * or consequential damages, which may result from the use of the PNG
 * Reference Library, even if advised of the possibility of such damage.
 *
 * Permission is hereby granted to use, copy, modify, and distribute this
 * source code, or portions hereof, for any purpose, without fee, subject
 * to the following restrictions:
 *
 * 1. The origin of this source code must not be misrepresented.
 *
 * 2. Altered versions must be plainly marked as such and
 * must not be misrepresented as being the original source.
 *
 * 3. This Copyright notice may not be removed or altered from
 *    any source or altered source distribution.
 *
 * The Contributing Authors and Group 42, Inc. specifically permit, without
 * fee, and encourage the use of this source code as a component to
 * supporting the PNG file format in commercial products.  If you use this
 * source code in a product, acknowledgment is not required but would be
 * appreciated.
 */

/*
 * A "png_get_copyright" function is available, for convenient use in "about"
 * boxes and the like:
 *
 * printf("%s",png_get_copyright(NULL));
 *
 * Also, the PNG logo (in PNG format, of course) is supplied in the
 * files "pngbar.png" and "pngbar.jpg (88x31) and "pngnow.png" (98x31).
 */

/*
 * Libpng is OSI Certified Open Source Software.  OSI Certified is a
 * certification mark of the Open Source Initiative.
 */

/*
 * The contributing authors would like to thank all those who helped
 * with testing, bug fixes, and patience.  This wouldn't have been
 * possible without all of you.
 *
 * Thanks to Frank J. T. Wojcik for helping with the documentation.
 */

/*
 * Y2K compliance in libpng:
 * =========================
 *
 *    September 10, 2009
 *
 *    Since the PNG Development group is an ad-hoc body, we can't make
 *    an official declaration.
 *
 *    This is your unofficial assurance that libpng from version 0.71 and
 *    upward through 1.2.40 are Y2K compliant.  It is my belief that earlier
 *    versions were also Y2K compliant.
 *
 *    Libpng only has three year fields.  One is a 2-byte unsigned integer
 *    that will hold years up to 65535.  The other two hold the date in text
 *    format, and will hold years up to 9999.
 *
 *    The integer is
 *        "png_uint_16 year" in png_time_struct.
 *
 *    The strings are
 *        "png_charp time_buffer" in png_struct and
 *        "near_time_buffer", which is a local character string in png.c.
 *
 *    There are seven time-related functions:
 *        png.c: png_convert_to_rfc_1123() in png.c
 *          (formerly png_convert_to_rfc_1152() in error)
 *        png_convert_from_struct_tm() in pngwrite.c, called in pngwrite.c
 *        png_convert_from_time_t() in pngwrite.c
 *        png_get_tIME() in pngget.c
 *        png_handle_tIME() in pngrutil.c, called in pngread.c
 *        png_set_tIME() in pngset.c
 *        png_write_tIME() in pngwutil.c, called in pngwrite.c
 *
 *    All handle dates properly in a Y2K environment.  The
 *    png_convert_from_time_t() function calls gmtime() to convert from system
 *    clock time, which returns (year - 1900), which we properly convert to
 *    the full 4-digit year.  There is a possibility that applications using
 *    libpng are not passing 4-digit years into the png_convert_to_rfc_1123()
 *    function, or that they are incorrectly passing only a 2-digit year
 *    instead of "year - 1900" into the png_convert_from_struct_tm() function,
 *    but this is not under our control.  The libpng documentation has always
 *    stated that it works with 4-digit years, and the APIs have been
 *    documented as such.
 *
 *    The tIME chunk itself is also Y2K compliant.  It uses a 2-byte unsigned
 *    integer to hold the year, and can hold years as large as 65535.
 *
 *    zlib, upon which libpng depends, is also Y2K compliant.  It contains
 *    no date-related code.
 *
 *       Glenn Randers-Pehrson
 *       libpng maintainer
 *       PNG Development Group
 */

#ifndef PNG_H
#define PNG_H

/* This is not the place to learn how to use libpng.  The file libpng.txt
 * describes how to use libpng, and the file example.c summarizes it
 * with some code on which to build.  This file is useful for looking
 * at the actual function definitions and structure components.
 */

/* Version information for png.h - this should match the version in png.c */
#define PNG_LIBPNG_VER_STRING "1.2.40"
#define PNG_HEADER_VERSION_STRING \
   " libpng version 1.2.40 - September 10, 2009\n"

#define PNG_LIBPNG_VER_SONUM   0
#define PNG_LIBPNG_VER_DLLNUM  13

/* These should match the first 3 components of PNG_LIBPNG_VER_STRING: */
#define PNG_LIBPNG_VER_MAJOR   1
#define PNG_LIBPNG_VER_MINOR   2
#define PNG_LIBPNG_VER_RELEASE 40
/* This should match the numeric part of the final component of
 * PNG_LIBPNG_VER_STRING, omitting any leading zero:
 */

#define PNG_LIBPNG_VER_BUILD  0

/* Release Status */
#define PNG_LIBPNG_BUILD_ALPHA    1
#define PNG_LIBPNG_BUILD_BETA     2
#define PNG_LIBPNG_BUILD_RC       3
#define PNG_LIBPNG_BUILD_STABLE   4
#define PNG_LIBPNG_BUILD_RELEASE_STATUS_MASK 7

/* Release-Specific Flags */
#define PNG_LIBPNG_BUILD_PATCH    8 /* Can be OR'ed with
                                       PNG_LIBPNG_BUILD_STABLE only */
#define PNG_LIBPNG_BUILD_PRIVATE 16 /* Cannot be OR'ed with
                                       PNG_LIBPNG_BUILD_SPECIAL */
#define PNG_LIBPNG_BUILD_SPECIAL 32 /* Cannot be OR'ed with
                                       PNG_LIBPNG_BUILD_PRIVATE */

#define PNG_LIBPNG_BUILD_BASE_TYPE PNG_LIBPNG_BUILD_STABLE

/* Careful here.  At one time, Guy wanted to use 082, but that would be octal.
 * We must not include leading zeros.
 * Versions 0.7 through 1.0.0 were in the range 0 to 100 here (only
 * version 1.0.0 was mis-numbered 100 instead of 10000).  From
 * version 1.0.1 it's    xxyyzz, where x=major, y=minor, z=release
 */
#define PNG_LIBPNG_VER 10240 /* 1.2.40 */

#ifndef PNG_VERSION_INFO_ONLY
/* Include the compression library's header */
#include "zlib.h"
#endif

/* Include all user configurable info, including optional assembler routines */
#include "pngconf.h"

/*
 * Added at libpng-1.2.8 */
/* Ref MSDN: Private as priority over Special
 * VS_FF_PRIVATEBUILD File *was not* built using standard release
 * procedures. If this value is given, the StringFileInfo block must
 * contain a PrivateBuild string.
 *
 * VS_FF_SPECIALBUILD File *was* built by the original company using
 * standard release procedures but is a variation of the standard
 * file of the same version number. If this value is given, the
 * StringFileInfo block must contain a SpecialBuild string.
 */

#if defined(PNG_USER_PRIVATEBUILD)
#  define PNG_LIBPNG_BUILD_TYPE \
          (PNG_LIBPNG_BUILD_BASE_TYPE | PNG_LIBPNG_BUILD_PRIVATE)
#else
#  if defined(PNG_LIBPNG_SPECIALBUILD)
#    define PNG_LIBPNG_BUILD_TYPE \
            (PNG_LIBPNG_BUILD_BASE_TYPE | PNG_LIBPNG_BUILD_SPECIAL)
#  else
#    define PNG_LIBPNG_BUILD_TYPE (PNG_LIBPNG_BUILD_BASE_TYPE)
#  endif
#endif

#ifndef PNG_VERSION_INFO_ONLY

/* Inhibit C++ name-mangling for libpng functions but not for system calls. */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* This file is arranged in several sections.  The first section contains
 * structure and type definitions.  The second section contains the external
 * library functions, while the third has the internal library functions,
 * which applications aren't expected to use directly.
 */

#ifndef PNG_NO_TYPECAST_NULL
#define int_p_NULL                (int *)NULL
#define png_bytep_NULL            (png_bytep)NULL
#define png_bytepp_NULL           (png_bytepp)NULL
#define png_doublep_NULL          (png_doublep)NULL
#define png_error_ptr_NULL        (png_error_ptr)NULL
#define png_flush_ptr_NULL        (png_flush_ptr)NULL
#define png_free_ptr_NULL         (png_free_ptr)NULL
#define png_infopp_NULL           (png_infopp)NULL
#define png_malloc_ptr_NULL       (png_malloc_ptr)NULL
#define png_read_status_ptr_NULL  (png_read_status_ptr)NULL
#define png_rw_ptr_NULL           (png_rw_ptr)NULL
#define png_structp_NULL          (png_structp)NULL
#define png_uint_16p_NULL         (png_uint_16p)NULL
#define png_voidp_NULL            (png_voidp)NULL
#define png_write_status_ptr_NULL (png_write_status_ptr)NULL
#else
#define int_p_NULL                NULL
#define png_bytep_NULL            NULL
#define png_bytepp_NULL           NULL
#define png_doublep_NULL          NULL
#define png_error_ptr_NULL        NULL
#define png_flush_ptr_NULL        NULL
#define png_free_ptr_NULL         NULL
#define png_infopp_NULL           NULL
#define png_malloc_ptr_NULL       NULL
#define png_read_status_ptr_NULL  NULL
#define png_rw_ptr_NULL           NULL
#define png_structp_NULL          NULL
#define png_uint_16p_NULL         NULL
#define png_voidp_NULL            NULL
#define png_write_status_ptr_NULL NULL
#endif

/* Variables declared in png.c - only it needs to define PNG_NO_EXTERN */
#if !defined(PNG_NO_EXTERN) || defined(PNG_ALWAYS_EXTERN)
/* Version information for C files, stored in png.c.  This had better match
 * the version above.
 */
#ifdef PNG_USE_GLOBAL_ARRAYS
PNG_EXPORT_VAR (PNG_CONST char) png_libpng_ver[18];
  /* Need room for 99.99.99beta99z */
#else
#define png_libpng_ver png_get_header_ver(NULL)
#endif

#ifdef PNG_USE_GLOBAL_ARRAYS
/* This was removed in version 1.0.5c */
/* Structures to facilitate easy interlacing.  See png.c for more details */
PNG_EXPORT_VAR (PNG_CONST int FARDATA) png_pass_start[7];
PNG_EXPORT_VAR (PNG_CONST int FARDATA) png_pass_inc[7];
PNG_EXPORT_VAR (PNG_CONST int FARDATA) png_pass_ystart[7];
PNG_EXPORT_VAR (PNG_CONST int FARDATA) png_pass_yinc[7];
PNG_EXPORT_VAR (PNG_CONST int FARDATA) png_pass_mask[7];
PNG_EXPORT_VAR (PNG_CONST int FARDATA) png_pass_dsp_mask[7];
/* This isn't currently used.  If you need it, see png.c for more details.
PNG_EXPORT_VAR (PNG_CONST int FARDATA) png_pass_height[7];
*/
#endif

#endif /* PNG_NO_EXTERN */

/* Three color definitions.  The order of the red, green, and blue, (and the
 * exact size) is not important, although the size of the fields need to
 * be png_byte or png_uint_16 (as defined below).
 */
typedef struct png_color_struct
{
   png_byte red;
   png_byte green;
   png_byte blue;
} png_color;
typedef png_color FAR * png_colorp;
typedef png_color FAR * FAR * png_colorpp;

typedef struct png_color_16_struct
{
   png_byte index;    /* used for palette files */
   png_uint_16 red;   /* for use in red green blue files */
   png_uint_16 green;
   png_uint_16 blue;
   png_uint_16 gray;  /* for use in grayscale files */
} png_color_16;
typedef png_color_16 FAR * png_color_16p;
typedef png_color_16 FAR * FAR * png_color_16pp;

typedef struct png_color_8_struct
{
   png_byte red;   /* for use in red green blue files */
   png_byte green;
   png_byte blue;
   png_byte gray;  /* for use in grayscale files */
   png_byte alpha; /* for alpha channel files */
} png_color_8;
typedef png_color_8 FAR * png_color_8p;
typedef png_color_8 FAR * FAR * png_color_8pp;

/*
 * The following two structures are used for the in-core representation
 * of sPLT chunks.
 */
typedef struct png_sPLT_entry_struct
{
   png_uint_16 red;
   png_uint_16 green;
   png_uint_16 blue;
   png_uint_16 alpha;
   png_uint_16 frequency;
} png_sPLT_entry;
typedef png_sPLT_entry FAR * png_sPLT_entryp;
typedef png_sPLT_entry FAR * FAR * png_sPLT_entrypp;

/*  When the depth of the sPLT palette is 8 bits, the color and alpha samples
 *  occupy the LSB of their respective members, and the MSB of each member
 *  is zero-filled.  The frequency member always occupies the full 16 bits.
 */

typedef struct png_sPLT_struct
{
   png_charp name;           /* palette name */
   png_byte depth;           /* depth of palette samples */
   png_sPLT_entryp entries;  /* palette entries */
   png_int_32 nentries;      /* number of palette entries */
} png_sPLT_t;
typedef png_sPLT_t FAR * png_sPLT_tp;
typedef png_sPLT_t FAR * FAR * png_sPLT_tpp;

#ifdef PNG_TEXT_SUPPORTED
/* png_text holds the contents of a text/ztxt/itxt chunk in a PNG file,
 * and whether that contents is compressed or not.  The "key" field
 * points to a regular zero-terminated C string.  The "text", "lang", and
 * "lang_key" fields can be regular C strings, empty strings, or NULL pointers.
 * However, the * structure returned by png_get_text() will always contain
 * regular zero-terminated C strings (possibly empty), never NULL pointers,
 * so they can be safely used in printf() and other string-handling functions.
 */
typedef struct png_text_struct
{
   int  compression;       /* compression value:
                             -1: tEXt, none
                              0: zTXt, deflate
                              1: iTXt, none
                              2: iTXt, deflate  */
   png_charp key;          /* keyword, 1-79 character description of "text" */
   png_charp text;         /* comment, may be an empty string (ie "")
                              or a NULL pointer */
   png_size_t text_length; /* length of the text string */
#ifdef PNG_iTXt_SUPPORTED
   png_size_t itxt_length; /* length of the itxt string */
   png_charp lang;         /* language code, 0-79 characters
                              or a NULL pointer */
   png_charp lang_key;     /* keyword translated UTF-8 string, 0 or more
                              chars or a NULL pointer */
#endif
} png_text;
typedef png_text FAR * png_textp;
typedef png_text FAR * FAR * png_textpp;
#endif

/* Supported compression types for text in PNG files (tEXt, and zTXt).
 * The values of the PNG_TEXT_COMPRESSION_ defines should NOT be changed.
 */
#define PNG_TEXT_COMPRESSION_NONE_WR -3
#define PNG_TEXT_COMPRESSION_zTXt_WR -2
#define PNG_TEXT_COMPRESSION_NONE    -1
#define PNG_TEXT_COMPRESSION_zTXt     0
#define PNG_ITXT_COMPRESSION_NONE     1
#define PNG_ITXT_COMPRESSION_zTXt     2
#define PNG_TEXT_COMPRESSION_LAST     3  /* Not a valid value */

/* png_time is a way to hold the time in an machine independent way.
 * Two conversions are provided, both from time_t and struct tm.  There
 * is no portable way to convert to either of these structures, as far
 * as I know.  If you know of a portable way, send it to me.  As a side
 * note - PNG has always been Year 2000 compliant!
 */
typedef struct png_time_struct
{
   png_uint_16 year; /* full year, as in, 1995 */
   png_byte month;   /* month of year, 1 - 12 */
   png_byte day;     /* day of month, 1 - 31 */
   png_byte hour;    /* hour of day, 0 - 23 */
   png_byte minute;  /* minute of hour, 0