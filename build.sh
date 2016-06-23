#!/bin/bash

# This build script has only been tested on Arch Linux
# It may not work on other systems


if [ -e "bin/linux/KPF" ] ; then
    make clean
else
    make
fi

