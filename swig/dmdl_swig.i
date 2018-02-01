/* -*- c++ -*- */

#define DMDL_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "dmdl_swig_doc.i"

%{
#include "dmdl/timer.h"
%}


%include "dmdl/timer.h"
GR_SWIG_BLOCK_MAGIC2(dmdl, timer);
