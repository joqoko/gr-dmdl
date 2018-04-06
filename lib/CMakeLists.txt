# Copyright 2011,2012,2016 Free Software Foundation, Inc.
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
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})
list(APPEND dmdl_sources
    timer_impl.cc
    backoff_impl.cc
    timeout_impl.cc
    framing_impl.cc
    frame_analysis_impl.cc
    cmd_buffer_impl.cc
    error_detection_impl.cc
    type_check_impl.cc
    address_check_impl.cc
    resend_check_impl.cc
    replicate_impl.cc
    redundancy_remover_impl.cc
    attribute_filter_impl.cc
    carrier_sensing_impl.cc
    random_impl.cc
    attribute_splitter_impl.cc
    divider_impl.cc
    counter_impl.cc
    dummy_source_impl.cc
    path_impl.cc
    start_pending_impl.cc
    rts_framing_impl.cc
    cts_framing_impl.cc
    probe_impl.cc
    baseband_derotation_simphy_impl.cc
#    packetizer_impl.cc
    packetizer_simphy_impl.cc
    tx_pending_simphy_impl.cc
    tx_control_simphy_impl.cc
    rx_switch_simphy_impl.cc
    variable_rotator_simphy_impl.cc
    frame_sync_simphy_impl.cc
    attribute_assignment_impl.cc
    attribute_editor_impl.cc
    absolute_timer_impl.cc
    cmd_switch_impl.cc )

set(dmdl_sources "${dmdl_sources}" PARENT_SCOPE)
if(NOT dmdl_sources)
	MESSAGE(STATUS "No C++ sources... skipping lib/")
	return()
endif(NOT dmdl_sources)

add_library(gnuradio-dmdl SHARED ${dmdl_sources})
target_link_libraries(gnuradio-dmdl ${Boost_LIBRARIES} ${GNURADIO_ALL_LIBRARIES})
set_target_properties(gnuradio-dmdl PROPERTIES DEFINE_SYMBOL "gnuradio_dmdl_EXPORTS")

if(APPLE)
    set_target_properties(gnuradio-dmdl PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif(APPLE)

########################################################################
# Install built library files
########################################################################
include(GrMiscUtils)
GR_LIBRARY_FOO(gnuradio-dmdl RUNTIME_COMPONENT "dmdl_runtime" DEVEL_COMPONENT "dmdl_devel")

########################################################################
# Build and register unit test
########################################################################
include(GrTest)

include_directories(${CPPUNIT_INCLUDE_DIRS})

list(APPEND test_dmdl_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/test_dmdl.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_dmdl.cc
)

add_executable(test-dmdl ${test_dmdl_sources})

target_link_libraries(
  test-dmdl
  ${GNURADIO_RUNTIME_LIBRARIES}
  ${Boost_LIBRARIES}
  ${CPPUNIT_LIBRARIES}
  gnuradio-dmdl
)

GR_ADD_TEST(test_dmdl test-dmdl)

########################################################################
# Print summary
########################################################################
message(STATUS "Using install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Building for version: ${VERSION} / ${LIBVER}")

