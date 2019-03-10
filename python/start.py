#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Copyright 2018 <RWTH INETS & KTH RSLAB>.
# 
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.

# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
 
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

from gnuradio import gr
import os
import sys
sys.path.append(os.environ.get('GRC_HIER_PATH', os.path.expanduser('~/.grc_gnuradio')))
from gnuradio import blocks
import dmdl
import pmt

class start(gr.hier_block2):
    """
    docstring for block start
    """
    def __init__(self, start_next_time_s=4, system_time_granularity_us=5):
        gr.hier_block2.__init__(self,
            "start",
            gr.io_signature(0, 0, 0),  # Input signature
            gr.io_signature(0, 0, 0)) # Output signature

        self.message_port_register_hier_out("B")
        ##################################################
        # Blocks
        ##################################################
        self.dmdl_start_pending_0 = dmdl.start_pending(start_next_time_s=start_next_time_s, system_time_granularity_us=system_time_granularity_us)
        self.blocks_message_strobe_0 = blocks.message_strobe(pmt.intern("TEST"), 1000)
  
        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.blocks_message_strobe_0, 'strobe'), (self.dmdl_start_pending_0, 'MSG'))
        self.msg_connect((self.dmdl_start_pending_0, 'B'), (self, 'B'))
