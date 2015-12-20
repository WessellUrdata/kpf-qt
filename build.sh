#!/bin/bash

# This build script has only been tested on Arch Linux
# It may not work on other systems

CWD=$(pwd)

if [ -e "bin/linux/kpf-qt" ] ; then
    # clean binaries
    cd bin/linux
    rm -rf kpf-qt
    cd $CWD/src
    make clean
    rm -rf Makefile
    rm -rf kpf-qt
else
    # compile binaries
    qmake kpf-qt.pro -o src/Makefile
    make -C src
    cp src/kpf-qt bin/linux/kpf-qt
fi
