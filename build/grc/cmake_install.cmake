# Install script for directory: /home/pwa/Source/gr-dmdl/grc

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/gnuradio/grc/blocks" TYPE FILE FILES
    "/home/pwa/Source/gr-dmdl/grc/dmdl_timer.xml"
    "/home/pwa/Source/gr-dmdl/grc/dmdl_backoff.xml"
    "/home/pwa/Source/gr-dmdl/grc/dmdl_timeout.xml"
    "/home/pwa/Source/gr-dmdl/grc/dmdl_framing.xml"
    "/home/pwa/Source/gr-dmdl/grc/dmdl_frame_analysis.xml"
    "/home/pwa/Source/gr-dmdl/grc/dmdl_cmd_buffer.xml"
    "/home/pwa/Source/gr-dmdl/grc/dmdl_error_detection.xml"
    "/home/pwa/Source/gr-dmdl/grc/dmdl_type_check.xml"
    "/home/pwa/Source/gr-dmdl/grc/dmdl_address_check.xml"
    "/home/pwa/Source/gr-dmdl/grc/dmdl_resend_check.xml"
    )
endif()

