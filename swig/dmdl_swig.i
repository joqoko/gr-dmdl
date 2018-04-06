/* -*- c++ -*- */

#define DMDL_API
#define DIGITAL_API
%include "gnuradio.i"			// the common stuff
//load generated python docstrings
%include "dmdl_swig_doc.i"

%{
#include "gnuradio/digital/constellation.h"
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
#include "dmdl/attribute_splitter.h"
#include "dmdl/divider.h"
#include "dmdl/counter.h"
#include "dmdl/dummy_source.h"
#include "dmdl/path.h"
#include "dmdl/start_pending.h"
#include "dmdl/rts_framing.h"
#include "dmdl/cts_framing.h"
#include "dmdl/probe.h"
#include "dmdl/baseband_derotation_simphy.h"
#include "dmdl/packetizer_simphy.h"
#include "dmdl/tx_pending_simphy.h"
#include "dmdl/tx_control_simphy.h"
#include "dmdl/rx_switch_simphy.h"
#include "dmdl/variable_rotator_simphy.h"
#include "dmdl/frame_sync_simphy.h"
#include "dmdl/attribute_assignment.h"
#include "dmdl/attribute_editor.h"
#include "dmdl/absolute_timer.h"
#include "dmdl/cmd_switch.h"
%}
%include "gnuradio/digital/constellation.h"


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

%include "dmdl/attribute_splitter.h"
GR_SWIG_BLOCK_MAGIC2(dmdl, attribute_splitter);
%include "dmdl/divider.h"
GR_SWIG_BLOCK_MAGIC2(dmdl, divider);
%include "dmdl/counter.h"
GR_SWIG_BLOCK_MAGIC2(dmdl, counter);
%include "dmdl/dummy_source.h"
GR_SWIG_BLOCK_MAGIC2(dmdl, dummy_source);
%include "dmdl/path.h"
GR_SWIG_BLOCK_MAGIC2(dmdl, path);
%include "dmdl/start_pending.h"
GR_SWIG_BLOCK_MAGIC2(dmdl, start_pending);

%include "dmdl/rts_framing.h"
GR_SWIG_BLOCK_MAGIC2(dmdl, rts_framing);
%include "dmdl/cts_framing.h"
GR_SWIG_BLOCK_MAGIC2(dmdl, cts_framing);

%include "dmdl/probe.h"
GR_SWIG_BLOCK_MAGIC2(dmdl, probe);
%include "dmdl/baseband_derotation_simphy.h"
GR_SWIG_BLOCK_MAGIC2(dmdl, baseband_derotation_simphy);

%include "dmdl/packetizer_simphy.h"
GR_SWIG_BLOCK_MAGIC2(dmdl, packetizer_simphy);


%include "dmdl/tx_pending_simphy.h"
GR_SWIG_BLOCK_MAGIC2(dmdl, tx_pending_simphy);

%include "dmdl/tx_control_simphy.h"
GR_SWIG_BLOCK_MAGIC2(dmdl, tx_control_simphy);
%include "dmdl/rx_switch_simphy.h"
GR_SWIG_BLOCK_MAGIC2(dmdl, rx_switch_simphy);

%include "dmdl/variable_rotator_simphy.h"
GR_SWIG_BLOCK_MAGIC2(dmdl, variable_rotator_simphy);

%include "dmdl/frame_sync_simphy.h"
GR_SWIG_BLOCK_MAGIC2(dmdl, frame_sync_simphy);
%include "dmdl/attribute_assignment.h"
GR_SWIG_BLOCK_MAGIC2(dmdl, attribute_assignment);
%include "dmdl/attribute_editor.h"
GR_SWIG_BLOCK_MAGIC2(dmdl, attribute_editor);

%include "dmdl/absolute_timer.h"
GR_SWIG_BLOCK_MAGIC2(dmdl, absolute_timer);

%include "dmdl/cmd_switch.h"
GR_SWIG_BLOCK_MAGIC2(dmdl, cmd_switch);
