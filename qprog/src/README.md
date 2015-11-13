# Bablib (library)

Test sur l'utilisation de *sdiff* pour comparer la lib sur deux versions présentes
dans les sources (actuellement).
Une version avec gestion des geometry shaders et une sans.
Faudrait faire deux branches séparées, les outils githubs sont suffisants pour réaliser ce genre de taches de comparaisons/merge.

Quelques liens sur les comparaisons de source sous linux depuis un bash:
* [How do I Compare two files under Linux or UNIX?](http://www.cyberciti.biz/faq/how-do-i-compare-two-files-under-linux-or-unix/)
* [Bash scripting, loop through files in folder](http://stackoverflow.com/questions/7477543/bash-scripting-loop-through-files-in-folder-fails)
* [Filter File systems with the find Command](https://www.linode.com/docs/tools-reference/tools/filter-file-systems-with-the-find-command)
* [Find files with certain extensions](http://unix.stackexchange.com/questions/28155/find-files-with-certain-extensions): utilisation de *RegExp* dans un *find*
* [HowTo: Bash Extract Filename And Extension In Unix / Linux](http://www.cyberciti.biz/faq/unix-linux-extract-filename-and-extension-in-bash/)
* [How to perform a for each file loop by using find in shell/bash?](http://stackoverflow.com/questions/15065010/how-to-perform-a-for-each-file-loop-by-using-find-in-shell-bash)
* [Bash Guide for Beginners - Prev	Chapter 7. Conditional statements - 7.1. Introduction to if](http://tldp.org/LDP/Bash-Beginners-Guide/html/sect_07_01.html)

Le script **scrip_diff_sub.sh**:
```bash

for full_path_src_name in $(find -L $1 -regex ".*\.\(h\|cpp\)"); do
  #echo - Full name for source file: $full_path_src_name
  src_name="${full_path_src_name##*/}"
  #echo - Name for source file: $src_name
  full_path_src_name_diff=$2/$src_name
  #echo - [SEARCH] Name for source file to compute diff: $full_path_src_name_diff
  if [ -f $full_path_src_name_diff ]
  	then
  		#echo - [FOUND] Name for source file to compute diff: $full_path_src_name_diff
  		echo $full_path_src_name
  		echo $full_path_src_name_diff
  		sdiff \
  		--ignore-space-change --ignore-blank-lines --ignore-tab-expansion --ignore-all-space --suppress-common-lines \
  		$full_path_src_name $full_path_src_name_diff
  		#>> $3/$src_name.sdiff
  		#if ! [ -s $3/$src_name.sdiff ]
  		#	then
  		#		rm $3/$src_name.sdiff
  		#fi
  fi
done
```
