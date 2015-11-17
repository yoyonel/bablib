#!/bin/bash

for d_lib in */ ; do
	echo Lib: $d_lib
	cd "$d_lib"
	if [ -a build_and_install.sh ]; then
		echo "-> BUILD & INSTALL ..."
		./build_and_install.sh
	fi
	cd ..	
done