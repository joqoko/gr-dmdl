#!/bin/sh
export VOLK_GENERIC=1
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/pwa/Source/gr-dmdl/lib
export PATH=/home/pwa/Source/gr-dmdl/build/lib:$PATH
export LD_LIBRARY_PATH=/home/pwa/Source/gr-dmdl/build/lib:$LD_LIBRARY_PATH
export PYTHONPATH=$PYTHONPATH
test-dmdl 
