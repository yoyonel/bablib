# ltmain.sh - Provide generalized library-building support services.
# NOTE: Changing this file will not affect anything until you rerun configure.
#
# Copyright (C) 1996, 1997, 1998, 1999, 2000, 2001, 2003, 2004, 2005, 2006,
# 2007, 2008  Free Software Foundation, Inc.
# Originally by Gordon Matzigkeit <gord@gnu.ai.mit.edu>, 1996
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#
# As a special exception to the GNU General Public License, if you
# distribute this file as part of a program that contains a
# configuration script generated by Autoconf, you may include it under
# the same distribution terms that you use for the rest of that program.

basename="s,^.*/,,g"

# Work around backward compatibility issue on IRIX 6.5. On IRIX 6.4+, sh
# is ksh but when the shell is invoked as "sh" and the current value of
# the _XPG environment variable is not equal to 1 (one), the special
# positional parameter $0, within a function call, is the name of the
# function.
progpath="$0"

# The name of this program:
progname=`echo "$progpath" | $SED $basename`
modename="$progname"

# Global variables:
EXIT_SUCCESS=0
EXIT_FAILURE=1

PROGRAM=ltmain.sh
PACKAGE=libtool
VERSION="1.5.26 Debian 1.5.26-4"
TIMESTAMP=" (1.1220.2.493 2008/02/01 16:58:18)"

# Be Bourne compatible (taken from Autoconf:_AS_BOURNE_COMPATIBLE).
if test -n "${ZSH_VERSION+set}" && (emulate sh) >/dev/null 2>&1; then
  emulate sh
  NULLCMD=:
  # Zsh 3.x and 4.x performs word splitting on ${1+"$@"}, which
  # is contrary to our usage.  Disable this feature.
  alias -g '${1+"$@"}'='"$@"'
  setopt NO_GLOB_SUBST
else
  case `(set -o) 2>/dev/null` in *posix*) set -o posix;; esac
fi
BIN_SH=xpg4; export BIN_SH # for Tru64
DUALCASE=1; export DUALCASE # for MKS sh

# Check that we have a working $echo.
if test "X$1" = X--no-reexec; then
  # Discard the --no-reexec flag, and continue.
  shift
elif test "X$1" = X--fallback-echo; then
  # Avoid inline document here, it may be left over
  :
elif test "X`($echo '\t') 2>/dev/null`" = 'X\t'; then
  # Yippee, $echo works!
  :
else
  # Restart under the correct shell, and then maybe $echo will work.
  exec $SHELL "$progpath" --no-reexec ${1+"$@"}
fi

if test "X$1" = X--fallback-echo; then
  # used as fallback echo
  shift
  cat <<EOF
$*
EOF
  exit $EXIT_SUCCESS
fi

default_mode=
help="Try \`$progname --help' for more information."
magic="%%%MAGIC variable%%%"
mkdir="mkdir"
mv="mv -f"
rm="rm -f"

# Sed substitution that helps us do robust quoting.  It backslashifies
# metacharacters that are still active within double-quoted strings.
Xsed="${SED}"' -e 1s/^X//'
sed_quote_subst='s/\([\\`\\"$\\\\]\)/\\\1/g'
# test EBCDIC or ASCII
case `echo X|tr X '\101'` in
 A) # ASCII based system
    # \n is not interpreted correctly by Solaris 8 /usr/ucb/tr
  SP2NL='tr \040 \012'
  NL2SP='tr \015\012 \040\040'
  ;;
 *) # EBCDIC based system
  SP2NL='tr \100 \n'
  NL2SP='tr \r\n \100\100'
  ;;
esac

# NLS nuisances.
# Only set LANG and LC_ALL to C if already set.
# These must not be set unconditionally because not all systems understand
# e.g. LANG=C (notably SCO).
# We save the old values to restore during execute mode.
lt_env=
for lt_var in LANG LANGUAGE LC_ALL LC_CTYPE LC_COLLATE LC_MESSAGES
do
  eval "if test \"\${$lt_var+set}\" = set; then
	  save_$lt_var=\$$lt_var
	  lt_env=\"$lt_var=\$$lt_var \$lt_env\"
	  $lt_var=C
	  export $lt_var
	fi"
done

if test -n "$lt_env"; then
  lt_env="env $lt_env"
fi

# Make sure IFS has a sensible default
lt_nl='
'
IFS=" 	$lt_nl"

if test "$build_libtool_libs" != yes && test "$build_old_libs" != yes; then
  $echo "$modename: not configured to build any kind of library" 1>&2
  $echo "Fatal configuration error.  See the $PACKAGE docs for more information." 1>&2
  exit $EXIT_FAILURE
fi

# Global variables.
mode=$default_mode
nonopt=
prev=
prevopt=
run=
show="$echo"
show_help=
execute_dlfiles=
duplicate_deps=no
preserve_args=
lo2o="s/\\.lo\$/.${objext}/"
o2lo="s/\\.${objext}\$/.lo/"
extracted_archives=
extracted_serial=0

#####################################
# Shell function definitions:
# This seems to be the best place for them

# func_mktempdir [string]
# Make a temporary directory that won't clash with other running
# libtool processes, and avoids race conditions if possible.  If
# given, STRING is the basename for that directory.
func_mktempdir ()
{
    my_template="${TMPDIR-/tmp}/${1-$progname}"

    if test "$run" = ":"; then
      # Return a directory name, but don't create it in dry-run mode
      my_tmpdir="${my_template}-$$"
    else

      # If mktemp works, use that first and foremost
      my_tmpdir=`mktemp -d "${my_template}-XXXXXXXX" 2>/dev/null`

      if test ! -d "$my_tmpdir"; then
	# Failing that, at least try and use $RANDOM to avoid a race
	my_tmpdir="${my_template}-${RANDOM-0}$$"

	save_mktempdir_umask=`umask`
	umask 0077
	$mkdir "$my_tmpdir"
	umask $save_mktempdir_umask
      fi

      # If we're not in dry-run mode, bomb out on failure
      test -d "$my_tmpdir" || {
        $echo "cannot create temporary directory \`$my_tmpdir'" 1>&2
	exit $EXIT_FAILURE
      }
    fi

    $echo "X$my_tmpdir" | $Xsed
}


# func_win32_libid arg
# return the library type of file 'arg'
#
# Need a lot of goo to handle *both* DLLs and import libs
# Has to be a shell function in order to 'eat' the argument
# that is supplied when $file_magic_command is called.
func_win32_libid ()
{
  win32_libid_type="unknown"
  win32_fileres=`file -L $1 2>/dev/null`
  case $win32_fileres in
  *ar\ archive\ import\ library*) # definitely import
    win32_libid_type="x86 archive import"
    ;;
  *ar\ archive*) # could be an import, or static
    if eval $OBJDUMP -f $1 | $SED -e '10q' 2>/dev/null | \
      $EGREP -e 'file format pe-i386(.*architecture: i386)?' >/dev/null ; then
      win32_nmres=`eval $NM -f posix -A $1 | \
	$SED -n -e '1,100{
		/ I /{
			s,.*,import,
			p
			q
			}
		}'`
      case $win32_nmres in
      import*)  win32_libid_type="x86 archive import";;
      *)        win32_libid_type="x86 archive static";;
      esac
    fi
    ;;
  *DLL*)
    win32_libid_type="x86 DLL"
    ;;
  *executable*) # but shell scripts are "executable" too...
    case $win32_fileres in
    *MS\ Windows\ PE\ Intel*)
      win32_libid_type="x86 DLL"
      ;;
    esac
    ;;
  esac
  $echo $win32_libid_type
}


# func_infer_tag arg
# Infer tagged configuration to use if any are available and
# if one wasn't chosen via the "--tag" command line option.
# Only attempt this if the compiler in the base compile
# command doesn't match the default compiler.
# arg is usually of the form 'gcc ...'
func_infer_tag ()
{
    if test -n "$available_tags" && test -z "$tagname"; then
      CC_quoted=
      for arg in $CC; do
	case $arg in
	  *[\[\~\#\^\&\*\(\)\{\}\|\;\<\>\?\'\ \	]*|*]*|"")
	  arg="\"$arg\""
	  ;;
	esac
	CC_quoted="$CC_quoted $arg"
      done
      case $@ in
      # Blanks in the command may have been stripped by the calling shell,
      # but not from the CC environment variable when configure was run.
      " $CC "* | "$CC "* | " `$echo $CC` "* | "`$echo $CC` "* | " $CC_quoted"* | "$CC_quoted "* | " `$echo $CC_quoted` "* | "`$echo $CC_quoted` "*) ;;
      # Blanks at the start of $base_compile will cause this to fail
      # if we don't check for them as well.
      *)
	for z in $available_tags; do
	  if grep "^# ### BEGIN LIBTOOL TAG CONFIG: $z$" < "$progpath" > /dev/null; then
	    # Evaluate the configuration.
	    eval "`${SED} -n -e '/^# ### BEGIN LIBTOOL TAG CONFIG: '$z'$/,/^# ### END LIBTOOL TAG CONFIG: '$z'$/p' < $progpath`"
	    CC_quoted=
	    for arg in $CC; do
	    # Double-quote args containing other shell metacharacters.
	    case $arg in
	      *[\[\~\#\^\&\*\(\)\{\}\|\;\<\>\?\'\ \	]*|*]*|"")
	      arg="\"$arg\""
	      ;;
	    esac
	    CC_quoted="$CC_quoted $arg"
	  done
	    case "$@ " in
	      " $CC "* | "$CC "* | " `$echo $CC` "* | "`$echo $CC` "* | " $CC_quoted"* | "$CC_quoted "* | " `$echo $CC_quoted` "* | "`$echo $CC_quoted` "*)
	      # The compiler in the base compile command matches
	      # the one in the tagged configuration.
	      # Assume this is the tagged configuration we want.
	      tagname=$z
	      break
	      ;;
	    esac
	  fi
	done
	# If $tagname still isn't set, then no tagged configuration
	# was found and let the user know that the "--tag" command
	# line option must be used.
	if test -z "$tagname"; then
	  $echo "$modename: unable to infer tagged configuration"
	  $echo "$modename: specify a tag with \`--tag'" 1>&2
	  exit $EXIT_FAILURE
#        else
#          $echo "$modename: using $tagname tagged configuration"
	fi
	;;
      esac
    fi
}


# func_extract_an_archive dir oldlib
func_extract_an_archive ()
{
    f_ex_an_ar_dir="$1"; shift
    f_ex_an_ar_oldlib="$1"

    $show "(cd $f_ex_an_ar_dir && $AR x $f_ex_an_ar_oldlib)"
    $run eval "(cd \$f_ex_an_ar_dir && $AR x \$f_ex_an_ar_oldlib)" || exit $?
    if ($AR t "$f_ex_an_ar_oldlib" | sort | sort -uc >/dev/null 2>&1); then
     :
    else
      $echo "$modename: ERROR: object name conflicts: $f_ex_an_ar_dir/$f_ex_an_ar_oldlib" 1>&2
      exit $EXIT_FAILURE
    fi
}

# func_extract_archives gentop oldlib ...
func_extract_archives ()
{
    my_gentop="$1"; shift
    my_oldlibs=${1+"$@"}
    my_oldobjs=""
    my_xlib=""
    my_xabs=""
    my_xdir=""
    my_status=""

    $show "${rm}r $my_gentop"
    $run ${rm}r "$my_gentop"
    $show "$mkdir $my_gentop"
    $run $mkdir "$my_gentop"
    my_status=$?
    if test "$my_status" -ne 0 && test ! -d "$my_gentop"; then
      exit $my_status
    fi

    for my_xlib in $my_oldlibs; do
      # Extract the objects.
      case $my_xlib in
	[\\/]* | [A-Za-z]:[\\/]*) my_xabs="$my_xlib" ;;
	*) my_xabs=`pwd`"/$my_xlib" ;;
      esac
      my_xlib=`$echo "X$my_xlib" | $Xsed -e 's%^.*/%%'`
      my_xlib_u=$my_xlib
      while :; do
        case " $extracted_archives " in
	*" $my_xlib_u "*)
	  extracted_serial=`expr $extracted_serial + 1`
	  my_xlib_u=lt$extracted_serial-$my_xlib ;;
	*) break ;;
	esac
      done
      extracted_archives="$extracted_archives $my_xlib_u"
      my_xdir="$my_gentop/$my_xlib_u"

      $show "${rm}r $my_xdir"
      $run ${rm}r "$my_xdir"
      $show "$mkdir $my_xdir"
      $run $mkdir "$my_xdir"
      exit_status=$?
      if test "$exit_status" -ne 0 && test ! -d "$my_xdir"; then
	exit $exit_status
      fi
      case $host in
      *-darwin*)
	$show "Extracting $my_xabs"
	# Do not bother doing anything if just a dry run
	if test -z "$run"; then
	  darwin_orig_dir=`pwd`
	  cd $my_xdir || exit $?
	  darwin_archive=$my_xabs
	  darwin_curdir=`pwd`
	  darwin_base_archive=`$echo "X$darwin_archive" | $Xsed -e 's%^.*/%%'`
	  darwin_arches=`lipo -info "$darwin_archive" 2>/dev/null | $EGREP Architectures 2>/dev/null`
	  if test -n "$darwin_arches"; then 
	    darwin_arches=`echo "$darwin_arches" | $SED -e 's/.*are://'`
	    darwin_arch=
	    $show "$darwin_base_archive has multiple architectures $darwin_arches"
	    for darwin_arch in  $darwin_arches ; do
	      mkdir -p "unfat-$$/${darwin_base_archive}-${darwin_arch}"
	      lipo -thin $darwin_arch -output "unfat-$$/${darwin_base_archive}-${darwin_arch}/${darwin_base_archive}" "${darwin_archive}"
	      cd "unfat-$$/${darwin_base_archive}-${darwin_arch}"
	      func_extract_an_archive "`pwd`" "${darwin_base_archive}"
	      cd "$darwin_curdir"
	      $rm "unfat-$$/${darwin_base_archive}-${darwin_arch}/${darwin_base_archive}"
	    done # $darwin_arches
      ## Okay now we have a bunch of thin objects, gotta fatten them up :)
	    darwin_filelist=`find unfat-$$ -type f -name \*.o -print -o -name \*.lo -print| xargs basename | sort -u | $NL2SP`
	    darwin_file=
	    darwin_files=
	    for darwin_file in $darwin_filelist; do
	      darwin_files=`find unfat-$$ -name $darwin_file -print | $NL2SP`
	      lipo -create -output "$darwin_file" $darwin_files
	    done # $darwin_filelist
	    ${rm}r unfat-$$
	    cd "$darwin_orig_dir"
	  else
	    cd "$darwin_orig_dir"
 	    func_extract_an_archive "$my_xdir" "$my_xabs"
	  fi # $darwin_arches
	fi # $run
	;;
      *)
        func_extract_an_archive "$my_xdir" "$my_xabs"
        ;;
      esac
      my_oldobjs="$my_oldobjs "`find $my_xdir -name \*.$objext -print -o -name \*.lo -print | $NL2SP`
    done
    func_extract_archives_result="$my_oldobjs"
}
# End of Shell function definitions
#####################################

# Darwin sucks
eval std_shrext=\"$shrext_cmds\"

disable_libs=no

# Parse our command line options once, thoroughly.
while test "$#" -gt 0
do
  arg="$1"
  shift

  case $arg in
  -*=*) optarg=`$echo "X$arg" | $Xsed -e 's/[-_a-zA-Z0-9]*=//'` ;;
  *) optarg= ;;
  esac

  # If the previous option needs an argument, assign it.
  if test -n "$prev"; then
    case $prev in
    execute_dlfiles)
      execute_dlfiles="$execute_dlfiles $arg"
      ;;
    tag)
      tagname="$arg"
      preserve_args="${preserve_args}=$arg"

      # Check whether tagname contains only valid characters
      case $tagname in
      *[!-_A-Za-z0-9,/]*)
	$echo "$progname: invalid tag name: $tagname" 1>&2
	exit $EXIT_FAILURE
	;;
      esac

      case $tagname in
      CC)
	# Don't test for the "default" C tag, as we know, it's there, but
	# not specially marked.
	;;
      *)
	if grep "^# ### BEGIN LIBTOOL TAG CONFIG: $tagname$" < "$progpath" > /dev/null; then
	  taglist="$taglist $tagname"
	  # Evaluate the configuration.
	  eval "`${SED} -n -e '/^# ### BEGIN LIBTOOL TAG CONFIG: '$tagname'$/,/^# ### END LIBTOOL TAG CONFIG: '$tagname'$/p' < $progpath`"
	else
	  $echo "$progname: ignoring unknown tag $tagname" 1>&2
	fi
	;;
      esac
      ;;
    *)
      eval "$prev=\$arg"
      ;;
    esac

    prev=
    prevopt=
    continue
  fi

  # Have we seen a non-optional argument yet?
  case $arg in
  --help)
    show_help=yes
    ;;

  --version)
    echo "\
$PROGRAM (GNU $PACKAGE) $VERSION$TIMESTAMP

Copyright (C) 2008  Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE."
    exit $?
    ;;

  --config)
    ${SED} -e '1,/^# ### BEGIN LIBTOOL CONFIG/d' -e '/^# ### END LIBTOOL CONFIG/,$d' $progpath
    # Now print the configurations for the tags.
    for tagname in $taglist; do
      ${SED} -n -e "/^# ### BEGIN LIBTOOL TAG CONFIG: $tagname$/,/^# ### END LIBTOOL TAG CONFIG: $tagname$/p" < "$progpath"
    done
    exit $?
    ;;

  --debug)
    $echo "$progname: enabling shell trace mode"
    set -x
    preserve_args="$preserve_args $arg"
    ;;

  --dry-run | -n)
    run=:
    ;;

  --features)
    $echo "host: $host"
    if test "$build_libtool_libs" = yes; then
      $echo "enable shared libraries"
    else
      $echo "disable shared libraries"
    fi
    if test "$build_old_libs" = yes; then
      $echo "enable static libraries"
    else
      $echo "disable static libraries"
    fi
    exit $?
    ;;

  --finish) mode="finish" ;;

  --mode) prevopt="--mode" prev=mode ;;
  --mode=*) mode="$optarg" ;;

  --preserve-dup-deps) duplicate_deps="yes" ;;

  --quiet | --silent)
    show=:
    preserve_args="$preserve_args $arg"
    ;;

  --tag)
    prevopt="--tag"
    prev=tag
    preserve_args="$preserve_args --tag"
    ;;
  --tag=*)
    set tag "$optarg" ${1+"$@"}
    shift
    prev=tag
    preserve_args="$preserve_args --tag"
    ;;

  -dlopen)
    prevopt="-dlopen"
    prev=execute_dlfiles
    ;;

  -*)
    $echo "$modename: unrecognized option \`$arg'" 1>&2
    $echo "$help" 1>&2
    exit $EXIT_FAILURE
    ;;

  *)
    nonopt="$arg"
    break
    ;;
  esac
done

if test -n "$prevopt"; then
  $echo "$modename: option \`$prevopt' requires an argument" 1>&2
  $echo "$help" 1>&2
  exit $EXIT_FAILURE
fi

case $disable_libs in
no) 
  ;;
shared)
  build_libtool_libs=no
  build_old_libs=yes
  ;;
static)
  build_old_libs=`case $build_libtool_libs in yes) echo no;; *) echo yes;; esac`
  ;;
esac

# If this variable is set in any of the actions, the command in it
# will be execed at the end.  This prevents here-documents from being
# left over by shells.
exec_cmd=

if test -z "$show_help"; then

  # Infer the operation mode.
  if test -z "$mode"; then
    $echo "*** Warning: inferring the mode of operation is deprecated." 1>&2
    $echo "*** Future versions of Libtool will require --mode=MODE be specified." 1>&2
    case $nonopt in
    *cc | cc* | *++ | gcc* | *-gcc* | g++* | xlc*)
      mode=link
      for arg
      do
	case $arg in
	-c)
	   mode=compile
	   break
	   ;;
	esac
      done
      ;;
    *db | *dbx | *strace | *truss)
      mode=execute
      ;;
    *install*|cp|mv)
      mode=install
      ;;
    *rm)
      mode=uninstall
      ;;
    *)
      # If we have no mode, but dlfiles were specified, then do execute mode.
      test -n "$execute_dlfiles" && mode=execute

      # Just use the default operation mode.
      if test -z "$mode"; then
	if test -n "$nonopt"; then
	  $echo "$modename: warning: cannot infer operation mode from \`$nonopt'" 1>&2
	else
	  $echo "$modename: warning: cannot infer operation mode without MODE-ARGS" 1>&2
	fi
      fi
      ;;
    esac
  fi

  # Only execute mode is allowed to have -dlopen flags.
  if test -n "$execute_dlfiles" && test "$mode" != execute; then
    $echo "$modename: unrecognized option \`-dlopen'" 1>&2
    $echo "$help" 1>&2
    exit $EXIT_FAILURE
  fi

  # Change the help message to a mode-specific one.
  generic_help="$help"
  help="Try \`$modename --help --mode=$mode' for more information."

  # These modes are in order of execution frequency so that they run quickly.
  case $mode in
  # libtool compile mode
  compile)
    modename="$modename: compile"
    # Get the compilation command and the source file.
    base_compile=
    srcfile="$nonopt"  #  always keep a non-empty value in "srcfile"
    suppress_opt=yes
    suppress_output=
    arg_mode=normal
    libobj=
    later=

    for arg
    do
      case $arg_mode in
      arg  )
	# do not "continue".  Instead, add this to base_compile
	lastarg="$arg"
	arg_mode=normal
	;;

      target )
	libobj="$arg"
	arg_mode=normal
	continue
	;;

      normal )
	# Accept any command-line options.
	case $arg in
	-o)
	  if test -n "$libobj" ; then
	    $echo "$modename: you cannot specify \`-o' more than once" 1>&2
	    exit $EXIT_FAILURE
	  fi
	  arg_mode=target
	  continue
	  ;;

	-static | -prefer-pic | -prefer-non-pic)
	  later="$later $arg"
	  continue
	  ;;

	-no-suppress)
	  suppress_opt=no
	  continue
	  ;;

	-Xcompiler)
	  arg_mode=arg  #  the next one goes into the "base_compile" arg list
	  continue      #  The current "srcfile" will either be retained or
	  ;;            #  replaced later.  I would guess that would be a bug.

	-Wc,*)
	  args=`$echo "X$arg" | $Xsed -e "s/^-Wc,//"`
	  lastarg=
	  save_ifs="$IFS"; IFS=','
 	  for arg in $args; do
	    IFS="$save_ifs"

	    # Double-quote args containing other shell metacharacters.
	    # Many Bourne shells cannot handle close brackets correctly
	    # in scan sets, so we specify it separately.
	    case $arg in
	      *[\[\~\#\^\&\*\(\)\{\}\|\;\<\>\?\'\ \	]*|*]*|"")
	      arg="\"$arg\""
	      ;;
	    esac
	    lastarg="$lastarg $arg"
	  done
	  IFS="$save_ifs"
	  lastarg=`$echo "X$lastarg" | $Xsed -e "s/^ //"`

	  # Add the arguments to base_compile.
	  base_compile="$base_compile $lastarg"
	  continue
	  ;;

	* )
	  # Accept the current argument as the source file.
	  # The previous "srcfile" becomes the current argument.
	  #
	  lastarg="$srcfile"
	  srcfile="$arg"
	  ;;
	esac  #  case $arg
	;;
      esac    #  case $arg_mode

      # Aesthetically quote the previous argument.
      lastarg=`$echo "X$lastarg" | $Xsed -e "$sed_quote_subst"`

      case $lastarg in
      # Double-quote args containing other shell metacharacters.
      # Many Bourne shells cannot handle close brackets correctly
      # in scan sets, and some SunOS ksh mistreat backslash-escaping
      # in scan sets (worked around with variable expansion),
      # and furthermore cannot handle '|' '&' '(' ')' in scan sets 
      # at all, so we specify them separately.
      *[\[\~\#\^\&\*\(\)\{\}\|\;\<\>\?\'\ \	]*|*]*|"")
	lastarg="\"$lastarg\""
	;;
      esac

      base_compile="$base_compile $lastarg"
    done # for arg

    case $arg_mode in
    arg)
      $echo "$modename: you must specify an argument for -Xcompile"
      exit $EXIT_FAILURE
      ;;
    target)
      $echo "$modename: you must specify a target with \`-o'" 1>&2
      exit $EXIT_FAILURE
      ;;
    *)
      # Get the name of the library object.
      [ -z "$libobj" ] && libobj=`$echo "X$srcfile" | $Xsed -e 's%^.*/%%'`
      ;;
    esac

    # Recognize several different file suffixes.
    # If the user specifies -o file.o, it is replaced with file.lo
    xform='[cCFSifmso]'
    case $libobj in
    *.ada) xform=ada ;;
    *.adb) xform=adb ;;
    *.ads) xform=ads ;;
    *.asm) xform=asm ;;
    *.c++) xform=c++ ;;
    *.cc) xform=cc ;;
    *.ii) xform=ii ;;
    *.class) xform=class ;;
    *.cpp) xform=cpp ;;
    *.cxx) xform=cxx ;;
    *.[fF][09]?) xform=[fF][09]. ;;
    *.for) xform=for ;;
    *.java) xform=java ;;
    *.obj) xform=obj ;;
    *.sx) xform=sx ;;
    esac

    libobj=`$echo "X$libobj" | $Xsed -e "s/\.$xform$/.lo/"`

    case $libobj in
    *.lo) obj=`$echo "X$libobj" | $Xsed -e "$lo2o"` ;;
    *)
      $echo "$modename: cannot determine name of library object from \`$libobj'" 1>&2
      exit $EXIT_FAILURE
      ;;
    esac

    func_infer_tag $base_compile

    for arg in $later; do
      case $arg in
      -static)
	build_old_libs=yes
	continue
	;;

      -prefer-pic)
	pic_mode=yes
	continue
	;;

      -prefer-non-pic)
	pic_mode=no
	continue
	;;
      esac
    done

    qlibobj=`$echo "X$libobj" | $Xsed -e "$sed_quote_subst"`
    case $qlibobj in
      *[\[\~\#\^\&\*\(\)\{\}\|\;\<\>\?\'\ \	]*|*]*|"")
	qlibobj="\"$qlibobj\"" ;;
    esac
    test "X$libobj" != "X$qlibobj" \
	&& $echo "X$libobj" | grep '[]~#^*{};<>?"'"'"' 	&()|`$[]' \
	&& $echo "$modename: libobj name \`$libobj' may not contain shell special characters."
    objname=`$echo "X$obj" | $Xsed -e 's%^.*/%%'`
    xdir=`$echo "X$obj" | $Xsed -e 's%/[^/]*$%%'`
    if test "X$xdir" = "X$obj"; then
      xdir=
    else
      xdir=$xdir/
    fi
    lobj=${xdir}$objdir/$objname

    if test -z "$base_compile"; then
      $echo "$modename: you must specify a compilation command" 1>&2
      $echo "$help" 1>&2
      exit $EXIT_FAILURE
    fi

    # Delete any leftover library objects.
    if test "$build_old_libs" = yes; then
      removelist="$obj $lobj $libobj ${libobj}T"
    else
      removelist="$lobj $libobj ${libobj}T"
    fi

    $run $rm $removelist
    trap "$run $rm $removelist; exit $EXIT_FAILURE" 1 2 15

    # On Cygwin there's no "real" PIC flag so we must build both object types
    case $host_os in
    cygwin* | mingw* | pw32* | os2*)
      pic_mode=default
      ;;
    esac
    if test "$pic_mode" = no && test "$deplibs_check_method" != pass_all; then
      # non-PIC code in shared libraries is not supported
      pic_mode=default
    fi

    # Calculate the filename of the output object if compiler does
    # not support -o with -c
    if test "$compiler_c_o" = no; then
      output_obj=`$echo "X$srcfile" | $Xsed -e 's%^.*/%%' -e 's%\.[^.]*$%%'`.${objext}
      lockfile="$output_obj.lock"
      removelist="$removelist $output_obj $lockfile"
      trap "$run $rm $removelist; exit $EXIT_FAILURE" 1 2 15
    else
      output_obj=
      need_locks=no
      lockfile=
    fi

    # Lock this critical section if it is needed
    # We use this script file to make the link, it avoids creating a new file
    if test "$need_locks" = yes; then
      until $run ln "$progpath" "$lockfile" 2>/dev/null; do
	$show "Waiting for $lockfile to be removed"
	sleep 2
      done
    elif test "$need_locks" = warn; then
      if test -f "$lockfile"; then
	$echo "\
*** ERROR, $lockfile exists and contains:
`cat $lockfile 2>/dev/null`

This indicates that another process is trying to use the same
temporary object file, and libtool could not work around it because
your compiler does not support \`-c' and \`-o' together.  If you
repeat this compilation, it may succeed, by chance, but you had better
avoid parallel builds (make -j) in this platform, or get a better
compiler."

	$run $rm $removelist
	exit $EXIT_FAILURE
      fi
      $echo "$srcfile" > "$lockfile"
    fi

    if test -n "$fix_srcfile_path"; then
      eval srcfile=\"$fix_srcfile_path\"
    fi
    qsrcfile=`$echo "X$srcfile" | $Xsed -e "$sed_quote_subst"`
    case $qsrcfile in
      *[\[\~\#\^\&\*\(\)\{\}\|\;\<\>\?\'\ \	]*|*]*|"")
      qsrcfile="\"$qsrcfile\"" ;;
    esac

    $run $rm "$libobj" "${libobj}T"

    # Create a libtool object file (analogous to a ".la" file),
    # but don't create it if we're doing a dry run.
    test -z "$run" && cat > ${libobj}T <<EOF
# $libobj - a libtool object file
# Generated by $PROGRAM - GNU $PACKAGE $VERSION$TIMESTAMP
#
# Please DO NOT delete this file!
# It is necessary for linking the library.

# Name of the PIC object.
EOF

    # Only build a PIC object if we are building libtool libraries.
    if test "$build_libtool_libs" = yes; then
      # Without this assignment, base_compile gets emptied.
      fbsd_hideous_sh_bug=$base_compile

      if test "$pic_mode" != no; then
	command="$base_compile $qsrcfile $pic_flag"
      else
	# Don't build PIC code
	command="$base_compile $qsrcfile"
      fi

      if test ! -d "${xdir}$objdir"; then
	$show "$mkdir ${xdir}$objdir"
	$run $mkdir ${xdir}$objdir
	exit_status=$?
	if test "$exit_status" -ne 0 && test ! -d "${xdir}$objdir"; then
	  exit $exit_status
	fi
      fi

      if test -z "$output_obj"; then
	# Place PIC objects in $objdir
	command="$command -o $lobj"
      fi

      $run $rm "$lobj" "$output_obj"

      $show "$command"
      if $run eval $lt_env "$command"; then :
      else
	test -n "$output_obj" && $run $rm $removelist
	exit $EXIT_FAILURE
      fi

      if test "$need_locks" = warn &&
	 test "X`cat $lockfile 2>/dev/null`" != "X$srcfile"; then
	$echo "\
*** ERROR, $lockfile contains:
`cat $lockfile 2>/dev/null`

but it should contain:
$srcfile

This indicates that another process is trying to use the same
temporary object file, and libtool could not work around it because
your compiler does not support \`-c' and \`-o' together.  If you
repeat this compilation, it may succeed, by chance, but you had better
avoid parallel builds (make -j) in this platform, or get a better
compiler."

	$run $rm $removelist
	exit $EXIT_FAILURE
      fi

      # Just move the object if needed, then go on to compile the next one
      if test -n "$output_obj" && test "X$output_obj" != "X$lobj"; then
	$show "$mv $output_obj $lobj"
	if $run $mv $output_obj $lobj; then :
	else
	  error=$?
	  $run $rm $removelist
	  exit $error
	fi
      fi

      # Append the name of the PIC object to the libtool object file.
      test -z "$run" && cat >> ${libobj}T <<EOF
pic_object='$objdir/$objname'

EOF

      # Allow error messages only from the first compilation.
      if test "$suppress_opt" = yes; then
        suppress_output=' >/dev/null 2>&1'
      fi
    else
      # No PIC object so indicate it doesn't exist in the libtool
      # object file.
      test -z "$run" && cat >> ${libobj}T <<EOF
pic_object=none

EOF
    fi

    # Only build a position-dependent object if we build old libraries.
    if test "$build_old_libs" = yes; then
      if test "$pic_mode" != yes; then
	# Don't build PIC code
	command="$base_compile $qsrcfile"
      else
	command="$base_compile $qsrcfile $pic_flag"
      fi
      if test "$compiler_c_o" = yes; then
	command="$command -o $obj"
      fi

      # Suppress compiler output if we already did a PIC compilation.
      command="$command$suppress_output"
      $run $rm "$obj" "$output_obj"
      $show "$command"
      if $run eval $lt_env "$command"; then :
      else
	$run $rm $removelist
	exit $EXIT_FAILURE
      fi

      if test "$need_locks" = warn &&
	 test "X`cat $lockfile 2>/dev/null`" != "X$srcfile"; then
	$echo "\
*** ERROR, $lockfile contains:
`cat $lockfile 2>/dev/null`

but it should contain:
$srcfile

This indicates that another process is trying to use the same
temporary object file, and libtool could not work around it because
your compiler does not support \`-c' and \`-o' together.  If you
repeat this compilation, it may succeed, by chance, but you had better
avoid parallel builds (make -j) in this platform, or get a better
compiler."

	$run $rm $removelist
	exit $EXIT_FAILURE
      fi

      # Just move the object if needed
      if test -n "$output_obj" && test "X$output_obj" != "X$obj"; then
	$show "$mv $output_obj $obj"
	if $run $mv $output_obj $obj; then :
	else
	  error=$?
	  $run $rm $removelist
	  exit $error
	fi
      fi

      # Append the name of the non-PIC object the libtool object file.
      # Only append if the libtool object file exists.
      test -z "$run" && cat >> ${libobj}T <<EOF
# Name of the non-PIC object.
non_pic_object='$objname'

EOF
    else
      # Append the name of the non-PIC object the libtool object file.
      # Only append if the libtool object file exists.
      test -z "$run" && cat >> ${libobj}T <<EOF
# Name of the non-PIC object.
non_pic_object=none

EOF
    fi

    $run $mv "${libobj}T" "${libobj}"

    # Unlock the critical section if it was locked
    if test "$need_locks" != no; then
      $run $rm "$lockfile"
    fi

    exit $EXIT_SUCCESS
    ;;

  # libtool link mode
  link | relink)
    modename="$modename: link"
    case $host in
    *-*-cygwin* | *-*-mingw* | *-*-pw32* | *-*-os2*)
      # It is impossible to link a dll without this setting, and
      # we shouldn't force the makefile maintainer to figure out
      # which system we are compiling for in order to pass an extra
      # flag for every libtool invocation.
      # allow_undefined=no

      # FIXME: Unfortunately, there are problems with the above when trying
      # to make a dll which has undefined symbols, in which case not
      # even a static library is built.  For now, we need to specify
      # -no-undefined on the libtool link line when we can be certain
      # that all symbols are satisfied, otherwise we get a static library.
      allow_undefined=yes
      ;;
    *)
      allow_undefined=yes
      ;;
    esac
    libtool_args="$nonopt"
    base_compile="$nonopt $@"
    compile_command="$nonopt"
    finalize_command="$nonopt"

    compile_rpath=
    finalize_rpath=
    compile_shlibpath=
    finalize_shlibpath=
    convenience=
    old_convenience=
    deplibs=
    old_deplibs=
    compiler_flags=
    linker_flags=
    dllsearchpath=
    lib_search_path=`pwd`
    inst_prefix_dir=

    avoid_version=no
    dlfiles=
    dlprefiles=
    dlself=no
    export_dynamic=no
    export_symbols=
    export_symbols_regex=
    generated=
    libobjs=
    ltlibs=
    module=no
    no_install=no
    objs=
    non_pic_objects=
    notinst_path= # paths that contain not-installed libtool libraries
    precious_files_regex=
    prefer_static_libs=no
    preload=no
    prev=
    prevarg=
    release=
    rpath=
    xrpath=
    perm_rpath=
    temp_rpath=
    thread_safe=no
    vinfo=
    vinfo_number=no
    single_module="${wl}-single_module"

    func_infer_tag $base_compile

    # We need to know -static, to get the right output filenames.
    for arg
    do
      case $arg in
      -all-static | -static | -static-libtool-libs)
	case $arg in
	-all-static)
	  if test "$build_libtool_libs" = yes && test -z "$link_static_flag"; then
	    $echo "$modename: warning: complete static linking is impossible in this configuration" 1>&2
	  fi
	  if test -n "$link_static_flag"; then
	    dlopen_self=$dlopen_self_static
	  fi
	  prefer_static_libs=yes
	  ;;
	-static)
	  if test -z "$pic_flag" && test -n "$link_static_flag"; then
	    dlopen_self=$dlopen_self_static
	  fi
	  prefer_static_libs=built
	  ;;
	-static-libtool-libs)
	  if test -z "$pic_flag" && test -n "$link_static_flag"; then
	    dlopen_self=$dlopen_self_static
	  fi
	  prefer_static_libs=yes
	  ;;
	esac
	bu