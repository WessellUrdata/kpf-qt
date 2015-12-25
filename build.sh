#!/bin/bash

# This build script has only been tested on Arch Linux
# It may not work on other systems

CWD=$(pwd)

if [ -e "bin/linux/KPF" ] ; then
    # clean binaries
    cd bin/linux
    rm -rf KPF
    cd $CWD/src
    make clean
    rm -rf Makefile
    rm -rf KPF
else
    # compile binaries
    qmake kpf-qt.pro -o src/Makefile
    make -C src
    cp src/KPF bin/linux/KPF
fi
