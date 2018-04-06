# Copyright 2011,2012 Free Software Foundation, Inc.
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

########################################################################
# Install public header files
########################################################################
install(FILES
    api.h
    timer.h
    backoff.h
    timeout.h
    framing.h
    frame_analysis.h
    cmd_buffer.h
    error_detection.h
    type_check.h
    address_check.h
    resend_check.h
    replicate.h
    redundancy_remover.h
    attribute_filter.h
    carrier_sensing.h
    random.h
    attribute_splitter.h
    divider.h
    counter.h
    dummy_source.h
    path.h
    start_pending.h
    rts_framing.h
    cts_framing.h
    probe.h
    baseband_derotation_simphy.h
    packetizer_simphy.h
    tx_pending_simphy.h
    tx_control_simphy.h
    rx_switch_simphy.h
    variable_rotator_simphy.h
    frame_sync_simphy.h
    attribute_assignment.h
    attribute_editor.h
    absolute_timer.h
    cmd_switch.h DESTINATION include/dmdl
)
