INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_DMDL dmdl)

FIND_PATH(
    DMDL_INCLUDE_DIRS
    NAMES dmdl/api.h
    HINTS $ENV{DMDL_DIR}/include
        ${PC_DMDL_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    DMDL_LIBRARIES
    NAMES gnuradio-dmdl
    HINTS $ENV{DMDL_DIR}/lib
        ${PC_DMDL_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(DMDL DEFAULT_MSG DMDL_LIBRARIES DMDL_INCLUDE_DIRS)
MARK_AS_ADVANCED(DMDL_LIBRARIES DMDL_INCLUDE_DIRS)

