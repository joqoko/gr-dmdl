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
from receiving_hier import receiving_hier # grc-generated hier_block
import dmdl
import numpy
import time

class receiving(gr.hier_block2):
    """
    docstring for block receiving
    """
    def __init__(self, develop_mode, block_id, constellation, matched_filter_coeff, mu, preamble, rx_gain, samp_rate, sps, threshold, usrp_device_address, center_frequency):
        gr.hier_block2.__init__(self,
            "receiving",
            gr.io_signature(0, 0, 0),  # Input signature
            gr.io_signature(0, 0, 0)
        ) # Output signature

        self.message_port_register_hier_in("RXI")
        self.message_port_register_hier_in("G")
        self.message_port_register_hier_out("E")
        self.message_port_register_hier_out("SNR")
        self.message_port_register_hier_out("R")
        self.message_port_register_hier_out("RXO")

        ##################################################
        # Blocks
        ##################################################
        self.receiving_hier_0 = receiving_hier(
            block_id=block_id,
            constellation=constellation,
            develop_mode=develop_mode,
            matched_filter_coeff=matched_filter_coeff,
            mu=mu,
            preamble=preamble,
            rx_gain=rx_gain,
            samp_rate=samp_rate,
            sps=sps,
            threshold=threshold,
            usrp_device_address=usrp_device_address,
            center_frequency=center_frequency,
        )

        self.msg_connect((self.receiving_hier_0, 'E'), (self, 'E'))    
        self.msg_connect((self.receiving_hier_0, 'RXO'), (self, 'RXO'))    
        self.msg_connect((self.receiving_hier_0, 'SNR'), (self, 'SNR'))    
        self.msg_connect((self.receiving_hier_0, 'R'), (self, 'R'))    
        self.msg_connect((self, 'RXI'), (self.receiving_hier_0, 'RXI'))    
        self.msg_connect((self, 'G'), (self.receiving_hier_0, 'G'))    
