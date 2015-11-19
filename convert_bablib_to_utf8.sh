#!/bin/bash
# encodage de depart
encodeFrom='ISO-8859-1'
# encodage voulu
encodeTo='UTF-8'

# url: http://stackoverflow.com/questions/1842254/how-can-i-find-all-of-the-distinct-file-extensions-in-a-folder-hierarchy
# voir les extensions des fichiers dans le projet
# find . -type f | perl -ne 'print $1 if m/\.([^.\/]+)$/' | sort -u

# url: http://superuser.com/questions/126290/find-files-filtered-by-multiple-extensions
# find . -type f | egrep '\.(shtml|css)$'

# bash - What is the best way to count find-results?
# url: http://stackoverflow.com/questions/15663607/bash-what-is-the-best-way-to-count-find-results
# find <expr> | wc -l

# application du script sur les fichiers *.c
for filename in ` find . -type f | egrep '\.(cpp|c|h|cfg|glsl|txt|vert|frag|gem|pro|pri)$' `
do
    echo "Convert file: $filename"

    filename_tmp=$filename'.tmp'

    iconv -f $encodeFrom -t $encodeTo $filename -o $filename_tmp
    #iconv -t $encodeTo $filename -o $filename_tmp

    mv -f $filename_tmp $filename
done
