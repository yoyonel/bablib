if ! [ -d sdiff ]
	then
	mkdir sdiff
fi

./script_diff_sub.sh src/bablib/ src_old/bablib/ sdiff/