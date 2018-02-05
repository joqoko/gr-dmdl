#!/bin/sh
export VOLK_GENERIC=1
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/pwa/Source/gr-dmdl/python
export PATH=/home/pwa/Source/gr-dmdl/build/python:$PATH
export LD_LIBRARY_PATH=/home/pwa/Source/gr-dmdl/build/lib:$LD_LIBRARY_PATH
export PYTHONPATH=/home/pwa/Source/gr-dmdl/build/swig:$PYTHONPATH
/usr/bin/python2 /home/pwa/Source/gr-dmdl/python/qa_timer.py 
