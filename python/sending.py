#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2017 <RWTH INETS & KTH RSLAB>.
# 
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 

import os
import sys
sys.path.append(os.environ.get('GRC_HIER_PATH', os.path.expanduser('~/.grc_gnuradio')))
from gnuradio import blocks
from gnuradio import digital
from gnuradio import filter
from gnuradio import gr
from gnuradio import uhd
from gnuradio.filter import firdes
import gnuradio
from sending_hier import sending_hier # grc-generated hier_block
import dmdl
import numpy
import time

class sending(gr.hier_block2):
    """
    docstring for block sending
    """
    def __init__(self, develop_mode=1, block_id=20, constellation=gnuradio.digital.constellation_qpsk().base(), preamble=[], samp_rate=4e6, sps=4, system_time_granularity_us=5, usrp_device_address="addr=10.0.0.6", center_frequency=400000000, interframe_interval_s=0.005, t_pretx_interval_s=0.05, file_name_extension_t_control="t1TXs", file_name_extension_pending="Tfr", record_on=1, name_with_timestamp=1, tx_gain=0):
        gr.hier_block2.__init__(self,
            "sending",
            gr.io_signature(0, 0, 0),  # Input signature
            gr.io_signature(0, 0, 0)
        ) # Output signature
 
        self.message_port_register_hier_in("in")
        self.message_port_register_hier_in("reconfig_in")
        self.message_port_register_hier_out("data_frame_out")
        self.message_port_register_hier_out("ack_frame_out")
        self.message_port_register_hier_out("beacon_frame_out")
        self.message_port_register_hier_out("rts_frame_out")
        self.message_port_register_hier_out("cts_frame_out")
        self.message_port_register_hier_out("ampdu_frame_out")
        self.message_port_register_hier_out("amsdu_frame_out")
        self.message_port_register_hier_out("unknown_frame_out")
        self.message_port_register_hier_out("rx_control_out")

        ##################################################
        # Blocks
        ##################################################
        self.sending_hier_0 = sending_hier(
            block_id=block_id,
            constellation=constellation,
            develop_mode=develop_mode,
            file_name_extension_t_control=file_name_extension_t_control,
            file_name_extension_pending=file_name_extension_pending,
            interframe_interval_s=interframe_interval_s,
            preamble=preamble,
            samp_rate=samp_rate,
            sps=sps,
            system_time_granularity_us=system_time_granularity_us,
            usrp_device_address=usrp_device_address,
            center_frequency=center_frequency,
            t_pretx_interval_s=t_pretx_interval_s,
            record_on=record_on,
            name_with_timestamp=name_with_timestamp,
            tx_gain=tx_gain,
        )
        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.sending_hier_0, 'data_frame_out'), (self, 'data_frame_out'))    
        self.msg_connect((self.sending_hier_0, 'ack_frame_out'), (self, 'ack_frame_out'))    
        self.msg_connect((self.sending_hier_0, 'beacon_frame_out'), (self, 'beacon_frame_out'))    
        self.msg_connect((self.sending_hier_0, 'rts_frame_out'), (self, 'rts_frame_out'))    
        self.msg_connect((self.sending_hier_0, 'cts_frame_out'), (self, 'cts_frame_out'))    
        self.msg_connect((self.sending_hier_0, 'ampdu_frame_out'), (self, 'ampdu_frame_out'))    
        self.msg_connect((self.sending_hier_0, 'amsdu_frame_out'), (self, 'amsdu_frame_out'))    
        self.msg_connect((self.sending_hier_0, 'unknown_frame_out'), (self, 'unknown_frame_out'))    
        self.msg_connect((self.sending_hier_0, 'rx_control_out'), (self, 'rx_control_out'))    
        self.msg_connect((self, 'in'), (self.sending_hier_0, 'in'))    
        self.msg_connect((self, 'reconfig_in'), (self.sending_hier_0, 'reconfig_in'))    
