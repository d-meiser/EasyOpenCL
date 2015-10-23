#!/bin/sh

if [ ! -f cgreen/include/cgreen/cgreen.h ]; then
  git clone https://github.com/cgreen-devs/cgreen
else
  cd cgreen
  git pull
  cd -
fi
cd cgreen
rm -rf build
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX:BOOL=../../cgreen ..
make -j2
make install


