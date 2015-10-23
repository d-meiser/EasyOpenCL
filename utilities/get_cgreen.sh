#!/bin/sh

if [ ! -d cgreen ]; then
  git clone https://github.com/cgreen-devs/cgreen
fi
cd cgreen
rm -rf build
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX:BOOL=../../cgreen ..
make -j2
make install


