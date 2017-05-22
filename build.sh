#########################################################################
# File Name: build.sh
# Author: ma6174
# mail: ma6174@163.com
# Created Time: 五  4/28 13:39:29 2017
#########################################################################
#!/bin/bash

rm -rf build
mkdir build
cd build && cmake ..
make clean
make -j8

mkdir release
mkdir release/logs
mv bin release/
