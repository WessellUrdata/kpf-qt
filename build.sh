#!/bin/bash

# This build script has only been tested on Arch Linux
# It may not work on other systems


if [ -e "bin/linux/KPF" ] ; then
    make clean
else
    echo How many threads do you want to compile with? 
    read threads
    make -e threads=-j$threads
fi

