#########################################################################
# File Name: build.sh
# Author: ma6174
# mail: ma6174@163.com
# Created Time: 五  4/28 13:39:29 2017
#########################################################################
#!/bin/bash

pkg_name = $(shell echo CheatServer-`date +%Y%m%d-%H%M%S`.tar.gz)
echo ${pkg_name}

if [ -d ./build ]; then
rm -rf build
fi

mkdir build
cd build && cmake ..
make clean
make -j4

