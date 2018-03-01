/* -*- c++ -*- */

#define DMDL_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "dmdl_swig_doc.i"

%{
#include "dmdl/timer.h"
#include "dmdl/backoff.h"
#include "dmdl/timeout.h"
#include "dmdl/framing.h"
#include "dmdl/frame_analysis.h"
#include "dmdl/cmd_buffer.h"
#include "dmdl/error_detection.h"
#include "dmdl/type_check.h"
#include "dmdl/address_check.h"
#include "dmdl/resend_check.h"
#include "dmdl/replicate.h"
#include "dmdl/redundancy_remover.h"
#include "dmdl/attribute_filter.h"
#include "dmdl/carrier_sensing.h"
#include "dmdl/random.h"
#include "dmdl/attribute_editor.h"
#include "dmdl/attribute_splitter.h"
#include "dmdl/divider.h"
%}


%include "dmdl/timer.h"
GR_SWIG_BLOCK_MAGIC2(dmdl, timer);
%include "dmdl/backoff.h"
GR_SWIG_BLOCK_MAGIC2(dmdl, backoff);
%include "dmdl/timeout.h"
GR_SWIG_BLOCK_MAGIC2(dmdl, timeout);
%include "dmdl/framing.h"
GR_SWIG_BLOCK_MAGIC2(dmdl, framing);

%include "dmdl/frame_analysis.h"
GR_SWIG_BLOCK_MAGIC2(dmdl, frame_analysis);
%include "dmdl/cmd_buffer.h"
GR_SWIG_BLOCK_MAGIC2(dmdl, cmd_buffer);
%include "dmdl/error_detection.h"
GR_SWIG_BLOCK_MAGIC2(dmdl, error_detection);
%include "dmdl/type_check.h"
GR_SWIG_BLOCK_MAGIC2(dmdl, type_check);
%include "dmdl/address_check.h"
GR_SWIG_BLOCK_MAGIC2(dmdl, address_check);
%include "dmdl/resend_check.h"
GR_SWIG_BLOCK_MAGIC2(dmdl, resend_check);
%include "dmdl/replicate.h"
GR_SWIG_BLOCK_MAGIC2(dmdl, replicate);
%include "dmdl/redundancy_remover.h"
GR_SWIG_BLOCK_MAGIC2(dmdl, redundancy_remover);

%include "dmdl/attribute_filter.h"
GR_SWIG_BLOCK_MAGIC2(dmdl, attribute_filter);
%include "dmdl/carrier_sensing.h"
GR_SWIG_BLOCK_MAGIC2(dmdl, carrier_sensing);
%include "dmdl/random.h"
GR_SWIG_BLOCK_MAGIC2(dmdl, random);
%include "dmdl/attribute_editor.h"
GR_SWIG_BLOCK_MAGIC2(dmdl, attribute_editor);
%include "dmdl/attribute_splitter.h"
GR_SWIG_BLOCK_MAGIC2(dmdl, attribute_splitter);
%include "dmdl/divider.h"
GR_SWIG_BLOCK_MAGIC2(dmdl, divider);
