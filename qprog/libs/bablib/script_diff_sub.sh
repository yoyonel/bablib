
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
