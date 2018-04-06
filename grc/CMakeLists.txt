# Copyright 2011 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
install(FILES
    dmdl_timer.xml
    dmdl_backoff.xml
    dmdl_timeout.xml
    dmdl_framing.xml
    dmdl_frame_analysis.xml
    dmdl_cmd_buffer.xml
    dmdl_error_detection.xml
    dmdl_type_check.xml
    dmdl_address_check.xml
    dmdl_resend_check.xml
    dmdl_replicate.xml
    dmdl_redundancy_remover.xml
    dmdl_attribute_filter.xml
    dmdl_carrier_sensing.xml
    dmdl_random.xml
    dmdl_attribute_splitter.xml
    dmdl_divider.xml
    dmdl_counter.xml
    dmdl_dummy_source.xml
    dmdl_path.xml
    dmdl_start.xml
    dmdl_start_pending.xml
    dmdl_rts_framing.xml
    dmdl_cts_framing.xml
    dmdl_probe.xml
    dmdl_baseband_derotation_simphy.xml
    dmdl_packetizer_simphy.xml
    dmdl_tx_pending_simphy.xml
    dmdl_sending.xml
    dmdl_tx_control_simphy.xml
    dmdl_rx_switch_simphy.xml
    dmdl_receiving.xml
    dmdl_variable_rotator_simphy.xml
    dmdl_frame_sync_simphy.xml
    dmdl_attribute_assignment.xml
    dmdl_attribute_editor.xml
    dmdl_absolute_timer.xml
    dmdl_cmd_switch.xml DESTINATION share/gnuradio/grc/blocks
)
